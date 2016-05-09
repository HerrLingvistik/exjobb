#include "normalizeAxis.h"
#include "common.h"
#include <iostream>
#include <iomanip>

using namespace std;


//This method is used to normalize the y-values of each indiviudal dimension, 
//so that the highest value of each axis will be on top of the window, and the lowest axis on the lowest point. 
//The loop is performed column by column and data values are normalized within the rage -1 to 1.
//Note that this normalization requires that all data is stored in a one-dimensional array with a set x-coordinate for each y.
void normalizeAxis(){
	
	for(int i = 0; i < 5; i++){
		//cout << setw(20) << " X" << ": " << data[1+i*10] << setw(8) <<  " Y" << ": " << data[3+i*10] << endl;
		cout << setw(2) << setw(8) <<  data[1+i*10] <<  setw(8) << data[3+i*10]<< setw(8) << data[5+i*10]<< setw(8) << setw(8) << data[7+i*10]<< setw(8) << setw(8) << data[9+i*10] << setw(8) <<endl;
	}
	
	int startPoint=1;
	int dimensionCounter=0;
	
	//Temporary values for finding min/max as needed for the feature scaling.
	float minValue = 9999999999999999;
	float maxValue = -9999999999999999;
	
	//Start on the first y.
	while(startPoint < 2*dimX*dimY && dimensionCounter < dimX) {
	
		//Find largest and smallest values per column to be used for the normalization.
		for(int i = startPoint; i < 2*dimX*dimY; ) {

			if(data[i] < minValue) {
				minValue = data[i];				
			}
			
			if(data[i] > maxValue) {
				maxValue = data[i];				
			}

			//cout << data[i] << " Index: " <<  i << endl;
			i+=dimX*2;
		}
		
		//cout << "		Smallest value: " << minValue << endl;
		//cout << "		Largest value: " << maxValue << endl;
		
		//Perform feature scaling on this column, this gives values between 0 to 1.
		for(int i = startPoint; i < 2*dimX*dimY; ) {
						
			data[i] = (data[i] - minValue)/(maxValue - minValue) * 599.0f;	
			//Change interval from -1 to 1.	
			//data[i] -= 0.5f;
			//data[i] *= 1.999f;
			//data[i] *= 2.0f;
			//cout << "					Rescaled value: " << data[i] << endl;	
			i+=dimX*2.0f;
		} 
		
		//Reset the temporary variables to be able to find min/max in the next column
		minValue = 9999999999999999;
		maxValue = -9999999999999999;
		
		dimensionCounter++;
		//cout << dimensionCounter << endl;
		//Go to the first y-value of the next dimension.
		startPoint+=2;
	}
	

	/*for(uint i = 0; i < data.size(); i+=2)
	{
	cout << setw(20) << " X" << ": " << data[i] << setw(8) <<  " Y" << ": " << data[i+1] << endl;
	}*/	
	for(int i = 0; i < 5; i++){
		//cout << setw(20) << " X" << ": " << data[1+i*10] << setw(8) <<  " Y" << ": " << data[3+i*10] << endl;
		cout << setw(2) <<  data[1+i*10] <<  setw(16) << data[3+i*10]<< setw(16) << data[5+i*10]<< setw(16) << setw(16) << data[7+i*10]<< setw(16) << setw(16) << data[9+i*10] << setw(16) <<endl;
	}

}
/*
void normalizeAxis2(){
	cout<<"normalize scatterplot values"<<endl;
	//Temporary values for finding min/max as needed for the feature scaling.
	float minValueX = 9999999999999999, minValueY = 9999999999999999;
	float maxValueX = -9999999999999999, maxValueY = -9999999999999999;
	for(uint i = 0; i<data2.size(); i+=2){
		//check x value first		
		if(data2[i] < minValueX)
			minValueX = data2[i];
		if(data2[i] > maxValueX)
			maxValueX = data2[i];
		//then check y value
		if(data2[i+1] < minValueY)
			minValueY = data2[i+1];
		if(data2[i+1] > maxValueY)
			maxValueY = data2[i+1];
	}

	for(uint i = 0; i<data2.size(); i+=2){
		//scale x value first		
		data2[i] = 2*((data2[i] - minValueX)/(maxValueX - minValueX) - 0.5);
		//then scale y value
		data2[i+1] = 2*((data2[i+1] - minValueY)/(maxValueY - minValueY) - 0.5);
	}
}*/
