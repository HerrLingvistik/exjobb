
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*
	THINGS TO ADD
	- Create FBO containing texture on which the parallel coordinates will be drawn.
	- bind the fbo when drawing the parallel coordinates
	- bind the textures when drawing to the screen, draw texture on the quad.

	- normalize all values between 0 and 1, no matter their range(feature scaling). y' = (y - min(y))/(max(y) - min(y))  
	- Also draw lines for the axes. 
	- for more axes set width of each axis 

	- add interaction mouse listener
	- add clustering?
	- add sound
*/

#define GL_GLEXT_PROTOTYPES	
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "utils/common.h"
#include "utils/dataReader.h"
#include "utils/shaderReader.h"
#include "utils/normalizeAxis.h"


using namespace std;
//Shader handles
GLuint drawShader, paralellShader;
//Pointers for vertices
GLuint triVertArray, triVertBuffer, dataArray, dataBuffer; 

//Vertices used to draw to triangles(one quad) upon which the texture will be drawn
GLfloat triVerts[] = 
{
	//top triangle
	-1.0f, 1.0f, 0.0f, 
	-1.0f, -1.0f, 0.0f, 
	1.0f, 1.0f, 0.0f,
	//bottom triangle
	-1.0f, -1.0f, 0.0f, 
	1.0f, -1.0f, 0.0f, 
	1.0f, 1.0f, 0.0f
};

//int canvas[DIM][DIM] = {0};
	//float data[2*DIM*DIM] = {0};
vector<float> data;
//int DIM = 3;
int dimX = 0;
int dimY = 0;

void draw(){
	//set window color and clear last screen
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//use parallel coordinates shader
	glUseProgram(paralellShader);
	//bind data array containing coordinates for drawing lines between
	glBindVertexArray(dataArray);
	//enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);
	//draw lines tell opengl how many values will be sent to the shaders
	//first says where each line should be drawn	
	int first[4] = {0,4,8,12};
	//count says how many vertices should be used in each strip
	int count[4] = {4,4,4,4};
	//BIND FRAMEBUFFER TO DRAW INTO TEXTURE
	glMultiDrawArrays(GL_LINE_STRIP, first, count, dimY/*(DIM-1)*2*DIM*2*/);
	//glDrawArrays(GL_LINE_STRIP, 0, 16);
	//disable and unbind just to be safe
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
	//don't draw using the parallel coordinates shader anymore.
	glUseProgram(0);
	//swaps the buffers of the current window if double buffered(draw)
	glutSwapBuffers();
}

void init(){
	//read data set into data array
	readFile();
	normalizeAxis();

	drawShader = loadShaders("./shaders/test.vert", "./shaders/test.frag");
	paralellShader = loadShaders("./shaders/paralell.vert", "./shaders/paralell.frag");

	//Generate holder for vertices in triangles!!!!!!!!!! CAFFEIN OVERLOAD!!!!!!!!!
	glGenVertexArrays(1, &triVertArray);
	glGenVertexArrays(1, &dataArray);

	/*
		Send vertices for the fullscreen quad, consisting of two triangles, to the draw shader.
		This will be used to draw the texture on when drawing to the screen. The vertices are
		sent to  be sent to a draw shader which will simply draw the result to a texture on which different 
		operations can be performed.
	*/
	glBindVertexArray(triVertArray);
	glGenBuffers(1, &triVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, triVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "in_Position"),3, GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),0);
	glDisableVertexAttribArray(0);
	
	/*
		Send vertices to the shader which will draw lines between the coordinates(parallel coordinates).
		Take the data array and send it two floats at a time to the parallel coordinates shader
		which will use these as coordinates to draw lines between.
	*/
	glBindVertexArray(dataArray);
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*sizeof(data), &data.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//TRY TO CHANGE THE STRIDE OR USE INDICES!
	glVertexAttribPointer(glGetAttribLocation(paralellShader, "in_Position"),2, GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),0);
	glDisableVertexAttribArray(0);

}

int main(int argc, char **argv){	
	//initiate glut
	glutInit(&argc, argv);
	//sets the initial display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,512);
	//set version to be used
	glutInitContextVersion(3, 3);

	glutCreateWindow("Do you wanna roll in my 64?!");

	//Call to the drawing function
  glutDisplayFunc(draw);
	//initiate stuff for the drawing
	init();
  // Loop required by OpenGL
  glutMainLoop();

	return 0;
}
