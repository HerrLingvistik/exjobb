#include "dataReader.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

//The aim of this function is to read ASCII-data from a text file and store it into a one dimensional array.
void readFile(){
	ifstream infile;
	infile.open("./data/out5dhaxad.txt"); 
	string line;
	string number;
	int col=0, ind=0; //int row=0; int i=0;
	float startPos = -1.0;
	if(infile.is_open()){		
		
		//Read line by line
		while(getline(infile, line)){
			
			istringstream is(line);
			
			first.push_back(dimX);			
			dimY++;		
			//cout << "Dim Y = " << dimY << endl;
			
			//Read element by element
			while( getline(is, number, ' ') ) {	
				//Add all data items to a one dimensional vector.		
				if(!isdigit(number[0]))
					break;
				//Add X 
				data.push_back(startPos + 0.5*col);
				//Add Y
				data.push_back((float)stof(number));
				col++;			
				dimX++;
				//cout << "Col = " << col << endl;
				//cout << "DIM X = " << dimX << endl;
    	}	
			
			col=0;

		}	
		cout<<dimX<<endl;
		dimX/=dimY;
		count.assign(first.size(), dimX);

		/*cout << "Dimensions X: " << dimX << endl;
		cout << "Dimensions Y: " << dimY << endl;
		cout << "count.size() = : " << count.size() << endl;*/
			
		infile.close();
	}
	else
		cout<<"fail"<<endl;
}
