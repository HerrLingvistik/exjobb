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
	infile.open("dataset1.txt"); 
	string line;
	string number;
	int row=0, col=0;
	if(infile.is_open()){		
		while(getline(infile, line)){

			istringstream is(line);

			while( getline(is, number, ' ') ) {
        canvas[row][col] = stoi(number);					
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
