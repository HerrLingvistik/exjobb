
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*
	THINGS TO ADD  	

	- add clustering?
	- (Also draw lines for the axes.) 
	- (Maybe make the file reading more general?? Work for both int and float and different formats like whitespace and so on.)
*/

#define GL_GLEXT_PROTOTYPES	
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include "utils/common.h"
#include "utils/dataHandler.h"
#include "utils/shaderReader.h"
#include "utils/normalizeAxis.h"
#include "utils/sound.h"

using namespace std;
//Shader handles
GLuint drawShader, paralellShader, mouseShader;

//pointers for vertices
GLuint triVertArray, triVertBuffer;
GLuint dataArray, dataBuffer, mouseBuffer, mouseArray,tex, fbo; 
int counter=0, plot;
const static int PARALLEL = 0, SCATTER = 1;

//Vertices used to draw to triangles(one quad) upon which the texture will be drawn
GLfloat triVerts[12] = 
{
	//top triangle
	-1.0f, 1.0f, 
	-1.0f, -1.0f, 
	1.0f, 1.0f,
	//bottom triangle
	-1.0f, -1.0f, 
	1.0f, -1.0f,
	1.0f, 1.0f
};

GLfloat mouseVerts[8] = 
{
	-0.5f, 0.5f, 
	-0.5f, -0.5f, 
	0.5f, -0.5f, 
	0.5f, 0.5f 
};

GLfloat mouse2Verts[8] = 
{
	-0.5f, 0.5f, 
	-0.5f, -0.5f, 
	0.5f, -0.5f, 
	0.5f, 0.5f
};

//Array of audio buffer ID's
//ALuint audioBuffers[1];


vector<float> data;
vector<int> first;
vector<int> count;

//Booleans used for deciding when to play the sounds
bool mouseClick = false;
bool mouse2Click = false;

uint startTex[W][H];


int dimX = 0, dimY = 0, maxPos = 0, mouseX, mouseY, mouse2X, mouse2Y;
float maxValue, markerSize;	

float* texture = new float[ W*H ];
void init(int H, int W);
void glErrorCheck()
{
    int errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        printf("Failure in OpenGL %d \n", errCode);
        //exit(0);
    }
}

/*
	calculate position for this mouse markers vertices.
*/
void calcMouseSquare(){
	//bind data array containing coordinates for drawing lines between
	float space = (markerSize-1.0f)/2.0f;
	mouseVerts[0] = ((mouseX-space)/(float)W)*2.0-1.0;
	mouseVerts[1] = -(((mouseY-space-1.0f)/(float)H)*2.0-1.0);
	mouseVerts[2] = ((mouseX-space)/(float)W)*2.0-1.0;
	mouseVerts[3] = -(((mouseY+space)/(float)H)*2.0-1.0);
	mouseVerts[4] = ((mouseX+space)/(float)W)*2.0-1.0;
	mouseVerts[5] = -(((mouseY+space)/(float)H)*2.0-1.0);
	mouseVerts[6] = ((mouseX+space)/(float)W)*2.0-1.0;
	mouseVerts[7] = -(((mouseY-space)/(float)H)*2.0-1.0);

	mouse2Verts[0] = ((mouse2X-space)/(float)W)*2.0-1.0;
	mouse2Verts[1] = -(((mouse2Y-space-1.0f)/(float)H)*2.0-1.0);
	mouse2Verts[2] = ((mouse2X-space)/(float)W)*2.0-1.0;
	mouse2Verts[3] = -(((mouse2Y+space)/(float)H)*2.0-1.0);
	mouse2Verts[4] = ((mouse2X+space)/(float)W)*2.0-1.0;
	mouse2Verts[5] = -(((mouse2Y+space)/(float)H)*2.0-1.0);
	mouse2Verts[6] = ((mouse2X+space)/(float)W)*2.0-1.0;
	mouse2Verts[7] = -(((mouse2Y-space)/(float)H)*2.0-1.0);
}



/*
	Calculate volume for this square send in positions.
*/

float calcVolume(int x, int y){
	float sum = 0, space = (markerSize-1.0f)/2.0f;
	int elements=0;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = x+i;
			posY = y+j;
			
			if(posX>=0 && posX < W && posY>=0 && posY < H){
				cout<<texArray[posX][posY]<<", ";
				elements++;
				sum+=texArray[posX][posY];
			}	
		}
		cout<<endl;		
	}	
	cout<<endl;
	cout<<elements<<endl;
	return sum/elements;
}

/*
	Problem med texture blending som skriver över istället för att blenda. 
	Alternativt att den blendar och sedan clampar;
*/

void initTexture(){

	cout<<"INIT TEXTURE"<<glutGet(GLUT_WINDOW_WIDTH)<<" - "<<glutGet(GLUT_WINDOW_HEIGHT)<<endl;

	//set window color and clear last screen
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glClampColor(GL_CLAMP_READSource_COLOR, GL_FALSE);
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
	if(plot == PARALLEL)
		glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());
	else
		glMultiDrawArrays(GL_POINTS, &first.front(), &count.front(),count.size());
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//disable and unbind just to be safe
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
	glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	int i = 0, row = 0, arrayRow = H-1, col = 0;
	float max = 0;
	while(i<W*H){
		if(texture[i] > max){
			max = texture[i];
			maxPos = i;
		}

		texArray[col][arrayRow] = texture[i];
		col++;
		i++;
		if(col == W){
			col = 0;
			row++;
			arrayRow--;
		}
	
	}
	cout<<col<<", "<<arrayRow<<endl;
	//don't draw using the parallel coordinates shader anymore.
	glUseProgram(0);
	glUseProgram(drawShader);
	glUniform1f(glGetUniformLocation(drawShader, "maxValue"), max);
	glUseProgram(0);
	
	cout << "DONE WITH TEXTURE. Max value: "<<max<<endl;
	maxValue = max;	
}

void draw(){
	
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

	glUseProgram(mouseShader);
	//recalculate mouse marker area

	calcMouseSquare();
	glBindVertexArray(mouseArray);
	glBindBuffer(GL_ARRAY_BUFFER, mouseBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouseVerts), mouseVerts, GL_STATIC_DRAW);
	glBindVertexArray(mouseArray);
	//enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);
	glUniform3f(glGetUniformLocation(mouseShader, "color"), 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	//bind data array containing coordinates for drawing lines between
	glBindVertexArray(0);

	glBindVertexArray(mouseArray);
	glBindBuffer(GL_ARRAY_BUFFER, mouseBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouse2Verts), mouse2Verts, GL_STATIC_DRAW);
	glBindVertexArray(mouseArray);
	//enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);
	glUniform3f(glGetUniformLocation(mouseShader, "color"), 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	//bind data array containing coordinates for drawing lines between
	glBindVertexArray(0);
	//enable or disable a generic vertex attribute array
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	//swaps the buffers of the current window if double buffered(draw)
	//glErrorCheck();
	glutSwapBuffers();
}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
	glutPostRedisplay();
}

/*
	Interaction function for clicking mouse button
*/ 
void mouseEvent(int event, int state, int x, int y){

	if(event == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	
		mouseClick = true;
		float vol = calcVolume(x, y)/maxValue *2.0;
		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound(vol);
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
	
	if(event == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		mouse2Click = true;
		float vol = calcVolume(x, y)/maxValue *2.0;
		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound2(vol);	
		mouse2X = x;
		mouse2Y = y;
		glutPostRedisplay();
	}
}

/*
	Interaction function for clicking mouse button
	Here it would do to calculate the vertices only when clicking mouse and not in every frame.
*/ 
void mouseMoveClick(int x, int y){
		mouseX = x;
		mouseY = y;
		playSound(calcVolume(x, y)/maxValue);
		glutPostRedisplay();
}

/*
	Interaction function for moving mouse marker
*/
void mouseMove(int x, int y){	

	if(!mouseClick){
		mouseX = x;
		mouseY = y;		
		std::string s;
		s = "x: " + std::to_string(x) + " y: " + to_string(y) + " value " + to_string(texArray[x][y]);
		char const *pchar = s.c_str();
		glutSetWindowTitle(pchar);
		glutPostRedisplay();
	}
		
	if(!mouse2Click){
		mouse2X = x;
		mouse2Y = y;		
		std::string s;
		s = "x: " + std::to_string(x) + " y: " + to_string(y) + " value " + to_string(texArray[x][y]);
		char const *pchar = s.c_str();
		glutSetWindowTitle(pchar);
		glutPostRedisplay();
	}	
}

void keyPressed(unsigned char key, int x, int y){
	if(key == '+' && markerSize <= 21){
		cout<<"marker size increased"<<endl;
		markerSize+=2;
	}else	if(key == '-' && markerSize >=3){
		cout<<"marker size decreased"<<endl;
		markerSize-=2;
	}
	float vol1 = calcVolume(mouseX, mouseY)/maxValue *2.0;
	float vol2 = calcVolume(mouse2X, mouse2Y)/maxValue *2.0;
	playSound(vol1);
	playSound2(vol2);
}

void fKeyPressed(int key, int x, int y){
	switch(key){
		case GLUT_KEY_F1:
			cout<<"f1 pressed"<<endl;
			plot = PARALLEL;
			glutReshapeWindow(W, H);
			/*tex = createTexture(W,H);
			fbo = createFbo(tex);
			initTexture();*/
		break;
		case GLUT_KEY_F2:
			cout<<"f2 pressed"<<endl;
			plot = SCATTER;
			glutReshapeWindow(600, 600);
			/*tex = createTexture(600,600);
			fbo = createFbo(tex);
			initTexture();*/
		break;
	}
	
}

void reshape(int width, int height){
	cout<<"RESIZE"<<width<<" - "<<height<<endl<<endl;
	glViewport(0,0,(GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	tex = createTexture(width,height);
			fbo = createFbo(tex);
			initTexture();
}
void init(int W, int H){

	
	//sets the initial display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(W,H);
	//set version to be used
	glutInitContextVersion(3, 3);

	glutCreateWindow("Do you wanna roll in my 64?!");
	//glutReshapeWindow(100,100);
	cout<<"window created"<<endl;
	cout << "WIN: "<<glutGetWindow()<<endl;
	
	//Call to the drawing function
  glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	//initiate stuff for the drawing
	//init();
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(mouseEvent);
	glutPassiveMotionFunc(mouseMove);
	//glutMotionFunc(mouseMoveClick);
	
	
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(fKeyPressed);
	
 
	//plot = PARALLEL;
	//read data set into data array
	readFile();
	normalizeAxis();

	markerSize = 5.0f;	

	drawShader = loadShaders("./shaders/draw.vert", "./shaders/draw.frag");
	paralellShader = loadShaders("./shaders/paralell.vert", "./shaders/paralell.frag");
	mouseShader = loadShaders("./shaders/mouse.vert", "./shaders/mouse.frag");
	
	//Create fbos and textures 
	triVertArray = createStuff2(triVerts, sizeof(triVerts), drawShader);
	dataArray = createStuff2( &data.front(), sizeof(GL_FLOAT)*data.size(), paralellShader); 
	mouseArray = createStuff2(mouseVerts, sizeof(mouseVerts), mouseShader);	 

	createStuff(W, H);
	/*tex = createTexture(W,H);
	fbo = createFbo(tex);

	initTexture();*/
	writeFile();
	 // Loop required by OpenGL
  glutMainLoop();
}

int main(int argc, char **argv){	
	
	alutInit(&argc, argv);

	initSound();

	//initiate glut
	glutInit(&argc, argv);
	init(W, H);
	exit(0);
}
