#include "dataReader.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* The aim of this function is to read ASCII-data from a text file and store it into
	 a two-dimensional array canvas[][]. */
void readFile(){
	ifstream infile;
	infile.open("./data/testSet.txt"); 
	string line;
	string number;
	int row=0, col=0, i=0;
	float startPos = -0.9;
	if(infile.is_open()){		
		while(getline(infile, line)){

			istringstream is(line);

			while( getline(is, number, ' ') ) {
        //canvas[row][col] = stoi(number);	
				//Added a one-dimensional data set which will probably be used since I don't think we can send in
				//2d arrays to the shaders
				//if(col==0 || col == DIM-1){
					data[i] = startPos + 0.9*col;
					data[i+1] = stof(number);
					i+=2;
				/*}else{
					data[i] = startPos + 0.9*col;
					data[i+1] = stof(number);
					i+=2;
					data[i] = startPos + 0.9*col;
					data[i+1] = stof(number);
					i+=2;
				}*/
								
				col++;			
				if(col>=DIM){
					row++;
					col=0; 
				}
    	}			
		}
		infile.close();
	}
	else
		cout<<"fail"<<endl;
}
