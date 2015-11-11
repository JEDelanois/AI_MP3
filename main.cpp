
#include <iostream>
#include "DigitClassification.h"
#include <fstream>

using namespace std;

int main( int argc, const char* argv[] )
{

    Data temp;
    temp.training("/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/traininglabels.txt", "/Users/Erik/Documents/School/AI/AI_MP3/AI_MP3/digitdata/trainingimages.txt");
    
    
    temp.printPercentageMaps(.5);
    
    
}