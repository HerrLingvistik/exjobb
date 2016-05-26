#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "common.h"
#include <iostream>
#include <string.h>

void readFile_pCoords(vector<float>& data);
void readFile_cluster(vector<float>& cluster, std::string filename, int& rowCounter);
void writeResultFile(string &resultString);

#endif
