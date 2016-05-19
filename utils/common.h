/* This file includes declaration of variables that are used in multiple parts of the program. */
#ifndef INCLUDED_COMMON_H
#define INCLUDED_COMMON_H
#include <vector> 


using namespace std;

//Sets the width and height of the window for the parallel coordinates
const int W = 1299;
const int H = 620;


const int sW = 512;
const int sH = 512;

//Two dimensional array to store the parallel texture
extern float parallelTex[W][H];

//Two dimensional array to store the scatter plot texture
extern float scatterTex[sW][sH];

//extern int DIM;
extern int dimX, dimY;

//Extern vector data to store the data points
extern vector<float> data;
extern vector<float> data2;
extern vector<float> data3;

//Vectors in order to have a dynamic amount of lines drawn 
extern vector<int> first;
extern vector<int> count;

//extern float data[(DIM-1)*2*DIM*2];
 
#endif /* INCLUDED_COMMON_H */



