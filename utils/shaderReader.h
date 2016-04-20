#ifndef SHADERREADER_H	//header guard to prevent multiple inclusion problems
#define SHADERREADER_H
 
#include <GL/gl.h>
#include <string>
#include "common.h"

using namespace std;

//pointers for vertices
//GLuint dataArray, dataBuffer, mouseBuffer, mouseArray;//,tex, fbo; 

extern GLfloat triVerts[12], mouseVerts[8], mouse2Verts[8]; 	

extern GLuint drawShader, paralellShader, mouseShader, mouseBuffer, mouseArray, tempArray, tempBuffer;

string readFile(const char* filename);

GLuint loadShaders(const char* vertexShader, const char* fragmentShader);

GLuint createStuff2(GLfloat *data, int size,GLuint shader);

GLuint createStuff(int W, int H);

GLuint createTexture(int W, int H, int active);

GLuint createFbo(GLuint tex);

void changeScatter(int x, int y, GLfloat *data, int size, GLuint shader);

#endif
