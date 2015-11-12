//
//  DigitClassification.cpp
//  AI_MP3
//
//  Created by Erik on 11/11/15.
//  Copyright (c) 2015 Erik. All rights reserved.
//
#define IMAGEWIDTH 28
#define IMAGEHEIGHT 28
#define SMOOTHVALUE 1
#define NUMBERCHARS 10 // number of characters can identify


#include "DigitClassification.h"
#include <iostream>
#include <math.h>

using namespace std;



void PixArrayData::init(float val) // initializes the number of black pixels to be the given value
{
    for(int x = 0; x < (int)IMAGEWIDTH; x++)
    {
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            num_black[x][y] = val; // set all pixels to the smoothed value
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
        
        int length = images_file.tellg();
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
    soulutions.resize(0); //clear the soulutions vector
    //initialize the solution vector;
    ifstream sol_file;
    sol_file.open(test_tables_path);
    
    soulutions.resize(0);
    while(!sol_file.eof())// for the entire solution file
    {
        int temp;
        sol_file >> temp;
        soulutions.push_back(temp); //save all the solutions
    }
    
    t_images_path = test_images_path;
    
    sol_file.close();
}


void Classification::classify()
{
    predictions.resize(0); // reset the  predictions
    
    ifstream test_data;
    test_data.open(t_images_path);
    
    
    double char_probs[(int)NUMBERCHARS];// probablitiy associated with each character for each image
    
    
    //This function assumes that the text data is formatted properly
    // ie chuncks or 28x28 arrays of pixels
    while(!test_data.eof())// for all the characters in the file
    {
        for(int i = 0; i < (int)NUMBERCHARS; i++) // get probability for all characters
        {
            double total = log(dict.prior(i));// start out the total at value of the prior
            for(int y = 0; y < (int)IMAGEHEIGHT; y++)
            {
                for(int x = 0;x <(int)IMAGEWIDTH; x++)
                {
                    char c = test_data.get();
                    
                    //multipy the current val by the next pixel probalility
                    if(c == ' ')
                        total = total * log(dict.data[i].prob_white(x, y)); //if white space multiply by white probability
                    else
                        total = total * log(dict.data[i].prob_black(x, y)); //if black mark muliply by probibility it is black
                }
                test_data.get(); //scan in extra newline character
            }
            
            char_probs[i] = total;
        
            
            //chose the highest probability
            int highestidx = 0;
            double highestval = char_probs[0];
            for(int i =0; i < int(NUMBERCHARS); i++)
            {
                if(char_probs[i] > highestval)// if found a greater probabliliyt
                    highestidx = i; //switch the index
            }
            
            //push the higest index becasue that is your prediction for the current image
            int length = test_data.tellg();
            predictions.push_back(char_probs[highestidx]);
        }
        
    }
    
    test_data.close();
}



void Classification::checkSolution()
{
    double total_attempted[(int)NUMBERCHARS];
    double correct[(int)NUMBERCHARS];
    
    for(int i = 0; i < (int)NUMBERCHARS; i++)
    {
        total_attempted[i] = correct[i] =0; //clear all values
    }
    
    if(soulutions.size() != predictions.size() )
    {
        cout << "Error Solution size(" << soulutions.size()<< ") does not match Predictions size (" << predictions.size() << ")"<<  predictions[(int)predictions.size()-1]<< endl;
        return;
    }

    for(int i = 0; i < (int)soulutions.size(); i++)//for all solutions
    {
        total_attempted[soulutions[i]] = total_attempted[soulutions[i]] + 1; // increment the attempted character by one
        
        if(soulutions[i] == predictions[i])
            correct[soulutions[i]] = correct[soulutions[i]] +1; // if correct guess increment number correct by one
    }
    
    //print out data
    cout << "Smoothing Value = " << (int)SMOOTHVALUE << endl;
    cout << "Character\tCorrect\tAttempts\tPercentage" << endl;
    for(int i = 0; i <(int)NUMBERCHARS; i++)
    {
        cout << i << "\t" << correct[i] << "\t" << total_attempted[i] << "\t" << (correct[i]/total_attempted[i])*100 << "%" << endl;
    }
}

