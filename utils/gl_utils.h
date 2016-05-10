#ifndef SHADERREADER_H	//header guard to prevent multiple inclusion problems
#define SHADERREADER_H
 
#include <GL/gl.h>
#include <string>
#include "common.h"

using namespace std;

string readFile(const char* filename);
GLuint loadShaders(const char* vertexShader, const char* fragmentShader);
GLuint createVertArray(GLfloat *data, int size,GLuint shader);
void createMouseMarker(GLuint &mouseArray, GLuint &mouseBuffer, GLfloat *data, int size, GLuint shader);
GLuint createTexture(int W, int H, int i);
GLuint createFbo(GLuint tex);
GLuint changeScatterPlot(int x, int y, GLfloat *data, int size, GLuint shader);

#endif
