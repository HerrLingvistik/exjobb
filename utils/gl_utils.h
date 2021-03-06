#ifndef SHADERREADER_H	//header guard to prevent multiple inclusion problems
#define SHADERREADER_H
 
#include <GL/gl.h>
#include <string>
#include "common.h"

using namespace std;
void glErrorCheck();
string readFile(const char* filename);
GLuint loadShaders(const char* vertexShader, const char* fragmentShader);
GLuint createVertArray(GLfloat *data, int size,GLuint shader);
void createMouseMarker(GLuint &mouseArray, GLuint &mouseBuffer, GLfloat *data, int size, GLuint shader);
GLuint createTexture(int W, int H, int i);
GLuint createTexture2(int W, int H, int i);
GLuint createFbo(GLuint tex);
GLuint changeScatterPlot(int x, int y, int skip, GLfloat *data, int size, GLuint shader);
void createParallelArray(float texArray[][H], GLuint tex, float &maxValue);
void createScatterArray(float texture[][sH], GLuint tex, float &scatterMax);
GLuint loadBMP(string bmpFile);

#endif
