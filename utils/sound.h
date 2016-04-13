#ifndef SOUND_H	//header guard to prevent multiple inclusion problems
#define SOUND_H

#include <AL/al.h>
#include <AL/alut.h>


extern ALuint source, source2; 

void initSound();
void playSound(float volume);
void playSound2(float volume);

#endif
