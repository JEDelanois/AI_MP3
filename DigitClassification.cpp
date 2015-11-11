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
        
        //for all pixels in the coresponding character
        for(int y = 0; y < (int)IMAGEHEIGHT; y++)
        {
            for(int x = 0; x < (int)IMAGEWIDTH; x++)
            {
                //assume all characters that  are not a space are black
                char c = images_file.get();
                if(c != ' ')
                    data[curr_char].num_black[x][y] = data[curr_char].num_black[x][y] + 1; // increment number of black pixels
            }
            images_file.get(); // read in the newline character and discard it
        }
        
        //all of the data has now been read in
        
    }
    
    table_file.close();
    images_file.close();
}





void Data::printPercentageMaps(float percent)
{
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
        cout << endl << endl << endl << endl;
    }
    
}






