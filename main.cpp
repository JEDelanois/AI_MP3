
#include <iostream>
#include "DigitClassification.h"
#include <fstream>

using namespace std;

int main( int argc, const char* argv[] )
{

    Classification temp;
    
    temp.init("/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/traininglabels.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/trainingimages.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/testlabels.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/testimages.txt");
     
    /*
    temp.init("/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/facedata/facedatatrainlabels.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/facedata/facedatatrain.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/facedata/facedatatestlabels.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/facedata/facedatatest.txt");
    */
    
    temp.dict.printPercentageMaps(20);
    temp.classify();
    temp.checkSolution();
    
    
}