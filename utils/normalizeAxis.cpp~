#include "normalizeAxis.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>

using namespace std;


//This method is used to normalize the y-values of each indiviudal dimension, 
//so that the highest value of each axis will be on top of the window, and the lowest axis on the lowest point. 
//The loop is performed column by column and data values are normalized within the rage -1 to 1.
//Note that this normalization requires that all data is stored in a one-dimensional array with a set x-coordinate for each y.
void normalizeAxis(){
	
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
						
			data[i] = (data[i] - minValue)/(maxValue - minValue);	
			//Change interval from -1 to 1.	
			data[i] -= 0.5;
			data[i] *= 2;
			//cout << "					Rescaled value: " << data[i] << endl;	
			i+=dimX*2;
		} 
		
		//Reset the temporary variables to be able to find min/max in the next column
		minValue = 9999999999999999;
		maxValue = -9999999999999999;
		
		dimensionCounter++;
		//cout << dimensionCounter << endl;
		//Go to the first y-value of the next dimension.
		startPoint+=2;
	}
	

	for(uint i = 0; i < data.size(); i+=2)
	{
	cout << setw(20) << " X" << ": " << data[i] << setw(8) <<  " Y" << ": " << data[i+1] << endl;
	}	


}
