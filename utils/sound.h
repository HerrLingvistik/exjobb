#ifndef SOUND_H	//header guard to prevent multiple inclusion problems
#define SOUND_H

#include <AL/al.h>
#include <AL/alut.h>
#include "common.h"


extern ALuint source, source2; 

void initSound();
float calcVolume(int x, int y, float markerSize);
float calcGaussVolume_Parallel(int x, int y, float markerSize, float texArray[W][620]);
float calcGaussVolume_Scatter(int x, int y, float markerSize, float scatterTex[W][512]);
void playSound(float volume);
void playSound2(float volume);

#endif
