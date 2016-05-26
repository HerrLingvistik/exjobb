#include "common.h"
#include "usertest.h"

void task1(std::string &resultString, int posX, int posY, float texture[][H]){
	resultString = "1. " + resultString + to_string(texture[posX][posY]) +"\n \n";
}

void task2(std::string &resultString){
	resultString += "2. resultat \n \n"; 
}

void task3(std::string &resultString){
	resultString += "3. resultatish \n \n";
}
