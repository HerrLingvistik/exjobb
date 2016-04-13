
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*
	THINGS TO ADD 

	- Make sound be based on density in region instead of in pixel. 	

	- Fix problem with point sampling, giving the wrong intensity / position.

	- Also draw lines for the axes. 

	- add clustering?
	- add sound
	- Maybe make the file reading more general?? Work for both int and float and different formats like whitespace and so on.
*/

#define GL_GLEXT_PROTOTYPES	
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include "utils/common.h"
#include "utils/dataReader.h"
#include "utils/shaderReader.h"
#include "utils/normalizeAxis.h"
#include "utils/sound.h"


using namespace std;
//Shader handles
GLuint drawShader, paralellShader, mouseShader;

//pointers for vertices
GLuint triVertArray, triVertBuffer;
GLuint dataArray, dataBuffer, mouseBuffer, mouseArray,tex, fbo; 
int counter=0;

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

const int W = 1299;
const int H = 620;


vector<float> data;
vector<int> first;
vector<int> count;

//Booleans used for deciding when to play the sounds
bool mouseClick = false;
bool mouse2Click = false;

uint startTex[W][H];


int dimX = 0, dimY = 0, maxPos = 0, mouseX, mouseY, mouse2X, mouse2Y;
float maxValue, markerSize;	

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
	int row = 0, col=0;
	while(i<W*H){
		myfile << texArray[col][row] << " "; 
		i++;
		col++;	
		if(col==W){
			myfile << "\n";
			row++;
			col=0;
		}
	}
	myfile.close();
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
	glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//disable and unbind just to be safe
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
	glDisable(GL_BLEND);


	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture);
	int i = 0, row = 0, arrayRow = 619, col = 0;
	float max = 0;
	while(i<W*H){
		if(texture[i] > max){
			max = texture[i];
			maxPos = i;
		}

		texArray[col][arrayRow] = texture[i];

		col++;
		i++;
		//cout<<col<<", "<<arrayRow<<endl;

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

void init(){
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

	createStuff();

	initTexture();
	writeFile();
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
		//cout<<<<endl;
		float vol = calcVolume(x, y)/maxValue *2.0;
		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound(vol);
		//alSourcePlay(source); 
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
	
	if(event == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		mouse2Click = true;
		//cout<<<<endl;
		float vol = calcVolume(x, y)/maxValue *2.0;
		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound2(vol);	
		//alSourcePlay(source2); 
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

void resizeKey(unsigned char key, int x, int y){
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


int main(int argc, char **argv){	
	
	alutInit(&argc, argv);

	initSound();

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
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(mouseEvent);
	glutPassiveMotionFunc(mouseMove);
	//glutMotionFunc(mouseMoveClick);
	glutKeyboardFunc(resizeKey);

  // Loop required by OpenGL
  glutMainLoop();
	exit(0);
}
