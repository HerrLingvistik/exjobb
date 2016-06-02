#include "common.h"
#include <iostream>
#include <string.h>

void movePosParallel(int &x, int &y, float markerSize, float texArray[W][H]);

void movePosScatter(int &x, int &y, float markerSize, float scatterTex[sW][sH]);

float sampleDensityParallel(int x, int y, float markerSize, float texArray[W][H]);

float sampleDensityScatter(int x, int y, float markerSize, float scatterTex[sW][sH]);

void task1(std::string &resultString, int posX, int posY, float texture[][H]);

void task2(std::string &resultString);

void task3(std::string &resultString);
