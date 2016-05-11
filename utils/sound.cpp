#include "sound.h"
#include <stdio.h>
#include <cmath>
#include <iostream>

using namespace std;

ALuint source, source2; 

void initSound(){
	
	//Create buffers for the sounds
	ALuint buffer = alutCreateBufferFromFile("var1.wav");
	ALuint buffer2 = alutCreateBufferFromFile("var2.wav");
	
	//Generate the number of sources desired 
	alGenSources(1, &source); 
	
	//Attach buffers to source using alSourcei
	alSourcei(source, AL_BUFFER, buffer);
	//Set the source to loop the sound from the buffer
	alSourcei(source, AL_LOOPING, 1);
	playSound(0);
	alSourcePlay(source); 	

	alGenSources(1, &source2); 

	alSourcei(source2, AL_BUFFER, buffer2);
	alSourcei(source2, AL_LOOPING, 1);
	playSound2(0);
	alSourcePlay(source2); 

	//Call alGetError to detect if an error was generated
	int error = alGetError(); 
	if (error) printf("%s\n", alutGetErrorString(error));
}

//Calculate volume for this square send in positions.
float calcVolume(int x, int y, float markerSize){
	float sum = 0, space = (markerSize-1.0f)/2.0f;
	int elements=0;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = x+i;
			posY = y+j;
			
			if(posX>=0 && posX < W && posY>=0 && posY < H){
				elements++;
				sum+=texArray[posX][posY];
			}	
		}		
	}	
	return sum/elements;
}


float calcGaussVolume_Parallel(int x, int y, float markerSize, float texArray[W][620]){
	
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
				sum += weight*texArray[posX][posY];
		
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
	return sum;
}

float calcGaussVolume_Scatter(int x, int y, float markerSize, float scatterTex[sW][512]){
	
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
	return sum;
}

//Play the sound. This function is called within mouseEvent, hence called if left mouse button is clicked
void playSound(float volume){
	alSourcef(source, AL_GAIN, volume);
}

//Play the sound. This function is called within mouseEvent, hence called if right mouse button is clicked
void playSound2(float volume){
	alSourcef(source2, AL_GAIN, volume);
}
