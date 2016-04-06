
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*
	THINGS TO ADD 

	- Fix problem with point sampling, giving the wrong intensity / position.

	- Also draw lines for the axes. 
	- for more axes set width of each axis 

	- add interaction mouse listener
	- add clustering?
	- add sound
	- Enable reading of dimension in beginning of file. 
	- Maybe make the file reading more general?? Work for both int and float and different formats like whitespace and so on.
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

//pointers for vertices
GLuint triVertArray, triVertBuffer, dataArray, dataBuffer, tex, fbo; 
int counter=0;

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

const int W = 1299;
const int H = 620;


vector<float> data;
vector<int> first;
vector<int> count;


uint startTex[W][H];

int dimX = 0;
int dimY = 0;
int maxPos = 0;	
int rowMax=0, colMax=0;

uint numbers[W][H];
float texArray[W][H];

float* texture = new float[ W*H ];

void glErrorCheck()
{
    int errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        printf("Failure in OpenGL %d \n", errCode);
        //exit(0);
    }
}

void writeFile(){

	ofstream myfile;
	myfile.open ("texture.txt");
	int i = 0;
	int row = 619, col=0;
	while(i<W*H){
		myfile << texture[i] << " "; 
		texArray[col][row] = texture[i];
		i++;
		col++;	
		if(col==W){
			myfile << "\n";
			row--;
			col=0;
		}
	}
	myfile.close();
}

/*
	Problem med texture blending som skriver över istället för att blenda. 
	Alternativt att den blendar och sedan clampar;
*/

void initTexture(){

	//set window color and clear last screen
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
	//use parallel coordinates shader
	glUseProgram(paralellShader);
	//bind data array containing coordinates for drawing lines between
	glBindVertexArray(dataArray);
	//enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);
	//draw lines tell opengl how many values will be sent to the shaders


	//BIND FRAMEBUFFER TO DRAW INTO TEXTURE
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//use parallel coordinates shader
	glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//disable and unbind just to be safe
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
	glDisable(GL_BLEND);


	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture);
	int i = 0, row = 0, col = 0;
	float max = 0;
	while(i<W*H){
		if(texture[i] > max){
			max = texture[i];
			maxPos = i;
			colMax=col;
			rowMax=row;
		}
		if(col == W){
			col = 0;
			row++;
		}
		col++;
		i++;
	}
	cout<<maxPos << " of " << i <<endl;
	//don't draw using the parallel coordinates shader anymore.
	glUseProgram(0);
	glUseProgram(drawShader);
	glUniform1f(glGetUniformLocation(drawShader, "maxValue"), max);
	glUseProgram(0);
	cout << "DONE WITH TEXTURE. Max value: "<<max<<" pos "<<colMax<<", "<<rowMax<<endl;	
}

void draw(){
	
	//cout<<numbers[W-1][H-1]<< endl;
	glUseProgram(drawShader);
	glBindVertexArray(triVertArray);
	//enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);    
	glUniform1i(glGetUniformLocation(drawShader, "parallelTex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindTexture(GL_TEXTURE_2D, 0); 
	//disable and unbind just to be safe
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
	//don't draw using the parallel coordinates shader anymore.
	glUseProgram(0);
	//swaps the buffers of the current window if double buffered(draw)
	//glErrorCheck();
	glutSwapBuffers();
}

void init(){
	//read data set into data array
	readFile();
	normalizeAxis();

	//temporary function
	//fillArray();	

	drawShader = loadShaders("./shaders/draw.vert", "./shaders/draw.frag");
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/*
		Send vertices to the shader which will draw lines between the coordinates(parallel coordinates).
		Take the data array and send it two floats at a time to the parallel coordinates shader
		which will use these as coordinates to draw lines between.
	*/
	glBindVertexArray(dataArray);
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*data.size(), &data.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//TRY TO CHANGE THE STRIDE OR USE INDICES!
	glVertexAttribPointer(glGetAttribLocation(paralellShader, "in_Position"),2, GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/*
		Create textuhttps://www.opengl.org/discussion_boards/showthread.php/169270-Subset-of-blending-modes-for-32-bit-integer-renderre and set attach it to a framebuffer object.
	*/
	
	//tex 1 and fbo object 1
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);  
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, W, H, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, W, H, 0, GL_RED, GL_UNSIGNED_INT, NULL);	
	//glErrorCheck();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		cout<<"texture creation not successful"<<endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	initTexture();
	cout<<"after initTexture "<<texArray[520][88]<<endl;
	writeFile();
	cout<<"after write: "<<texArray[520][88]<<endl;
}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
	glutPostRedisplay();

}

/*
	Interaction function
*/

void mouseEvent(int event, int state, int x, int y){
	if(event == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		cout<<texArray[x][y]<<endl;
	}
}
	
void mouseMove(int x, int y){	
		std::string s;
		s = std::to_string(x) + ", " + to_string(y) + " value " + to_string(texArray[x][y]);
		char const *pchar = s.c_str();
		//cout<<"hello"<<texArray[520][88]<<endl;
		glutSetWindowTitle(pchar);
		
}


int main(int argc, char **argv){	
	//initiate glut
	glutInit(&argc, argv);
	//sets the initial display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(W,H);
	//set version to be used
	glutInitContextVersion(3, 3);

	glutCreateWindow("Do you wanna roll in my 64?!");
	glutIdleFunc(idle);
	//Call to the drawing function
  glutDisplayFunc(draw);
	//initiate stuff for the drawing
	init();
	glutMouseFunc(mouseEvent);
	glutPassiveMotionFunc(mouseMove);
  // Loop required by OpenGL
  glutMainLoop();
	exit(0);
}
