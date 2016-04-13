/* This file includes declaration of variables that are used in multiple parts of the program. */
#ifndef INCLUDED_COMMON_H
#define INCLUDED_COMMON_H
#include <vector> 

using namespace std;

//extern int DIM;
extern int dimX, dimY;

//Extern vector data to store the data points
extern vector<float> data;

//Vectors in order to have a dynamic amount of lines drawn 
extern vector<int> first;
extern vector<int> count;

//extern float data[(DIM-1)*2*DIM*2];
 
#endif /* INCLUDED_COMMON_H */



