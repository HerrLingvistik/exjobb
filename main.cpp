/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

#include "common.h"
#include "dataReader.h"

using namespace std;

int canvas[DIM][DIM] = {0};


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
