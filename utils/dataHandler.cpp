#include "dataHandler.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

float texArray[W][H];

//The aim of this function is to read ASCII-data from a text file and store it into a one dimensional array.
//This function is specifically for the parallel coordinates model with arbitrary dimensions
void readFile_pCoords(){
	
	data.clear(); //Clear the vector storing data values	

	ifstream infile;
	infile.open("./data/out5d.txt"); 
	string line;
	string number, dim;
	int col=0;
	float startPos = -1.0, axisSpacing;

	if(infile.is_open()){		
		//Read first line to get dimensions.
		getline(infile, line);
		istringstream is1(line);
		getline(is1, dim, ' ') ;
		
		axisSpacing = 2.0/(stof(dim)-1);
		cout <<"axis spacing is "<<axisSpacing<<endl;
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
				data.push_back(startPos + axisSpacing*col);
				//Add Y
				data.push_back((float)stof(number));
				col++;			
				dimX++;
				cout << number << " ";
				//cout << "Col = " << col << endl;
				//cout << "DIM X = " << dimX << endl;
    	}	
			cout <<endl;
			col=0;

		}	
		cout<<"numbers in file: "<<dimX<<endl;
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

//This function is specifically for the two dimensional scatter plot model
void readFile_sPlot(){

	data.clear(); //Clear the vector storing data values		
	
	ifstream infile;
	infile.open("./data/out5dhaxad.txt"); 
	string line;
	string number;

	int counter = 0;

	if(infile.is_open()){		

		//Read the data set line by line
		while(getline(infile, line)){
			
			istringstream is(line);
			
			first.push_back(dimX);			
			
			//Read X, stop reading when blankspace is found
			getline(is, number, ' ');
			data.push_back((float)stof(number));
			cout << data[counter] << " "; 
			counter++;
			
			//Read X, stop reading when blankspace is found
			getline(is, number, ' ');
			data.push_back((float)stof(number));
			cout << data[counter] << endl;
			counter++;
		}	
			
		infile.close();
	}
	else
		cout<<"fail"<<endl;
}

void writeFile(){

	ofstream myfile;
	myfile.open ("texture.txt");
	int i = 0;
	int row = 0, col=0;
	while(i<W*H){
		myfile << texArray[col][row] << " "; 
		i++;
		col++;	
		if(col==W){
			myfile << "\n";
			row++;
			col=0;
		}
	}
	myfile.close();
}
