//
//  DigitClassification.h
//  AI_MP3
//
//  Created by Erik on 11/11/15.
//  Copyright (c) 2015 Erik. All rights reserved.
//

#ifndef __AI_MP3__DigitClassification__
#define __AI_MP3__DigitClassification__


#define IMAGEWIDTH 28
#define IMAGEHEIGHT 28
#define SMOOTHVALUE 1
#define NUMBERCHARS 10 // number of characters can identify


#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;
#endif /* defined(__AI_MP3__DigitClassification__) */


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
private:
    double total_images; // total number of training images
    
};


