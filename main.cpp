#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>


using namespace std;

const int DIM = 10;

int canvas[DIM][DIM] = {0};


void printLines(){
	for(int i=0; i<DIM; i++){
		for(int j=0; j<DIM; j++){
			cout<<canvas[i][j]<<" ";
		}
		cout<<endl;
	}
}

void createLines(){
	int x1=0; 
	int y1, y2, dx = DIM, dy; 
	int ypos;
	for(int y=0; y<3; y++){
		y1 = rand()%DIM;
		y2 = rand()%DIM;
		dy = y2 - y1;
		for(int x=0; x<DIM; x++){
			ypos = y1 + round(dy * (x-x1) / (float)dx); 
			canvas[ypos][x] += 1;
		}
	}
}

int main(int argc, char **argv){
	//std::cout<<"hello strut"<<std::endl;
	srand(time(NULL));
	createLines();
	printLines();
	return 0;
}
