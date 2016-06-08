#include "dataHandler.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>


using namespace std;

//The aim of this function is to read ASCII-data from a text file and store it into a one dimensional array.
//This function is specifically for the parallel coordinates model with arbitrary dimensions
void readFile_pCoords(vector<float>& data, std::string filename, vector<GLfloat> &axes){
	
	data.clear(); //Clear the vector storing data values	
	ifstream infile;
	infile.open(filename); 
	string line;
	string number, dim;
	dimX = 0;
	dimY = 0;
	count.clear();
	first.clear();
	int col=0;
	float startPos = -0.96, axisSpacing;

	if(infile.is_open()){		
		//Read first line to get dimensions.
		getline(infile, line);
		istringstream is1(line);
		getline(is1, dim, ' ') ;
		axisSpacing = 1.92/(stof(dim)-1);

		while(getline(infile, line)){
			istringstream is(line);
			first.push_back(dimX);			
			dimY++;		
			
			//Read element by element
			while( getline(is, number, ' ') ) {	
				//This if-statement is only needed if one wants to load out5d	
				//if(number == ' ')
					//break;//cout << number.back() << endl;
				cout << number.back() << endl;
				//Add X 
				data.push_back(startPos + axisSpacing*col);
				//Add Y
				data.push_back((float)stof(number));
				col++;			
				dimX++;
    	}	
			col=0;
		}	

		dimX/=dimY;
		count.assign(first.size(), dimX);
		infile.close();

		for(float x=-0.96; x <= 0.96; x+=axisSpacing){
			axes.push_back((GLfloat)x);
			axes.push_back((GLfloat)-1.0f);
			axes.push_back((GLfloat)x);
			axes.push_back((GLfloat)1.0f);
		}

	}

	else cout<<"fail"<<endl;

}

void readFile_cluster(vector<float>& cluster, std::string filename, int& rowCounter){
	rowCounter = 0;
	cluster.clear(); //Clear the vector storing data values	
	ifstream infile;

	infile.open(filename); 
	string line;
	string number, dim;
	
	if(infile.is_open()){		

		while(getline(infile, line)){
			istringstream is(line);		
			rowCounter++;
			//Read element by element
			while( getline(is, number, ' ') ) {	
			
				cluster.push_back((float)stof(number));
				
    	}	
		}	

		infile.close();
	}
	else cout<<"FAIIIIIIIIIIIIIIL!!!"<<endl;
	
}

void writeFile(){

	ofstream myfile;
	myfile.open ("texture.txt");
	int i = 0;
	int row = 0, col=0;
	while(i<W*H){
		myfile << parallelTex[col][row] << " "; 
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

void writeResultFile(string &resultString){
	time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
	string hour = (timeinfo->tm_hour) > 9 ? to_string(timeinfo->tm_hour) :  "0" +to_string(timeinfo->tm_hour);
	string min = (timeinfo->tm_min) > 9 ? to_string(timeinfo->tm_min) :  "0" +to_string(timeinfo->tm_min);
	string time = "user test: " + to_string(timeinfo->tm_mday) + "-" + to_string(timeinfo->tm_mon+1) + " kl " + hour + ":" + min + ".txt";
  cout <<  time << endl;
	ofstream myfile;
	myfile.open ("./testResults/" + time);
	myfile << resultString;
	myfile.close();
	
}
