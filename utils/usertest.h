#include "common.h"
#include <iostream>
#include <string.h>

void movePosParallel(int &x, int &y, float markerSize, float texArray[W][H]);

void movePosScatter(int &x, int &y, float markerSize, float scatterTex[sW][sH]);

float sampleDensityParallel(int x, int y, float markerSize, float texArray[W][H]);

float sampleDensityScatter(int x, int y, float markerSize, float scatterTex[sW][sH]);

void task1(std::string &resultString, int posX, int posY, float texture[][H], float maxValue);

void task2(std::string &resultString, int posX, int posY, float texture[][H], float maxValue);

void task3(std::string &resultString, int posX1, int posY1, int posX2, int posY2, float texture1[][H], float texture2[][H]);

void task4(std::string &resultString, int posX, int posY, float scatterTex1[][sH], float scatterMax1);
	
void task5(std::string &resultString, int posX, int posY, float scatterTex2[][sH], float scatterMax2);

void task6(std::string &resultString, int posX1, int posY1, int posX2, int posY2, float texture1[][sH], float texture2[][sH]);
