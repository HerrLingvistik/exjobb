
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*

	THINGS TO ADD 	
	
	- Återupprätta Hallströms heder - sänk familjen Wallenberg

	- Problem med skalning i fönster när x=1 eller y=-1. Använder just nu Ingis version då man aldrig skalar upp till 1.0. 	

	- add clustering - kmeans? Ska detta användas?

*/

#define GL_GLEXT_PROTOTYPES	
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "utils/common.h"
#include "utils/dataHandler.h"
#include "utils/gl_utils.h"
#include "utils/normalizeAxis.h"
#include "utils/sound.h"

using namespace std;
//Shader handles
GLuint drawShader, parallelShader, mouseShader, tempScatterShader, drawScatterShader, drawTexShader;

//Pointers for vertices
GLuint triVertArray, triVertArray2, triVertBuffer, triVertBuffer2, scatterAxisArray;
GLuint dataArray, mouseBuffer, mouseArray,tex, fbo, parTex, parFbo,  scatTex, scatFbo, tex2, tex3, fbo2, fbo3, tempArray, tempArray2; 
int counter=0, plot, scatterAxisX = 1, scatterAxisY = 2, background = 0;
const static int PARALLEL = 0, SCATTER = 1, BLACK = 0, WHITE = 1;
bool xPressed = false, yPressed = false, hoover = false; 

float texArray[W][H];
float scatterTex[sW][sH];

//Vertices used to draw to triangles(one quad) upon which the texture will be drawn
GLfloat triVerts[12] = 
{
	//Top triangle
	-1.0f, 1.0f, 
	-1.0f, -1.0f, 
	1.0f, 1.0f,
	//Bottom triangle
	-1.0f, -1.0f, 
	1.0f, -1.0f,
	1.0f, 1.0f
};

GLfloat scatterAxisPoints[6] =
{
	//Top left point
	-0.9f, 0.9f,
	//Bottom left point
	-0.9f, -0.9f,
	//Bottom right point
	0.9f, -0.9f
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

vector<float> data;
vector<int> first;
vector<int> count;

//Booleans used for deciding when to play the sounds
bool mouseClick = false;
bool mouse2Click = false;

uint startTex[W][H];

int dimX = 0, dimY = 0, maxPos = 0, mouseX, mouseY, mouse2X, mouse2Y;
float maxValue=0, markerSize;	

//https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glActiveTexture.xml
float* texture = new float[ W*H ];

void init(int W, int H);

void glErrorCheck()
{
    int errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        printf("Failure in OpenGL %d \n", errCode);
        //exit(0);
    }
}

//http://stackoverflow.com/questions/927358/how-do-you-undo-the-last-commit
//Calculate position for this mouse markers vertices.
void calcMouseSquare(){
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	//Bind data array containing coordinates for drawing lines between
	float space = (markerSize-1.0f)/2.0f;
	mouseVerts[0] = ((mouseX-space)/(float)width)*2.0-1.0;
	mouseVerts[1] = -(((mouseY-space-1.0f)/(float)height)*2.0-1.0);
	mouseVerts[2] = ((mouseX-space)/(float)width)*2.0-1.0;
	mouseVerts[3] = -(((mouseY+space)/(float)height)*2.0-1.0);
	mouseVerts[4] = ((mouseX+space)/(float)width)*2.0-1.0;
	mouseVerts[5] = -(((mouseY+space)/(float)height)*2.0-1.0);
	mouseVerts[6] = ((mouseX+space)/(float)width)*2.0-1.0;
	mouseVerts[7] = -(((mouseY-space)/(float)height)*2.0-1.0);

	mouse2Verts[0] = ((mouse2X-space)/(float)width)*2.0-1.0;
	mouse2Verts[1] = -(((mouse2Y-space-1.0f)/(float)height)*2.0-1.0);
	mouse2Verts[2] = ((mouse2X-space)/(float)width)*2.0-1.0;
	mouse2Verts[3] = -(((mouse2Y+space)/(float)height)*2.0-1.0);
	mouse2Verts[4] = ((mouse2X+space)/(float)width)*2.0-1.0;
	mouse2Verts[5] = -(((mouse2Y+space)/(float)height)*2.0-1.0);
	mouse2Verts[6] = ((mouse2X+space)/(float)width)*2.0-1.0;
	mouse2Verts[7] = -(((mouse2Y-space)/(float)height)*2.0-1.0);
}


//Calculate volume for this square send in positions.
float calcVolume(int x, int y){
	float sum = 0, space = (markerSize-1.0f)/2.0f;
	int elements=0;
	int posX = 0, posY = 0;
	for(int j=-space; j<=space; j++){
		for(int i=-space; i<=space; i++){
			posX = x+i;
			posY = y+j;
			
			if(posX>=0 && posX < W && posY>=0 && posY < H){
				elements++;
				sum+=texArray[posX][posY];
			}	
		}	
	}	

	return sum/elements;
}

//Problem med texture blending som skriver över istället för att blenda. 
//Alternativt att den blendar och sedan clampar;
void initTexture(GLuint fboTemp, GLuint texTemp){
	
	//set window color and clear last screenpixel
	//glClearColor(0,0,0,0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	
	//Bind data array containing coordinates for drawing lines between
	if(plot == PARALLEL){	
		glUseProgram(parallelShader);
		cout << "create parallel texture"<<endl;
		glBindVertexArray(dataArray);
		glEnableVertexAttribArray(0);

		//Draw lines tell opengl how many values will be sent to the shaders
		//Bind frambuffer to draw into texture
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//use parallel coordinates shader
		glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);	
		glDisable(GL_BLEND);
		glUseProgram(0);
		
		
		glUseProgram(drawShader);
		glBindFramebuffer(GL_FRAMEBUFFER, parFbo);
		glBindVertexArray(triVertArray);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(glGetUniformLocation(drawShader, "parallelTex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	else{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cout << "create scatter texture"<<endl;
		glUseProgram(tempScatterShader);
		glBindVertexArray(tempArray);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//Draw lines tell opengl how many values will be sent to the shaders
		glDrawArrays(GL_POINTS, 0, data.size()*(1.0f/10.0f));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);	
		glDisable(GL_BLEND);
		glUseProgram(0);

		glEnable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo3);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cout << "create scatter texture"<<endl;
		glUseProgram(tempScatterShader);
		glBindVertexArray(tempArray2);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//Draw lines tell opengl how many values will be sent to the shaders
		glDrawArrays(GL_POINTS, 0, data.size()*(1.0f/10.0f));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);	
		glDisable(GL_BLEND);
		glUseProgram(0);
		
		glUseProgram(drawScatterShader);
		glBindFramebuffer(GL_FRAMEBUFFER, scatFbo);
		glBindVertexArray(triVertArray);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex2);
		glUniform1i(glGetUniformLocation(drawScatterShader, "scatterTex"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex3);
		glUniform1i(glGetUniformLocation(drawScatterShader, "scatterTex2"), 1);	

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glUseProgram(0);

	}	

	
	if(plot == PARALLEL){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(0);
	
		int i = 0, row = 0, arrayRow = H-1, col = 0, xpos, ypos;

		while(i<W*H){
			if(texture[i] > maxValue){
				maxValue = texture[i];
				maxPos = i;
				xpos = col; 
				ypos = row; 
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

		glUseProgram(drawShader);
		glUniform1f(glGetUniformLocation(drawShader, "maxValue"), maxValue);
		glUseProgram(0);
	
		cout << "DONE WITH TEXTURE. Max value: "<<maxValue << "pos: "<<xpos << " : " <<ypos<<endl;
	}	

	if(plot == SCATTER){
		float* texture2 = new float[sW*sH];

		glActiveTexture(GL_TEXTURE1);
	
		glBindTexture(GL_TEXTURE_2D, tex2);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture2);
		glBindTexture(GL_TEXTURE_2D, 0);
	
		int i = 0, row = sH-1, col = 0, scatterMax=0, xpos, ypos, row2=0;
		int count = 0;

		while(i<sW*sH){
			if(texture2[i] > scatterMax){
				scatterMax = texture2[i];
				xpos = col; 
				ypos = row;
			}
			if(texture2[i] != 0)
				count+=texture2[i];
			scatterTex[col][row] = texture2[i];
			col++;
			i++;

			if(col == sW){
				col = 0;
				row--;
				row2++;
			}
		}

		cout << "maximum value in scatterplot: "<< scatterMax<< " pos: "<<xpos << " : "<<ypos << endl;
		cout << "counter: "<<count<<endl;

	}
}

void draw(){
	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if(plot == PARALLEL){
		glUseProgram(drawTexShader);
		glBindVertexArray(triVertArray2);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, parTex);
		glUniform1i(glGetUniformLocation(drawTexShader, "tex"), 0);
		glUniform4f(glGetUniformLocation(drawTexShader, "color"), 1.0, 0, 0.0, 1);
		glUniform1i(glGetUniformLocation(drawTexShader, "backgroundcolor"), background);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindTexture(GL_TEXTURE_2D, 0); 
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);	
		//Don't draw using the parallel coordinates shader anymore.
		glUseProgram(0);
	}
	else if(plot == SCATTER){		

		glUseProgram(drawTexShader);
		glBindVertexArray(triVertArray2);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scatTex);
		glUniform1i(glGetUniformLocation(drawTexShader, "tex"), 0);
		glUniform4f(glGetUniformLocation(drawTexShader, "color"), 0, 1, 0, 1);
		glUniform1i(glGetUniformLocation(drawTexShader, "backgroundcolor"), background);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindTexture(GL_TEXTURE_2D, 0); 
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);	
		//Don't draw using the parallel coordinates shader anymore.
		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);		

		glUseProgram(mouseShader);
		glBindVertexArray(scatterAxisArray);
		glEnableVertexAttribArray(0);
		//Set line width of the axes
		
		glLineWidth(3.0f);
		glUniform4f(glGetUniformLocation(mouseShader, "color"), 1.0f, 1.0f, 1.0f, 0.3f);
		glDrawArrays(GL_LINE_STRIP, 0, 3);
		
		//Set regular line width	
		glLineWidth(1.0f);

		glUseProgram(0);
		
		glDisable(GL_BLEND);

	}
    
	


	glUseProgram(mouseShader);
	//Recalculate mouse marker area
	
	calcMouseSquare();
	glBindVertexArray(mouseArray);
	glBindBuffer(GL_ARRAY_BUFFER, mouseBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouseVerts), mouseVerts, GL_STATIC_DRAW);
	glBindVertexArray(mouseArray);
	//Enable or disable a generic vertex attribute array
	glEnableVertexAttribArray(0);

	if(!hoover){
		glUniform4f(glGetUniformLocation(mouseShader, "color"), 1.0f, 0.0f, 0.0f, 1.0f);
		}

	else if(hoover){
		glUniform4f(glGetUniformLocation(mouseShader, "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	}

	glDrawArrays(GL_LINE_LOOP, 0, 4);
	//Bind data array containing coordinates for drawing lines between
	glBindVertexArray(0);
	
	if(!hoover){	
		glBindVertexArray(mouseArray);
		glBindBuffer(GL_ARRAY_BUFFER, mouseBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mouse2Verts), mouse2Verts, GL_STATIC_DRAW);
		glBindVertexArray(mouseArray);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glUniform4f(glGetUniformLocation(mouseShader, "color"), 0.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		//Bind data array containing coordinates for drawing lines between
		glBindVertexArray(0);
	}
	
	//Enable or disable a generic vertex attribute array
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	//Swaps the buffers of the current window if double buffered(draw)
	//glErrorCheck();
	

	glutSwapBuffers();

}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new frame
void idle()
{
	glutPostRedisplay();
}

//Interaction function for clicking mouse button
void mouseEvent(int event, int state, int x, int y){	

	float vol = 0.0;

	if(event == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	
		mouseClick = true;

		if(plot == PARALLEL){
			cout << markerSize<< endl;
			vol = calcGaussVolume_Parallel(x, y, markerSize, texArray)/maxValue *10.0;
		}
		
		else if(plot == SCATTER){
			vol = calcGaussVolume_Scatter(x, y, markerSize, scatterTex)/maxValue *1500.0;
		}

		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound(vol);
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
	
	if(event == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		mouse2Click = true;

		if(plot == PARALLEL){
			vol = calcGaussVolume_Parallel(x, y, markerSize, texArray)/maxValue *10.0;
		}
		
		else if(plot == SCATTER){
			vol = calcGaussVolume_Scatter(x, y, markerSize, scatterTex)/maxValue *1500.0;
		}		

		cout<<"On position x:"<<x<<" y: "<<y << "volume is "<<vol << " marker size: "<<markerSize<<endl;
		playSound2(vol);	
		mouse2X = x;
		mouse2Y = y;
		glutPostRedisplay();
	}
}

//Interaction function for clicking mouse button
//Here it would do to calculate the vertices only when clicking mouse and not in every frame. 
void mouseMoveClick(int x, int y){
		if(hoover){
		mouseX = x;
		mouseY = y;

		if(plot == PARALLEL){
			playSound((calcGaussVolume_Parallel(x, y, markerSize, texArray)/maxValue)*10);
		}
		
		else if(plot == SCATTER){
			playSound((calcGaussVolume_Scatter(x, y, markerSize, scatterTex)/maxValue)*1500);
		}

		glutPostRedisplay();
		}
}

//Interaction function for moving mouse marker
void mouseMove(int x, int y){	
	float pxlValue;
	if(plot == PARALLEL)
		pxlValue = texArray[x][y];
	else
		pxlValue = scatterTex[x][y];

	if(!mouseClick){
		mouseX = x;
		mouseY = y;		
		std::string s;
		s = "x: " + std::to_string(x) + " y: " + to_string(y) + " value " + to_string(pxlValue);
		char const *pchar = s.c_str();
		glutSetWindowTitle(pchar);
		glutPostRedisplay();
	}
		
	if(!mouse2Click){
		mouse2X = x;
		mouse2Y = y;		
		std::string s;
		s = "x: " + std::to_string(x) + " y: " + to_string(y) + " value " + to_string(pxlValue);
		char const *pchar = s.c_str();
		glutSetWindowTitle(pchar);
		glutPostRedisplay();
	}	
}

void keyPressed(unsigned char key, int x, int y){	
	
	float vol1 = 0.0;
	float vol2 = 0.0;
	
	if(key == '+' && markerSize <= 21){
		cout<<"marker size increased"<<endl;
		markerSize+=2;


		if(plot == PARALLEL){
			vol1 = calcGaussVolume_Parallel(mouseX, mouseY, markerSize, texArray)/maxValue *10.0;
			vol2 = calcGaussVolume_Parallel(mouse2X, mouse2Y, markerSize, texArray)/maxValue *10.0;
		}
		
		else if(plot == SCATTER){
			vol1 = calcGaussVolume_Scatter(mouseX, mouseY, markerSize, scatterTex)/maxValue *1500.0;
			vol2 = calcGaussVolume_Scatter(mouse2X, mouse2Y, markerSize, scatterTex)/maxValue *1500.0;
		}

		playSound(vol1);
		playSound2(vol2);
	}
	
	else	if(key == '-' && markerSize >=3){
		cout<<"marker size decreased"<<endl;
		markerSize-=2;

		if(plot == PARALLEL){
		vol1 = calcGaussVolume_Parallel(mouseX, mouseY, markerSize, texArray)/maxValue *10.0;
		vol2 = calcGaussVolume_Parallel(mouse2X, mouse2Y, markerSize, texArray)/maxValue *10.0;
		}
		
		else if(plot == SCATTER){
			vol1 = calcGaussVolume_Scatter(mouseX, mouseY, markerSize, scatterTex)/maxValue *1500.0;
			vol2 = calcGaussVolume_Scatter(mouse2X, mouse2Y, markerSize, scatterTex)/maxValue *1500.0;
		}

		playSound(vol1);
		playSound2(vol2);
	}
	
	else if(key == 'x'){
		yPressed = false;
		xPressed = true;
	}
	
	else if(key == 'y'){
		xPressed = false;
		yPressed = true;
	}
	else if(isdigit(key) && plot == SCATTER){
			int axis = key - '0';
			if(axis > 0 && axis <=dimX){
				cout << "chose axis"<<axis<<endl;
				if(xPressed){
					scatterAxisX = axis;
					cout << "chosen value is: "<< axis << endl;
					glViewport(0,0,sW,sH);
					tempArray = changeScatterPlot(scatterAxisX,scatterAxisY, &data.front(), sizeof(GL_FLOAT)*data.size(), tempScatterShader);
					initTexture(fbo2, tex2);

				}else if(yPressed){
					scatterAxisY = axis;
					cout << "chosen value is: "<< axis << endl;
					glViewport(0,0,sW,sH);
					tempArray = changeScatterPlot(scatterAxisX,scatterAxisY, &data.front(), sizeof(GL_FLOAT)*data.size(), tempScatterShader);
					initTexture(fbo2, tex2);
				}
			}
	}
	
	else if(key == 'h'){	

		if(hoover == true){
			hoover = false;
		}
		
		else{
			hoover = true;
			playSound2(0);
		}
	}
	else if(key == 'b'){
		background = (background == BLACK) ? WHITE : BLACK;
	}
	
}

void fKeyPressed(int key, int x, int y){
	switch(key){
		case GLUT_KEY_F1:
			plot = PARALLEL;
			glutReshapeWindow(W, H);
		break;
		case GLUT_KEY_F2:
			plot = SCATTER;
			glutReshapeWindow(sW, sH);
		break;
	}
}

void reshape(int width, int height){
	glViewport(0,0,(GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}
void init(int W, int H){

	//Sets the initial display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	
	//glutInitWindowSize(W,H);
	glutInitWindowSize(W,H);

	//Set version to be used
	glutInitContextVersion(3, 3);

	glutCreateWindow("Do you wanna roll in my 64?!");
	
	//Call to the drawing function
  glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	//Initiate stuff for the drawing
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(mouseEvent);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMoveClick);
	
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(fKeyPressed);
	
	//read data set into data array
	readFile_pCoords();
	normalizeAxis();

	markerSize = 5.0f;	

	drawShader = loadShaders("./shaders/draw.vert", "./shaders/draw.frag");
	parallelShader = loadShaders("./shaders/paralell.vert", "./shaders/paralell.frag");
	drawScatterShader = loadShaders("./shaders/drawScatter.vert", "./shaders/drawScatter.frag");
	tempScatterShader = loadShaders("./shaders/tempScatter.vert", "./shaders/tempScatter.frag");
	mouseShader = loadShaders("./shaders/mouse.vert", "./shaders/mouse.frag");
	drawTexShader = loadShaders("./shaders/texDraw.vert", "./shaders/texDraw.frag");
	
	//Create fbos and textures 
	triVertArray = createVertArray(triVerts, sizeof(triVerts), drawShader);
	scatterAxisArray = createVertArray(scatterAxisPoints, sizeof(scatterAxisPoints), mouseShader);
	triVertArray2 = createVertArray(triVerts, sizeof(triVerts), drawTexShader);
	dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
	
	 
	createMouseMarker(mouseArray, mouseBuffer, mouseVerts, sizeof(mouseVerts),  mouseShader);

	tempArray = changeScatterPlot(1,2, &data.front(), sizeof(GL_FLOAT)*data.size(), tempScatterShader);
	tempArray2 = changeScatterPlot(3,4, &data.front(), sizeof(GL_FLOAT)*data.size(), tempScatterShader);

	//Create parallel coordinates texture
	plot = PARALLEL;
	tex = createTexture(W,H,0);
	fbo = createFbo(tex);
	parTex = createTexture(W,H,0);
	parFbo = createFbo(parTex); 
	initTexture(fbo, tex);
	

	//Create scatterplot texture
	plot = SCATTER;
	glViewport(0,0,sW,sH);
	tex2 = createTexture(sW,sH, 2);
	fbo2 = createFbo(tex2);
	scatTex = createTexture(sW,sH, 3);
	scatFbo = createFbo(scatTex);
	tex3 = createTexture(sW,sH, 2);
	fbo3 = createFbo(tex3);
	initTexture(fbo2, tex2);

	//initTexture(fbo3, tex3);
	
	//Set initial display to parallel coordinates plot
	plot = PARALLEL;
	glViewport(0,0,W,H);

	glErrorCheck();
}

int main(int argc, char **argv){	
	
	alutInit(&argc, argv);

	initSound();

	//initiate glut
	glutInit(&argc, argv);
	init(W, H);
	// Loop required by OpenGL
  glutMainLoop();
	exit(0);
}
