#include "dataReader.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//The aim of this function is to read ASCII-data from a text file and store it into a one dimensional array.
void readFile(){
	ifstream infile;
	infile.open("./data/testSet.txt"); 
	string line;
	string number;
	int col=0; //int row=0; int i=0;
	float startPos = -1.0;
	if(infile.is_open()){		
		
		//Read line by line
		while(getline(infile, line)){

			istringstream is(line);
			
			first.push_back(dimX);			

			//Read element by element
			while( getline(is, number, ' ') ) {
 						
				//Add all data items to a one dimensional vector.
				//Add X 
				data.push_back(startPos + 0.66*col);
				//Add Y
				data.push_back(stof(number));
								
				col++;			
				dimX++;
    	}	

			dimY++;		
			col=0;

		}	

		dimX/=dimY;
		count.assign(first.size(), dimY);
		
		cout << "Dimensions X: " << dimX << endl;
		cout << "Dimensions Y: " << dimY << endl;

		infile.close();
	}
	else
		cout<<"fail"<<endl;
}
