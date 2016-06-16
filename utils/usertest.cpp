#include "usertest.h"
#include <cmath>

void movePosParallel(int &x, int &y, float markerSize, float texArray[W][H]){
	
	float tempMax = texArray[x][y];

	float space = (markerSize-1.0f)/2.0f;
	int tempX = x, tempY=y;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = tempX+i;
			posY = tempY+j;
			if(posX>=0 && posX < W && posY>=0 && posY < H){

				if(texArray[posX][posY] > tempMax){
					x = posX;
					y = posY;
					tempMax = texArray[posX][posY];
				}
								
			}	
		}		
	}
	
}

void movePosScatter(int &x, int &y, float markerSize, float scatterTex[sW][sH]){
	float tempMax = scatterTex[x][y];

	float space = (markerSize-1.0f)/2.0f;
	int tempX = x, tempY=y;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = tempX+i;
			posY = tempY+j;
			
			if(posX>=0 && posX < sW && posY>=0 && posY < sH){

				if(scatterTex[posX][posY] > tempMax){
					x = posX;
					y = posY;
					//cout << std::to_string(scatterTex[posX][posY]) +" > "+ std::to_string(tempMax)<<endl;
					tempMax = scatterTex[posX][posY];
					
				}
						
			}	
		}		
	}
}

float sampleDensityParallel(int x, int y, float markerSize, float texArray[W][H]){
	float sigma = 1.0;
	float weight = 0;
	
	int count = 0;

	float sum = 0, space = (markerSize-1.0f)/2.0f;
	//int elements=0;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = x+i;
			posY = y+j;
			
			if(posX>=0 && posX < W && posY>=0 && posY < H){

				//elements++;
				weight = 1.0f/(2.0f * M_PI* sigma*sigma) * exp( -(pow(i, 2) + pow(j, 2))/(2.0f * sigma*sigma ));
				sum += weight*parallelTex[posX][posY];
		
				//cout << weight << " ";
				count++;

					if(count == markerSize) { 
						//cout << endl; 
						count = 0;
					}				
			}	
		}		
	}	

	//cout << "SUM IS " << sum << endl;
	//return sum;
	return parallelTex[x][y];
}

float sampleDensityScatter(int x, int y, float markerSize, float scatterTex[sW][sH]){
	float sigma = 1.0;
	float weight = 0;
	
	int count = 0;

	float sum = 0, space = (markerSize-1.0f)/2.0f;
	//int elements=0;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = x+i;
			posY = y+j;
			
			if(posX>=0 && posX < W && posY>=0 && posY < H){

				//elements++;
				weight = 1.0f/(2.0f * M_PI* sigma*sigma) * exp( -(pow(i, 2) + pow(j, 2))/(2.0f * sigma*sigma ));
				sum += weight*scatterTex[posX][posY];
		
				//cout << weight << " ";
				count++;

					if(count == markerSize) { 
						//cout << endl; 
						count = 0;
					}				
			}	
		}		
	}	

	//cout << "SUM IS " << sum << endl;
	//return sum;
	return scatterTex[x][y];
}

void task1(std::string &resultString, int posX, int posY, float texture[][H], float maxValue){
	
	resultString = resultString + to_string(texture[posX][posY]) + "," + to_string(100.0f*texture[posX][posY]/maxValue) + ", ";
}

void task2(std::string &resultString, int posX, int posY, float texture[][H], float maxValue){
	resultString = resultString + to_string(texture[posX][posY]) + "," + to_string(100.0f*texture[posX][posY]/maxValue) + ","; 
}

void task3(std::string &resultString, int posX1, int posY1, int posX2, int posY2, float texture1[][H], float texture2[][H]){
	//resultString += to_string(100.0f*texture1[posX1][posY1]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + "% vs " + to_string(100.0f*texture2[posX2][posY2]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + "%\n";
	resultString += to_string(100.0f*texture1[posX1][posY1]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + ", " + to_string(100.0f*texture2[posX2][posY2]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + ", ";
}

void task4(std::string &resultString, int posX, int posY, float scatterTex1[][sH], float scatterMax1){
	resultString = resultString + to_string(scatterTex1[posX][posY]) + ", " + to_string(100.0f*scatterTex1[posX][posY]/scatterMax1) + ", ";
}

void task5(std::string &resultString, int posX, int posY, float scatterTex2[][sH], float scatterMax2){
	resultString = resultString + to_string(scatterTex2[posX][posY]) + ", " + to_string(100.0f*scatterTex2[posX][posY]/scatterMax2) + ", ";
}

void task6(std::string &resultString, int posX1, int posY1, int posX2, int posY2, float texture1[][sH], float texture2[][sH]){
	resultString += to_string(100.0f*texture1[posX1][posY1]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + ", " + to_string(100.0f*texture2[posX2][posY2]/(texture1[posX1][posY1]+texture2[posX2][posY2])) + ", ";
}
