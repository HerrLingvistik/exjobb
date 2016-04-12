#include "sound.h"
#include <stdio.h>

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

//Play the sound. This function is called within mouseEvent, hence called if left mouse button is clicked
void playSound(float volume){
	alSourcef(source, AL_GAIN, volume);
}

//Play the sound. This function is called within mouseEvent, hence called if right mouse button is clicked
void playSound2(float volume){
	alSourcef(source2, AL_GAIN, volume);
}
