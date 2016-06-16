#include "normalizeAxis.h"
//#include "common.h"
#include <iostream>
#include <iomanip>


using namespace std;

//This method is used to normalize the y-values of each indiviudal dimension, 
//so that the highest value of each axis will be on top of the window, and the lowest axis on the lowest point. 
//The loop is performed column by column and data values are normalized within the rage -1 to 1.
//Note that this normalization requires that all data is stored in a one-dimensional array with a set x-coordinate for each y.
void normalizeAxis(vector<float>& data, vector<float>& data2){
	
	/*for(int i = 0; i < 5; i++){
		cout << setw(2) << setw(8) <<  data[1+i*10] <<  setw(8) << data[3+i*10]<< setw(8) << data[5+i*10]<< setw(8) << setw(8) << 		data[7+i*10]<< setw(8) << setw(8) << data[9+i*10] << setw(8) <<endl;
	}*/
	
	int startPoint=1;
	int dimensionCounter=0;
	
	//Temporary values for finding min/max as needed for the feature scaling.
	float minValue = 9999999999999999;
	float maxValue = -9999999999999999;

	//float minValue2 = 9999999999999999;
	//float maxValue2 = -9999999999999999;
	
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

			i+=dimX*2;
		}

		for(int i = startPoint; i < 2*dimX*dimY; ) {

			if(data2[i] < minValue) {
				minValue = data2[i];				
			}
			
			if(data2[i] > maxValue) {
				maxValue = data2[i];				
			}

			i+=dimX*2;
		}
		
		//Perform feature scaling on this column, this gives values between 0 to 1.
		for(int i = startPoint; i < 2*dimX*dimY; ) {
						
			data[i] = (data[i] - minValue)/(maxValue - minValue) * (float)H;	
			i+=dimX*2.0f;
		} 
		
		//Perform feature scaling on this column, this gives values between 0 to 1.
		for(int i = startPoint; i < 2*dimX*dimY; ) {
						
			data2[i] = (data2[i] - minValue)/(maxValue - minValue) * (float)H;	
			i+=dimX*2.0f;
		} 

		//Reset the temporary variables to be able to find min/max in the next column
		minValue = 9999999999999999;
		maxValue = -9999999999999999;

		//minValue2 = 9999999999999999;
		//maxValue2 = -9999999999999999;
		
		dimensionCounter++;
		//Go to the first y-value of the next dimension.
		startPoint+=2;
	}
}

void normalizeAxis2(vector<float>& cluster1, vector<float>& cluster2){
	/*for(int i=0; i<2*10; i+=2)
	{
		cout<< cluster[i] << " " << cluster[i+1]<<endl;
	}*/
	uint startPoint=0;
	uint dimensionCounter=0;
	
	//Temporary values for finding min/max as needed for the feature scaling.
	float minValue = 9999999999999999;
	float maxValue = -9999999999999999;
	
	//Start on the first y.
	//cout << "size in normalize: "<<cluster.size()/2.0 << endl;
	while(startPoint < cluster1.size() && dimensionCounter < 2) {

		//Find largest and smallest values per column to be used for the normalization.
		for(uint i = startPoint; i < cluster1.size(); ) {

			if(cluster1[i] < minValue) {
				minValue = cluster1[i];				
			}
			
			if(cluster1[i] > maxValue) {
				maxValue = cluster1[i];				
			}

			i+=2;
		}

		//Find largest and smallest values per column to be used for the normalization.
		for(uint i = startPoint; i < cluster2.size(); ) {

			if(cluster2[i] < minValue) {
				minValue = cluster2[i];				
			}
			
			if(cluster2[i] > maxValue) {
				maxValue = cluster2[i];				
			}

			i+=2;
		}
		
		//cout << "max of this col: " <<  maxValue << endl;
		//cout << "min of this col: " <<  minValue << endl;		
		
		//Perform feature scaling on this column, this gives values between 0 to 1.
		for(uint i = startPoint; i < cluster1.size(); ) {
						
			cluster1[i] = (cluster1[i] - minValue)/(maxValue - minValue) * sW;	
			i+=2;
		} 
		
		for(uint i = startPoint; i < cluster2.size(); ) {
						
			cluster2[i] = (cluster2[i] - minValue)/(maxValue - minValue) * sW;	
			i+=2;
		} 

		//Reset the temporary variables to be able to find min/max in the next column
		minValue = 9999999999999999;
		maxValue = -9999999999999999;
		
		dimensionCounter++;
		//Go to the first y-value of the next dimension.
		startPoint+=1;
	}
	/*for(int i=0; i<2*699; i+=2)
	{
		cout<< cluster[i] << " " << cluster[i+1]<<endl;
	}*/
}

//Output the normalized values of the data set
/*void outputNormalized(){

	for(int i = 0; i < 5; i++){
		cout << setw(2) <<  data[1+i*10] <<  setw(16) << data[3+i*10]<< setw(16) << data[5+i*10]<< setw(16) << setw(16) << 
		data[7+i*10]<< setw(16) << setw(16) << data[9+i*10] << setw(16) <<endl;
	}
}*/

