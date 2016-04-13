#include "dataReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//The aim of this function is to read ASCII-data from a text file and store it into a one dimensional array.
void readFile(){
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
				//cout << "Col = " << col << endl;
				//cout << "DIM X = " << dimX << endl;
    	}	
			
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
