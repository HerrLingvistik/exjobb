
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

#define GL_GLEXT_PROTOTYPES
//#include "glext.h"
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

#include "utils/common.h"

#include "utils/dataReader.h"


#include "utils/shaderReader.h"

using namespace std;

GLuint drawShader, computeShader;
//pointers for vertices
GLuint triVertArray, triVertBuffer; 


GLfloat verts[] = 
{
	//top triangle
	-0.9f, 0.9f, 0.0f, //top left 
	-0.9f, -0.9f, 0.0f, //bottom left
	
	-0.9f, -0.9f, 0.0f,//bottom left
	0.9f, -0.9f, 0.0f, //bottom right
	
	0.9f, -0.9f, 0.0f, //bottom right
	0.9f, 0.9f, 0.0f, //top right

	0.9f, 0.9f, 0.0f, //top right 
	-0.9f, 0.9f, 0.0f //top left 
};

int canvas[DIM][DIM] = {0};


void printLines(){
	for(int i=0; i<DIM; i++){
		for(int j=0; j<DIM; j++){
			cout<<canvas[i][j]<<" ";
		}
		cout<<endl;
	}
}

void draw(){

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(drawShader);
	glDrawArrays(GL_LINES, 0, 8);
	glUseProgram(0);
	glutSwapBuffers();
}

void init(){
	
	drawShader = loadShaders("./shaders/test.vert", "./shaders/test.frag");

	glGenVertexArrays(1, &triVertArray);
	glBindVertexArray(triVertArray);
	glGenBuffers(1, &triVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, triVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "in_Position"),3, 		GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),0);

}

int main(int argc, char **argv){
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	
	glutInitWindowPosition(100,100);

	glutInitWindowSize(512,512);

	glutInitContextVersion(3, 2);

	glutCreateWindow("Pruttbas");

	//Call to the drawing function
  glutDisplayFunc(draw);

	init();
  // Loop require by OpenGL
  glutMainLoop();

	return 0;
}
