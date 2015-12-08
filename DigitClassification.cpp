//
//  DigitClassification.cpp
//  AI_MP3
//
//  Created by Erik on 11/11/15.
//  Copyright (c) 2015 Erik. All rights reserved.
//



#include "DigitClassification.h"
#include <iostream>
#include <math.h>

using namespace std;



void PixArrayData::init(float val) // initializes the number of black pixels to be the given value
{float test;
    for(int x = 0; x < (int)IMAGEWIDTH; x++)
    {
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            test = num_black[x][y] = val; // set all pixels to the smoothed value
        }
    }
    
    total_occ = 0;
    
}

double PixArrayData::prob_black(int x, int y) //returns the probability that the given x y cood will be black
{
    return (num_black[x][y] / total_occ);
}

double PixArrayData::prob_white(int x, int y) //returns the probability that the given x y cood will be white
{
    return (1 - prob_black(x, y));
}



void Data::training(string tables_path, string images_path)
{
    ifstream table_file;
    ifstream images_file;
    
    table_file.open(tables_path);
    images_file.open(images_path);
    
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        data[i].init((float)SMOOTHVALUE); // init all arrays to the smooth value
    }
    
    while(!table_file.eof()) // while there are characters to scan in
    {
        int curr_char;
        table_file >> curr_char; // read in the current index we are looking at
        //error checking
        if((curr_char < 0) ||(curr_char >= NUMBERCHARS))
        {
            cout << "Tried to read in invalid character" << endl;
            return;
        }
        
        //if no error then valid so increment the total number of occurences for that index
        data[curr_char].total_occ = data[curr_char].total_occ + 1;
        char c;
        //for all pixels in the coresponding character
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            for(int x = 0; x < (int)IMAGEWIDTH; x++)
            {
                //assume all characters that  are not a space are black
                c = images_file.get();
                if(c != ' ')
                    data[curr_char].num_black[x][y] = data[curr_char].num_black[x][y] + 1; // increment number of black pixels
                
                //if it is white you need to do nothing
            }
            images_file.get(); // read in the newline character and discard it
        }
        
        int length = (int)images_file.tellg();
        length++;
        //all of the data has now been read in
        
    }
    
    table_file.close();
    images_file.close();
}





void Data::printPercentageMaps(float percent)
{
    percent = percent / 100;
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        for(int y = 0; y < IMAGEHEIGHT; y++)
        {
            for(int x = 0; x < IMAGEWIDTH; x++)
            {
                if(data[i].prob_black(x, y) >= percent)
                    cout << "*";
                else
                    cout << " ";
                
            }
            cout << endl;
        }
        cout << "'*' represent pixels that are more than " << percent*100 << "% likely to occure" << endl;
        cout << data[i].total_occ <<" occurances in training"<<endl;
        cout << prior(i) << " is the prior for this character"<<endl;
        cout  << endl;
    }
    
}


double Data::prior(int idx)
{
    double grandtotal_occ = 0;
    
    for(int i = 0; i < (int)NUMBERCHARS; i ++)
    {
        grandtotal_occ += data[i].total_occ;
    }
    
    return (data[idx].total_occ / grandtotal_occ);
    
}

void Classification::init(string training_tables_path, string training_images_path, string test_tables_path, string test_images_path)
{
    dict.training(training_tables_path, training_images_path);
    
    testSoulutions.resize(0); //clear the soulutions vector
    //initialize the solution vector;
    ifstream sol_file;
    sol_file.open(test_tables_path);
    
    testSoulutions.resize(0);
    while(!sol_file.eof())// for the entire solution file
    {
        int temp;
        sol_file >> temp;
        testSoulutions.push_back(temp); //save all the solutions
    }
    
    ifstream trainsol_file;
    trainsol_file.open(training_tables_path);
    
    trainSoulutions.resize(0);
    while(!trainsol_file.eof())// for the entire solution file
    {
        int temp;
        trainsol_file >> temp;
        trainSoulutions.push_back(temp); //save all the solutions
    }
    
    t_images_path = test_images_path;
    loadData(test_images_path,testData);
    loadData(training_images_path,trainData);
    sol_file.close();
    trainsol_file.close();
    
}

void Classification::loadData(string filepath,vector<vector<vector<char>>> & d )
{
    ifstream dataFile;
    dataFile.open(filepath);
    //reset entire array
    d.resize(0);
    
    
   
    
    // current character
    int i = 0;
    while(!dataFile.eof())// for the entire solution file
    {
        
        //add new character
        d.push_back( vector<vector<char>>() );
        
        for(int y = 0; y < IMAGEHEIGHT; y++)
        {
            //add a new row
            d[i].push_back( vector<char>() );
            for(int x = 0; x < IMAGEWIDTH + 1; x++)
            {
                
                //scan in that row
                char c = dataFile.get();
                d[i][y].push_back(c);
            }
        }
        
        
        
        i++;
    }
    
    
}

void Classification::printImage(int idx)
{
    for(int y = 0; y < (int)IMAGEHEIGHT; y++)
    {
        for(int x = 0; x < (int)IMAGEWIDTH + 1 ; x++)
        {
            cout << testData[idx][y][x];
        }
    }
}


void Classification::Adjustweight(Weight & weight, vector<vector<char>> & charArray, double a)
{
    for(int y = 0; y < (int)IMAGEHEIGHT; y++)
    {
        for(int x = 0; x < (int)IMAGEWIDTH ; x++)
        {
           
            if(charArray[y][x] == ' ')// if no pixel present then do nothing
            {
                
            }
            else
            {
                weight.val[x][y] = weight.val[x][y] + (a * 1);
                            }
            
            
        }
    }
    //adjust b val if
    //if testing without bvals then do so
    if(shouldAddB)
        weight.b += weight.b + (a*1);

    
}


void Classification::perceptronTrain()
{
    if(trainData.size() != trainSoulutions.size())
    {
        cout << "Training image vector size does not match Training solution vector"  << trainData.size() << " " <<trainSoulutions.size() << endl;
        return;
    }
    
    if(shouldTrainRandom)
    {
        cout << "ERROR NEED TO IMPLEMENT RANDOM TRAINING!!!!!!!!!!!!!!!!!!!!!"<< endl;
    }
    else // train not randomly
    {
        bool needTrain = true;
        double epochNum = 0;
        double a = 1; // Alpha value
        int lastNumWrong = INT_MAX;
        int numWrong = 0;
        int chainWrong = 0;
        
        while(needTrain) // while you need to train
        {
            needTrain = false;// assume you dont need to train
            
            lastNumWrong = numWrong;
            numWrong = 0;
            
            for(int i = 0; i < (int)trainData.size(); i++)// for all of the trainin images
            {
                
                int prediction = -1;
                double predicVal = -DBL_MAX;// assume negative double max for prediction
            
            
                for(int j = 0; j < (int)NUMBERCHARS; j++) // for all of the possible character (ie number of weiht vectors)
                {
                    double temp = getDotPrdouct(weights[j], trainData[i]); // getdot product val
                    if(temp >= predicVal) // if found a better prediction change your prediction
                    {
                        prediction = j;
                        predicVal = temp;
                    }
                }
                
                //you now have your prediction so see if it is correct and adjust weight if you need to
                
                if(prediction != trainSoulutions[i]) // if prediction is wrong
                {
                    numWrong++;
                    //upddate weights
                    
                    //decrease wrong one
                    Adjustweight(weights[prediction], trainData[i], -a);
                    
                    //increase correct one
                    Adjustweight(weights[trainSoulutions[i]], trainData[i], a);
                }
                
                
            }
        
            
            // handle if it is impossible to improve weights any more
            if(lastNumWrong == numWrong)
            {
                chainWrong++;
            }
            else
            {
                chainWrong = 0;
            }
            
            
            if(chainWrong == 100)// if no improvement for 100 iterations then just quit
                needTrain = false;
            
            if(numWrong == 0)// if nothing is wrong then you are done training
                needTrain = false;
        
            
            epochNum++;
            a = 100/(100+epochNum); // get new alpha value
        }
        
    }
}

double Classification::getDotPrdouct(Weight & weight,vector<vector<char>> & charArray ) // weight array and the image of the character array
{
    //calculate the dot product
    double total = 0;
    for(int x = 0; x < (int)IMAGEWIDTH; x++)
    {
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            //charArray has backwards index format so this is correct !!!!
            if(charArray[y][x] == ' ')
            {
                // if there is a space add nothing to the total
            }
            
            else// else add the pixel multiplied by the weight to the total
                total += weight.val[x][y] * 1;
        }
    }
    
    if(shouldAddB)
        total += weight.b * 1; // if you should add the bias value then add it
    
    return total;
}


void Classification::classify()
{
    predictions.resize(0); // reset the  predictions
    
    ifstream test_data;
    test_data.open(t_images_path);
    
    
    double char_probs[(int)NUMBERCHARS];// probablitiy associated with each character for each image
    //int length;
    
    //This function assumes that the text data is formatted properly
    // ie chuncks or 28x28 arrays of pixels
    int currIdx = 0;
    while(!test_data.eof())// for all the characters in the file
    {
        //initilaze all probabilites to be their prior values for each picture
        for(int i = 0; i < (int)NUMBERCHARS; i++)
        {
            char_probs[i] = log(dict.prior(i));
        }
        
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            for(int x = 0;x <(int)IMAGEWIDTH; x++)
            {
                char c = test_data.get();
                
                for(int i = 0; i < (int)NUMBERCHARS; i++) // get probability for all characters
                {

                    //multipy the current val by the next pixel probalility
                    if(c == ' ')
                        char_probs[i] = char_probs[i] + log(dict.data[i].prob_white(x, y)); //if white space multiply by white probability
                    else
                        char_probs[i] = char_probs[i] +  log(dict.data[i].prob_black(x, y)); //if black mark muliply by probibility it is black
                    
                    
                }
            }
            test_data.get(); //scan in extra newline character
        }
            
    
        
        //chose the highest probability
        int highestidx = 0;
        highestidx = 0;
        double highestval;
        highestval = char_probs[0];
        for(int z = 0; z < int(NUMBERCHARS); z++)
        {
            if(char_probs[z] > highestval)// if found a greater probabliliyt
            {
                highestidx = z; //switch the index
                highestval = char_probs[z];
            }
        }
        //check if anything needs to be added to examples
        
        //looking for smallest negative number
        if(highestval > examps[highestidx].max_val)
        {
            //save the highest index max value
            examps[highestidx].max_val = highestval;
            
            //save the index of the image
            examps[highestidx].max_idx = currIdx;
        }
        
        //looking for biggest negative number
        else if(highestval < examps[highestidx].min_val)
        {
            //save the highest index max value
            examps[highestidx].min_val = highestval;
            
            //save the index of the image
            examps[highestidx].min_idx = currIdx;
        }
        
        //push the higest index becasue that is your prediction for the current image
        predictions.push_back(highestidx);
        
        
        currIdx++;
        
    }
    
    test_data.close();
}


void Classification::printOdds(int char1, int char2, double tolerance)
{
    cout << "This is the odds ratio for " << char1 << "/" << char2 << " :"<< endl;
    for(int y = 0; y < IMAGEHEIGHT; y++)
    {
        for(int x = 0;x< IMAGEWIDTH; x++)
        {
            if( (dict.data[char1].prob_black(x, y) /dict.data[char2].prob_black(x, y)) > 1+tolerance)
                cout << "+";
            else if( (dict.data[char1].prob_black(x, y) /dict.data[char2].prob_black(x, y)) < 1 - tolerance)
                cout << "-";
            else
                cout << " ";
        }
        
        cout << endl;
        
    }
    
    cout << endl << endl;
    
    
}



void Classification::checkSolution()
{
    double total_attempted[(int)NUMBERCHARS];
    double correct[(int)NUMBERCHARS];
    
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        total_attempted[i] = correct[i] =0; //clear all values
    }
    
    if(testSoulutions.size() != predictions.size() )
    {
        cout << "Error Solution size(" << testSoulutions.size()<< ") does not match Predictions size (" << predictions.size() << ")"<< endl;
        return;
    }

    for(int i = 0; i < (int)testSoulutions.size(); i++)//for all solutions
    {
        total_attempted[testSoulutions[i]] = total_attempted[testSoulutions[i]] + 1; // increment the attempted character by one
        
        if(testSoulutions[i] == predictions[i])
            correct[testSoulutions[i]] = correct[testSoulutions[i]] +1; // if correct guess increment number correct by one
    }
    
    //print out data
    cout << "Smoothing Value = " << (int)SMOOTHVALUE << endl << endl << endl;
    cout << "Character\tCorrect\t\tAttempts\tPercentage" << endl;
    for(int i = 0; i <(int)NUMBERCHARS; i++)
    {
        cout << i << "\t\t\t" << correct[i] << "\t\t\t" << total_attempted[i] << "\t\t\t" << (correct[i]/total_attempted[i])*100 << "%" << endl;
    }
    float grand_total = 0;
    float total_right = 0;
    
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        grand_total += total_attempted[i];
        total_right += correct[i];
    }
    cout << "Total\t\t" << total_right << "\t\t\t" << grand_total << "\t\t\t" << (total_right/grand_total)*100 << "%" << endl;
    
    
    
    
    
    //create classification matrix
    double conf[NUMBERCHARS][NUMBERCHARS];
    
    //initialize the confusion matrix
    for(int a = 0; a < (int)NUMBERCHARS; a++)
    {
         for(int b = 0; b < (int)NUMBERCHARS; b++)
         {
             conf[a][b] = 0;
         }
    }
    
    for(int i = 0; i < (int)testSoulutions.size(); i++)
    {
        conf[testSoulutions[i]][predictions[i]] = conf[testSoulutions[i]][predictions[i]] +1;
    }
    
    cout << endl << endl << endl <<"Confusion Matrix:" << endl;
    
    for(int a = 0; a < (int)NUMBERCHARS; a++)
    {
        cout << "\t\t" << a;
    }
    
    cout << endl;
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(3);
    
    for(int a = 0; a < (int)NUMBERCHARS; a++)
    {
        cout << a ;
        for(int b = 0; b < (int)NUMBERCHARS; b++)
        {
            cout << "\t" << (conf[a][b] / total_attempted[a]) * 100 ;
        }
        cout << endl << endl;
    }
    
    /*
    for(int i = 0; i < 1000; i++)
        cout << soulutions[i] << "  " << predictions[i]<<endl;
     */
    
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        cout << "For Character '" << i << "' this is the most prototypical image:" << endl;
        
        printImage(examps[i].max_idx);
        
        
        cout << "For Character '" << i << "' this is the least prototypical image:" << endl;
        
        printImage(examps[i].min_idx);
        
        
        
    }
    
    
    printOdds( 0, 5, .1);
    printOdds( 5, 3, .1);
    printOdds( 8, 3, .1);
    printOdds( 8, 9, .1);
    
}

