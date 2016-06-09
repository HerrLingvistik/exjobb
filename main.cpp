
/* ---------------------------------------------------------------------------
** Audial Support for Visual Dense Data Display, 
** Master of Science in Media Technology, Linköping University
** Spring 2016
** Authors: Gustav Hallström, Tobias Erlandsson
** -------------------------------------------------------------------------*/

/*
	THINGS TO ADD 	 
	- fixa användartester!
	- Kom på frågor!
	- Utför rigorösa tester!
	- Återupprätta Hallströms heder - sänk familjen Wallenberg
	- Städa kod
	- Fixa så ljudet används med hjälp av soundgains-arrayen
*/

#define GL_GLEXT_PROTOTYPES	
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include "utils/common.h"
#include "utils/dataHandler.h"
#include "utils/gl_utils.h"
#include "utils/normalizeAxis.h"
#include "utils/sound.h"
#include "utils/usertest.h"

using namespace std;
//Shader handles
GLuint drawShader, parallelShader, mouseShader, tempScatterShader, drawScatterShader, drawTexShader;
//Pointers for vertices
GLuint triVertArray, triVertArray2, triVertBuffer, triVertBuffer2, scatterAxisArray, paraAxisArray, dataArray, dataArray2, mouseBuffer, mouseArray, clusterArray1, clusterArray2;
//Texture and fbo handles
GLuint parTex1, parFbo1, parTex2, parFbo2, finalParTex, finalParFbo,  finalScatTex, finalScatFbo, scatTex1, scatTex2, scatFbo1, scatFbo2; 

int plot, scatterAxisX = 1, scatterAxisY = 2, background = 0, clusterCounter1=1, clusterCounter2=1, dimX = 0, dimY = 0, maxPos = 0, mouseX, mouseY, mouse2X, mouse2Y, taskNumber=1, subTask = 1, numberOfRuns=0;

const static int PARALLEL = 0, SCATTER = 1, BLACK = 0, WHITE = 1;
bool bPressed = false, rPressed = false, hoover = false, soundactive = true, DRAWRED=true, DRAWBLUE=true, USERTEST=false; 

float parallelTex[W][H];
float parallelTex2[W][H];
float scatterTex[sW][sH];
float scatterTex2[sW][sH];
float* texture = new float[ W*H ];

vector<float> data, parData2, data2, data3;
vector<int> first;
vector<int> count;

string clusterFileB;
string clusterFileR;
string paraFile1;
string paraFile2;
string resultString;

//Booleans used for deciding when to play the sounds
bool mouseClick = false, mouse2Click = false;
 
float maxValue=0, maxValue2 = 0, markerSize, scatterMax1, scatterMax2;	

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
	-0.91f, 0.9f,
	//Bottom left point
	-0.91f, -0.91f,
	//Bottom right point
	0.9f, -0.91f
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
/*
float soundGains[6] = 
{
	0.00f, 0.10f, 0.18f, 
	0.32f, 0.56f, 1.00f 
};
*/

/*float soundGains[11] = 
{
	0.00f, 0.01f, 0.02f, 
	0.03f, 0.04f, 0.06f, 
	0.10f, 0.16f, 0.25f, 
	0.40f, 0.63f 
};*/

float soundGains[11] = 
{
	0.00f, 0.02f, 
	0.03f, 0.04f, 0.06f, 
	0.10f, 0.16f, 0.25f, 
	0.40f, 0.63f, 1.0f 
};

vector <GLfloat> paraAxes;

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

float getGains(float key){
	int pos = ceil(key * (sizeof(soundGains)/sizeof(float)-1));
	//cout << "key: "<< key  << "*" << sizeof(soundGains)/sizeof(float)-1 << " gives arraypos "<< pos << endl;
	return soundGains[pos]; 
}


//Problem med texture blending som skriver över istället för att blenda. 
//Alternativt att den blendar och sedan clampar;
void initTexture(GLuint fboTemp, GLuint texTemp){
	
	//set window color and clear last screenpixel
	
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	
	//Bind data array containing coordinates for drawing lines between
	if(plot == PARALLEL){	
		//Create texture with first parallel cluster
		glUseProgram(parallelShader);
		cout << "create parallel texture"<<endl;
		glBindVertexArray(dataArray);
		glEnableVertexAttribArray(0);
		//Draw lines tell opengl how many values will be sent to the shaders
		//Bind frambuffer to draw into texture
		glBindFramebuffer(GL_FRAMEBUFFER, parFbo1);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//use parallel coordinates shader
		glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);	
		//glDisable(GL_BLEND);
		glUseProgram(0);
		
		//Create texture with second parallel cluster
		glUseProgram(parallelShader);
		cout << "create parallel texture"<<endl;
		glBindVertexArray(dataArray2);
		glEnableVertexAttribArray(0);
		//Draw lines tell opengl how many values will be sent to the shaders
		//Bind frambuffer to draw into texture
		glBindFramebuffer(GL_FRAMEBUFFER, parFbo2);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//use parallel coordinates shader
		glMultiDrawArrays(GL_LINE_STRIP, &first.front(), &count.front(),count.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);	
		//glDisable(GL_BLEND);
		glUseProgram(0);

		createParallelArray(parallelTex, parTex1, maxValue);
		createParallelArray(parallelTex2, parTex2, maxValue2);

		glUseProgram(drawShader);
		glBindFramebuffer(GL_FRAMEBUFFER, finalParFbo);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(triVertArray);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glUniform1i(glGetUniformLocation(drawShader, "parallelTex"), 0);
		glUniform1i(glGetUniformLocation(drawShader, "parallelTex2"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, parTex1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, parTex2);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}
	else{
		glBindFramebuffer(GL_FRAMEBUFFER, scatFbo1);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cout << "create scatter texture"<<endl;
		glUseProgram(tempScatterShader);
		glUniform1i(glGetUniformLocation(tempScatterShader, "resolution"), sW);
		glBindVertexArray(clusterArray1);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//Draw lines tell opengl how many values will be sent to the shaders
		if(DRAWRED){
			glDrawArrays(GL_POINTS, 0, data2.size()/2.0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);	
		glUseProgram(0);
	
	
		glBindFramebuffer(GL_FRAMEBUFFER, scatFbo2);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cout << "create scatter texture"<<endl;
		glUseProgram(tempScatterShader);
		glUniform1f(glGetUniformLocation(tempScatterShader, "resolution"), sH);
		glBindVertexArray(clusterArray2);
		//Enable or disable a generic vertex attribute array
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//Draw lines tell opengl how many values will be sent to the shaders
		if(DRAWBLUE){
			glDrawArrays(GL_POINTS, 0, data3.size()/2.0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Disable and unbind just to be safe
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);	
		glUseProgram(0);
		
		glDisable(GL_BLEND);


		createScatterArray(scatterTex, scatTex1, scatterMax1);
		createScatterArray(scatterTex2, scatTex2, scatterMax2);

		float scatterMaxTot_Balle = (scatterMax1 > scatterMax2) ? scatterMax1 : scatterMax2;
		cout << "scatterMaxTot_Balle = " << scatterMaxTot_Balle << endl;


		glUseProgram(drawScatterShader);
		glBindFramebuffer(GL_FRAMEBUFFER, finalScatFbo);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(triVertArray);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glUniform1i(glGetUniformLocation(drawScatterShader, "scatterTex"), 0);
		glUniform1i(glGetUniformLocation(drawScatterShader, "scatterTex2"), 1);	
		glUniform1f(glGetUniformLocation(drawScatterShader, "maxValue"), scatterMaxTot_Balle);	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scatTex1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, scatTex2);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);

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
		glBindTexture(GL_TEXTURE_2D, finalParTex);
		glUniform1i(glGetUniformLocation(drawTexShader, "tex"), 0);
		if(background == WHITE){
			glUniform4f(glGetUniformLocation(drawTexShader, "color"), 64.0f/255.0f, 1.0f, 64.0f/255.0f, 1);
			glUniform4f(glGetUniformLocation(drawTexShader, "color2"), 1.0f, 64.0f/255.0f, 1.0f, 1);
		}
		else{
			glUniform4f(glGetUniformLocation(drawTexShader, "color"), 64.0f/255.0f, 1.0f, 64.0f/255.0f, 1);
			glUniform4f(glGetUniformLocation(drawTexShader, "color2"), 1.0f, 64.0f/255.0f, 1.0f, 1);
		}
		
		
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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

		glUseProgram(mouseShader);
		glBindVertexArray(paraAxisArray);
		glEnableVertexAttribArray(0);
		//Set line width of the axes
		
		glLineWidth(3.0f);
		if(background == BLACK)
			glUniform4f(glGetUniformLocation(mouseShader, "color"), 1.0f, 1.0f, 1.0f, 0.2f);
		else if(background == WHITE)
			glUniform4f(glGetUniformLocation(mouseShader, "color"), 0.0f, 0.0f, 0.0f, 0.2f);
		glDrawArrays(GL_LINES, 0, 8);
		//Set regular line width	
		glLineWidth(1.0f);

		glUseProgram(0);
		
		glDisable(GL_BLEND);

	}
	else if(plot == SCATTER){		

		glUseProgram(drawTexShader);
		glBindVertexArray(triVertArray2);
		//Enable or disable a genedata[ric vertex attribute array
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, finalScatTex);
		glUniform1i(glGetUniformLocation(drawTexShader, "tex"), 0);
		glUniform4f(glGetUniformLocation(drawTexShader, "color"), 64.0f/255.0f, 1.0f, 64.0f/255.0f, 1);
		glUniform4f(glGetUniformLocation(drawTexShader, "color2"), 1.0f, 64.0f/255.0f, 1.0f, 1);
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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

		glUseProgram(mouseShader);
		glBindVertexArray(scatterAxisArray);
		glEnableVertexAttribArray(0);
		//Set line width of the axes
		
		glLineWidth(3.0f);
		if(background == BLACK)
			glUniform4f(glGetUniformLocation(mouseShader, "color"), 1.0f, 1.0f, 1.0f, 0.3f);
		else if(background == WHITE)
			glUniform4f(glGetUniformLocation(mouseShader, "color"), 0.0f, 0.0f, 0.0f, 0.3f);
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

	int newX = x, newY = y;
	if(event == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	
		float vol1;		
		mouseClick = true;
		if(plot == PARALLEL){
			mouseX = x;
			mouseY = y;
			movePosParallel(newX, newY, markerSize, parallelTex);
			vol1 = getGains((calcGaussVolume_Parallel(newX, newY, markerSize, parallelTex)/maxValue));
		}else if(plot == SCATTER){
			mouseX = x;
			mouseY = y;
			movePosScatter(newX, newY, markerSize, scatterTex);
			vol1 = getGains((calcGaussVolume_Scatter(newX, newY, markerSize, scatterTex)/scatterMax1));
		}
		mouseX = x;
		mouseY = y;
		if(soundactive)
			playSound(vol1);
	}
	
	if(event == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		float vol2;
		mouse2Click = true;
		if(plot == PARALLEL){
			newX=x;
			newY=y;
			movePosParallel(newX, newY, markerSize, parallelTex2);
			vol2 = getGains((calcGaussVolume_Parallel(newX, newY, markerSize, parallelTex2)/maxValue2));
		}
		else if(plot == SCATTER){
			newX=x;
			newY=y;
			movePosScatter(newX, newY, markerSize, scatterTex2);
			vol2 = getGains((calcGaussVolume_Scatter(newX, newY, markerSize, scatterTex2)/scatterMax2));
		}			
		mouse2X = x;
		mouse2Y = y;
		if(soundactive)
			playSound2(vol2);
	}
}

//Interaction function for clicking mouse button
//Here it would do to calculate the vertices only when clicking mouse and not in every frame. 
//void mouseMoveClick(int x, int y){
void mouseMove(int x, int y){
	int newX = x, newY = y;
	//cout << to_string(x) << " " << to_string(y) << endl;
	if(hoover){
		//if(soundactive){
			if(plot == PARALLEL && x>=0  && x<W && y>=0 && y<H){
				mouseX = x;
				mouseY = y;
				movePosParallel(newX, newY, markerSize, parallelTex);
				float vol1 = getGains((calcGaussVolume_Parallel(newX, newY, markerSize, parallelTex)/maxValue));

				std::string s;
				s = " pos " + std::to_string(newX) + " y "  + to_string(newY) + " vol: " + to_string(parallelTex[newX][newY]);			
				char const *pchar = s.c_str();
				glutSetWindowTitle(pchar);

				newX=x;
				newY=y;
				movePosParallel(newX, newY, markerSize, parallelTex2);
				float vol2 = getGains((calcGaussVolume_Parallel(newX, newY, markerSize, parallelTex2)/maxValue2));
				if(soundactive){
					playSound(vol1);
					playSound2(vol2);
				}
				

			}
			else if(plot == SCATTER && x>=0  && x<sW && y>=0 && y<sH){
				mouseX = x;
				mouseY = y;
				movePosScatter(newX, newY, markerSize, scatterTex);
				
				cout << "playing sound percentage "<<(calcGaussVolume_Scatter(newX, newY, markerSize, scatterTex)/scatterMax1)<<endl;
	
				float vol1 = getGains((calcGaussVolume_Scatter(newX, newY, markerSize, scatterTex)/scatterMax1));
				//cout << "moved ("<<to_string(x)<<","<<to_string(y)<<") giving "<< scatterTex[x][y] <<"to ("<<newX<<","<<newY<<")giving "<< scatterTex[newX][newY] <<endl;
				newX=x;
				newY=y;
				movePosScatter(newX, newY, markerSize, scatterTex2);
				float vol2 = getGains((calcGaussVolume_Scatter(newX, newY, markerSize, scatterTex2)/scatterMax2));

				cout <<endl;

				if(soundactive){
					playSound(vol1);
					playSound2(vol2);
				}

				std::string s;
				s = " pos " + std::to_string(newX) + " y "  + to_string(newY) + " vol: " + to_string(vol1);			
				char const *pchar = s.c_str();
				//glutSetWindowTitle(pchar);
			}
		//}
	}
		

	

}

void keyPressed(unsigned char key, int x, int y){	
	
	
	if(key == 'b'){
		rPressed = false;
		bPressed = true;
	}
	
	else if(key == 'r'){
		bPressed = false;
		rPressed = true;
	}
	else if(isdigit(key) && plot == SCATTER){
		int cluster = key - '0';
		//string str(key);	
		if(bPressed){
			if(cluster > 0 && cluster <= 8){
				DRAWBLUE = true;
				clusterFileB[14] = key;

				glViewport(0,0,sW,sH);
				readFile_cluster(data2, clusterFileB, clusterCounter1);
				readFile_cluster(data3, clusterFileR, clusterCounter2);
				normalizeAxis2(data2, data3);
				clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
				clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
				initTexture(scatFbo1, scatTex1);
				bPressed = false;
			}else if( cluster == 0){
				DRAWBLUE = false;
				initTexture(scatFbo1, scatTex1);
			}
		}else if(rPressed){
			if(cluster > 0 && cluster <= 8){
				DRAWRED = true;
				clusterFileR[14] = key;

				glViewport(0,0,sW,sH);	
				readFile_cluster(data2, clusterFileB, clusterCounter1);
				readFile_cluster(data3, clusterFileR, clusterCounter2);
				normalizeAxis2(data2, data3);
				clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
				clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
				initTexture(scatFbo2, scatTex2);
				rPressed = false;
			}else if( cluster == 0){
				DRAWRED = false;
				initTexture(scatFbo2, scatTex2);
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
	}else if(key == 13){
		//Gör tre tester variera texturer som presenteras i dessa!
		if(USERTEST){
			switch(taskNumber){
				//högst densitet i första klustret
				//räkna ut hur nära högsta densitet
				//byt texturer to_string
				//om 10 test är körda öka tasknumber
				//Parallella strutar
				//OBS GÖR BARA MED LJUD NU FIXA UTAN LJUD SEN!!
				int newX, newY, newX2, newY2;
				case 1:
					//glutSetWindowTitle("Task 1");
					
					newX = mouseX;
					newY = mouseY;
					movePosParallel(newX, newY, markerSize, parallelTex);
					resultString += "maxValue is "+ to_string(maxValue) + " chose value ";					
					task1(resultString, newX, newY, parallelTex, maxValue);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						resultString += "\n2:\n";
						glutSetWindowTitle("Running test - Task 2 - Sample that purple stuff(kush)");
						playSound(0);
						playSound2(0);
						paraFile1 = "./data/parallel/3dpara" + to_string(subTask) + ".txt";
						readFile_pCoords(data, paraFile1, paraAxes);
						paraFile2 = "./data/parallel/3dpara" + to_string(subTask+1) + ".txt";
						//paraFile2[22] = '0'+(subTask+1);
						cout << "switching to "<<paraFile1 << " and "<<paraFile2<<endl;
						readFile_pCoords(parData2, paraFile2, paraAxes);
						normalizeAxis(data, parData2);
						dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
						dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
						initTexture(scatFbo1, scatTex1);
						break;
					}

					subTask+=2;
					//paraFile1[22] = '0'+subTask;	
					paraFile1 = "./data/parallel/3dpara" + to_string(subTask) + ".txt";
					readFile_pCoords(data, paraFile1, paraAxes);
					paraFile2 = "./data/parallel/3dpara" + to_string(subTask+1) + ".txt";
					//paraFile2[22] = '0'+(subTask+1);
					cout << "switching to "<<paraFile1 << " and "<<paraFile2<<endl;
					readFile_pCoords(parData2, paraFile2, paraAxes);
					normalizeAxis(data, parData2);
					dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
					dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
					initTexture(scatFbo1, scatTex1);
					
					
				break;
				//högst densitet i andra klustret
				//gör samma som innan fast med andra klustret
				case 2:
					//glutSetWindowTitle("Task 2");

					newX = mouseX;
					newY = mouseY;
					movePosParallel(newX, newY, markerSize, parallelTex2);
					resultString += "maxValue is "+ to_string(maxValue2) + " chose value ";					
					task1(resultString, newX, newY, parallelTex2, maxValue2);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						resultString += "\n3:\n";
						glutSetWindowTitle("Running test - Task 3 - Mix and compare green vs purple(chronic kush)"); 
						hoover = false;
						playSound(0);
						playSound2(0);
						paraFile1 = "./data/parallel/3dpara" + to_string(subTask) + ".txt";
						readFile_pCoords(data, paraFile1, paraAxes);
						paraFile2 = "./data/parallel/3dpara" + to_string(subTask+1) + ".txt";
						//paraFile2[22] = '0'+(subTask+1);
						cout << "switching to "<<paraFile1 << " and "<<paraFile2<<endl;
						readFile_pCoords(parData2, paraFile2, paraAxes);
						normalizeAxis(data, parData2);
						dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
						dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
						initTexture(scatFbo1, scatTex1);
						break;
					}

					subTask+=2;
					//paraFile1[22] = '0'+subTask;	
					paraFile1 = "./data/parallel/3dpara" + to_string(subTask) + ".txt";
					readFile_pCoords(data, paraFile1, paraAxes);
					paraFile2 = "./data/parallel/3dpara" + to_string(subTask+1) + ".txt";
					//paraFile2[22] = '0'+(subTask+1);
					cout << "switching to "<<paraFile1 << " and "<<paraFile2<<endl;
					readFile_pCoords(parData2, paraFile2, paraAxes);
					normalizeAxis(data, parData2);
					dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
					dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
					initTexture(scatFbo1, scatTex1);
				break;
				//Hitta 50/50
				//samma som innan eventuellt andra plots
				//byt till parallella koordinater
				case 3:
					newX = mouseX;
					newY = mouseY;
					newX2 = mouse2X;
					newY2 = mouse2Y;
					movePosParallel(newX, newY, markerSize, parallelTex);
					movePosParallel(newX2, newY2, markerSize, parallelTex2);					
					task3(resultString, newX, newY, newX2, newY2, parallelTex, parallelTex2);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						resultString += "\n4:\n";
						glutSetWindowTitle("Running test - Task 4 - Find max of green (chronic)"); 
						hoover=true;
						playSound(0);
						playSound2(0);
						plot = SCATTER;
						glutReshapeWindow(sW, sH);
						glViewport(0,0,sW,sH);
						clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
						readFile_cluster(data2, clusterFileB, clusterCounter1);
						clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
						readFile_cluster(data3, clusterFileR, clusterCounter2);
						normalizeAxis2(data2, data3);
						clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
						clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
						initTexture(scatFbo1, scatTex1);
						break;
					}

					subTask+=2;
					//paraFile1[22] = '0'+subTask;	
					paraFile1 = "./data/parallel/3dpara" + to_string(subTask) + ".txt";
					readFile_pCoords(data, paraFile1, paraAxes);
					paraFile2 = "./data/parallel/3dpara" + to_string(subTask+1) + ".txt";
					//paraFile2[22] = '0'+(subTask+1);
					cout << "switching to "<<paraFile1 << " and "<<paraFile2<<endl;
					readFile_pCoords(parData2, paraFile2, paraAxes);
					normalizeAxis(data, parData2);
					dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
					dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
					initTexture(scatFbo1, scatTex1);
				break;

				case 4:
					newX = mouseX;
					newY = mouseY;
					movePosScatter(newX, newY, markerSize, scatterTex);				
					task4(resultString, newX, newY, scatterTex, scatterMax1);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						resultString += "\n5:\n";
						glutSetWindowTitle("Running test - Task 5 - Find max of green(kush)"); 
						playSound(0);
						playSound2(0);
						clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
						readFile_cluster(data2, clusterFileB, clusterCounter1);
						clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
						cout <<" file for purple "<<clusterFileR <<endl;
						readFile_cluster(data3, clusterFileR, clusterCounter2);
						normalizeAxis2(data2, data3);
						clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
						clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
						initTexture(scatFbo1, scatTex1);
						break;
					}

					subTask+=2;	
					clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
					readFile_cluster(data2, clusterFileB, clusterCounter1);
					clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
					cout <<" file for purple "<<clusterFileR <<endl;
					readFile_cluster(data3, clusterFileR, clusterCounter2);
					normalizeAxis2(data2, data3);
					clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
					clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
					initTexture(scatFbo1, scatTex1);
				break;

				case 5:
					newX = mouseX;
					newY = mouseY;
					movePosScatter(newX, newY, markerSize, scatterTex2);				
					task5(resultString, newX, newY, scatterTex2, scatterMax2);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						resultString += "\n6:\n";
						glutSetWindowTitle("Running test - Task 6 - fiddy/fiddy(kush)"); 
						playSound(0);
						playSound2(0);
						hoover=false;
						clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
						readFile_cluster(data2, clusterFileB, clusterCounter1);
						clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
						cout <<" file for purple "<<clusterFileR <<endl;
						readFile_cluster(data3, clusterFileR, clusterCounter2);
						normalizeAxis2(data2, data3);
						clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
						clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
						initTexture(scatFbo1, scatTex1);
						break;
					}

					subTask+=2;	
					clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
					readFile_cluster(data2, clusterFileB, clusterCounter1);
					clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
					cout <<" file for purple "<<clusterFileR <<endl;
					readFile_cluster(data3, clusterFileR, clusterCounter2);
					normalizeAxis2(data2, data3);
					clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
					clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
					initTexture(scatFbo1, scatTex1);
				break;
		
				case 6:
					newX = mouseX;
					newY = mouseY;
					newX2 = mouse2X;
					newY2 = mouse2Y;
					movePosScatter(newX, newY, markerSize, scatterTex);	
					movePosScatter(newX2, newY2, markerSize, scatterTex2);				
					task6(resultString, newX, newY, newX2, newY2, scatterTex, scatterTex2);

					if(subTask == 9){
						taskNumber++;
						subTask = 1;
						//resultString += "\n4:\n";
						//glutSetWindowTitle("Running test - Task 4 - Find max of purple(kush)"); 
						playSound(0);
						playSound2(0);
						numberOfRuns++;
						if(numberOfRuns == 2){
							writeResultFile(resultString);
							exit(0);
						}else{
							resultString += "\n\n sound off 1: \n";
							glutSetWindowTitle("Running test - Task 1 - Sample that green stuff(chronic)");
							hoover = true;
							plot = PARALLEL;

							glViewport(0,0,W,H);
							//glutReshapeWindow(W, H);
							paraFile1 = "./data/parallel/3dpara1.txt";
							paraFile2 = "./data/parallel/3dpara2.txt";
							readFile_pCoords(data, paraFile1, paraAxes);
							readFile_pCoords(parData2, paraFile2, paraAxes);

							normalizeAxis(data, parData2);
							dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
							dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
							initTexture(scatFbo1, scatTex1);
							glutReshapeWindow(W, H);
							USERTEST = true;
							taskNumber = 1;
							subTask = 1;
							soundactive=false;
						}
						//exit(0);
						break;
					}

					subTask+=2;	
					clusterFileB = "./data/scatter/cluster" + to_string(subTask) + ".txt";
					readFile_cluster(data2, clusterFileB, clusterCounter1);
					clusterFileR = "./data/scatter/cluster" + to_string(subTask+1) + ".txt";
					cout <<" file for purple "<<clusterFileR <<endl;
					readFile_cluster(data3, clusterFileR, clusterCounter2);
					normalizeAxis2(data2, data3);
					clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
					clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);
					initTexture(scatFbo1, scatTex1);
				break;
				//kör case 4-6 för parallella koordinater
			}
		}
	}
}

void fKeyPressed(int key, int x, int y){
	switch(key){
		case GLUT_KEY_F1:
			plot = PARALLEL;
			playSound(0);
			playSound2(0);
			glutReshapeWindow(W, H);
		break;
		case GLUT_KEY_F2:
			plot = SCATTER;
			playSound(0);
			playSound2(0);
			glutReshapeWindow(sW, sH);
		break;
		case GLUT_KEY_F3:
			background = (background == BLACK) ? WHITE : BLACK;
		break;
		case GLUT_KEY_F4:
			soundactive = (soundactive) ? false : true;
			if(!soundactive){
				playSound(0);
				playSound2(0);
			}		
			cout << "sound on? " << soundactive << endl;
		break;
		case GLUT_KEY_F12:
			//Start with parallel coordintes
			resultString = "1: \n";
			glutSetWindowTitle("Running test - Task 1 - Sample that green stuff(chronic)");
			hoover = true;
			plot = PARALLEL;

			glViewport(0,0,W,H);
			//glutReshapeWindow(W, H);
			paraFile1 = "./data/parallel/3dpara1.txt";
			paraFile2 = "./data/parallel/3dpara2.txt";
			readFile_pCoords(data, paraFile1, paraAxes);
			readFile_pCoords(parData2, paraFile2, paraAxes);

			normalizeAxis(data, parData2);
			dataArray = createVertArray( &data.front(), sizeof(GL_FLOAT)*data.size(), parallelShader); 
			dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
			initTexture(scatFbo1, scatTex1);
			glutReshapeWindow(W, H);
			USERTEST = true;
			taskNumber = 1;
			subTask = 1;
			//cout << parData2.size()<<endl;
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
	//glutMotionFunc(mouseMoveClick);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(fKeyPressed);
	
	clusterFileR = "./data/scatter/cluster1.txt";
	clusterFileB = "./data/scatter/cluster2.txt";
	clusterFileR[14] = '1';
	//read data set into data array
	//readFile_pCoords(data, "./data/clusterspara5.txt", paraAxes);
	//readFile_pCoords(parData2, "./data/clusterspara3.txt", paraAxes);

	readFile_pCoords(data, "./data/parallel/3dpara1.txt", paraAxes);
	readFile_pCoords(parData2, "./data/parallel/3dpara2.txt", paraAxes);

	readFile_cluster(data2, clusterFileR, clusterCounter1);	
	readFile_cluster(data3, clusterFileB, clusterCounter2);	
	
	cout << "size of cluster 1 "<< data.size()<<endl;
	cout << "size of cluster 2 "<< parData2.size()<<endl;
	//Normalize clusters
	normalizeAxis(data, parData2);
	normalizeAxis2(data2, data3);
	//Set size of mouse marker
	//Marker size must be an uneven number	
	markerSize = 11.0f;	

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
	dataArray2 = createVertArray( &parData2.front(), sizeof(GL_FLOAT)*parData2.size(), parallelShader);
	//Create vertex arrays for the mouse markers
	createMouseMarker(mouseArray, mouseBuffer, mouseVerts, sizeof(mouseVerts),  mouseShader);
	//Create vertex arrays for the scatter plot
	clusterArray1 = changeScatterPlot(1,2, 0, &data2.front(), sizeof(GL_FLOAT)*data2.size(), tempScatterShader);
	clusterArray2 = changeScatterPlot(1,2, 0, &data3.front(), sizeof(GL_FLOAT)*data3.size(), tempScatterShader);

	paraAxisArray = createVertArray(&paraAxes.front(), sizeof(GL_FLOAT)*paraAxes.size(), mouseShader);

	//Create parallel coordinates texture
	plot = PARALLEL;
	parTex1 = createTexture(W,H,0);
	parFbo1 = createFbo(parTex1);
	parTex2 = createTexture(W,H,0);
	parFbo2 = createFbo(parTex2);
	finalParTex = createTexture2(W,H,0);
	finalParFbo = createFbo(finalParTex); 
	initTexture(parFbo1, parTex1);
	//Create scatterplot texture
	plot = SCATTER;
	glViewport(0,0,sW,sH);
	scatTex1 = createTexture(sW,sH, 0);
	scatFbo1 = createFbo(scatTex1);
	finalScatTex = createTexture2(sW,sH, 3);
	finalScatFbo = createFbo(finalScatTex);
	scatTex2 = createTexture(sW,sH, 0);
	scatFbo2 = createFbo(scatTex2);
	initTexture(scatFbo2, scatTex2);
	
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
