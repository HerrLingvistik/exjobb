#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

#include "common.h"
#include "dataReader.cpp"

using namespace std;

int canvas[DIM][DIM] = {0};

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
	}
	else
		cout<<"fail"<<endl;
}

void printLines(){
	for(int i=0; i<DIM; i++){
		for(int j=0; j<DIM; j++){
			cout<<canvas[i][j]<<" ";
		}
		cout<<endl;
	}
}

int main(int argc, char **argv){
	readFile();
	printLines();
	
	return 0;
}
