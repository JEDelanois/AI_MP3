//
//  DigitClassification.h
//  AI_MP3
//
//  Created by Erik on 11/11/15.
//  Copyright (c) 2015 Erik. All rights reserved.
//

#ifndef __AI_MP3__DigitClassification__
#define __AI_MP3__DigitClassification__


//for numbers
#define IMAGEWIDTH 28
#define IMAGEHEIGHT 28
#define SMOOTHVALUE 1
#define NUMBERCHARS 10 // number of characters can identify

/*
//for faces
#define IMAGEWIDTH 60
#define IMAGEHEIGHT 70
#define SMOOTHVALUE 10
#define NUMBERCHARS 2 // number of characters can identify
*/
 
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <float.h>

using namespace std;


class PixArrayData //this is super simple but i am adding it as a class incase i want to store extra data about each array later
{
public:
    double num_black[(int)IMAGEWIDTH][(int)IMAGEHEIGHT]; // each position holds the numer of times the coresponding pixel has been black
                                // the difference between black occurences and total occurances is assumed to be # of white occurences
    double total_occ; // total number of times this number has occured in training
    
    void init(float val); // initializes the number of black pixels to be the given value
    double prob_black(int x, int y); //returns the probability that the given x y cood will be black
    double prob_white(int x, int y); //returns the probability that the given x y cood will be white
    
private:
    
};




class Data
{
public:
    void training(string tables_path, string images_path);
    PixArrayData data[(int)NUMBERCHARS];
    void printPercentageMaps(float percent);
    double prior(int idx); // returns the percentage (less than 1) of the value associated with that index's prior
private:
    double total_images; // total number of training images
    
};



class Weight
{
public:
    Weight(){
        for(int x = 0; x < (int)IMAGEWIDTH; x++){
            for(int y = 0; y < (int)IMAGEHEIGHT; y++)
            {
                val[x][y] = 0;
            }
        }
        
    }
    
    double val[(int)IMAGEWIDTH][(int)IMAGEHEIGHT];  // weight values
    double b = 0;                                   // b value
    int getWidth(){return (int)IMAGEWIDTH;}
    int getHeight(){return (int)IMAGEHEIGHT;}
private:
};




/*
 CANNOT HAVE ANY NEWLINE OR BLANK CHARACTERS AT THE END OF ANY DATA INPUT FILES
 */
class Classification
{
    
public:
    Data dict;
    
    
    void init(string training_tables_path, string training_images_path, string test_tables_path, string test_images_path);
    // initializes the data element and the character arrays for the test data

    void classify(); // classify all characters in file specified by t_images_path and store the predictions into the solution array
    void checkSolution();
    //STUFF FOR PERCEPTRON
    
    void perceptronTrain(); // trains based off of the given training data in trainData
    
    
    
private:
    vector<int> trainSoulutions;
    vector<int> testSoulutions;
    vector<int> predictions;
    string t_images_path; // path to the test images
    
                                //data[image number][y][x] is the notation that you need to use
    vector<vector<vector<char>>> trainData;
    vector<vector<vector<char>>> testData;
    
    
    
    void loadData(string filepath,vector<vector<vector<char>>> & d );
    void printImage(int idx); // prints the image of the data of teh coresponding index
    void printOdds(int char1, int char2 ,double tolerance);
    
    //used to save and print extreame vals for data examples
    class examples
    {
    public:
        double max_val = -DBL_MAX;
        int max_idx = 0;
        double min_val = DBL_MAX;
        int min_idx = 0;
    };
    
    examples examps[(int)NUMBERCHARS];
    
    
    //STUFF FOR PERCEPTRON
    bool shouldAddB = true;
    bool shouldTrainRandom = false;
    Weight weights[(int)NUMBERCHARS]; // get a weight vector for every possible char
    double getDotPrdouct(Weight & weight,vector<vector<char>> & charArray ); // returns the value of the dot product of the perceptron
                                                                            // weight and the current character vector
    void Adjustweight(Weight & weight, vector<vector<char>> & charArray, double a);
    
};



#endif /* defined(__AI_MP3__DigitClassification__) */




