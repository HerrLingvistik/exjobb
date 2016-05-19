#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "common.h"

void readFile_pCoords();
void readFile_cluster(vector<float>& cluster, const char* filename, int& rowCounter);
void writeFile();

#endif
