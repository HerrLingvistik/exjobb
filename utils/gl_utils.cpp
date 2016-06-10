#define GL_GLEXT_PROTOTYPES
#include "gl_utils.h"
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <stdio.h>


void glErrorCheck()
{
    int errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        printf("Failure in OpenGL %d \n", errCode);
        //exit(0);
    }
}

string readShaderFile(const char* filename){

	ifstream infile;
	infile.open(filename,  std::ifstream::binary);
	string contents;
	string line;
	int i = 0;

	while(getline(infile, line)){
		contents.append(line + "\n");
		i++;
	}
	return contents;

}

GLuint loadShaders(const char* vertexShader, const char* fragmentShader){
	
	//Get contents of shader files
	string vertSource = readShaderFile(vertexShader);
	string fragSource = readShaderFile(fragmentShader);
	//Create handles for the shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Transform the string into something useful for the shadersource.
	const GLchar *source = (const GLchar *)vertSource.c_str();
	//Set source code and compile shaders
	glShaderSource(vertShader, 1, &source, NULL);
	glCompileShader(vertShader);
	//Test if the compilation was successfull
	GLint isCompiled = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
	
	if(isCompiled == GL_FALSE){

		cout << " vert shader not compiled"<< endl;
		GLint maxLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertShader, maxLength, &maxLength, &infoLog[0]);
		//We don't need the shader anymore.
		glDeleteShader(vertShader);

		return 0;
	}	

	//Same process for the fragment shader as for the vertex shader
	source = (const GLchar *)fragSource.c_str();
	glShaderSource(fragShader, 1, &source, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
	
	if(isCompiled == GL_FALSE){

		cout << " frag shader not compiled"<< endl;

		GLint maxLength = 0;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragShader, maxLength, &maxLength, &infoLog[0]);
	
		//We don't need the shader anymore.
		glDeleteShader(fragShader);

		return 0;
	}	
	
	//Finally create and compile the program object to be used for using the shaders.
	GLuint program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	//Link our program
	glLinkProgram(program);
	
	//Attach our shaders to our program
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	//Not needed anymore.
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	//return handle to our shaders
	return program; 

}

GLuint createVertArray(GLfloat *data, int size, GLuint shader){

	GLuint vertArray; 
	GLuint vertBuffer; 
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),2, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertArray;

}

GLuint changeScatterPlot(int x, int y, int skip, GLfloat *data, int size, GLuint shader){

	GLuint tempArray, tempBuffer;
	glGenVertexArrays(1, &tempArray);
	glBindVertexArray(tempArray);
	glGenBuffers(1, &tempBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tempBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionX"),1, GL_FLOAT, GL_FALSE, skip*sizeof(GLfloat),(GLvoid*)((2*x-1)*sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionX"),1, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat),(GLvoid*)(0*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionY"),1, GL_FLOAT, GL_FALSE, skip*sizeof(GLfloat), (GLvoid*)((2*y-1)*sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionY"),1, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)(1*sizeof(GLfloat)));
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return tempArray;
}

void createMouseMarker(GLuint &mouseArray, GLuint &mouseBuffer, GLfloat *data, int size, GLuint shader){
	
	//For rendering square around mouse pointer
	glGenVertexArrays(1, &mouseArray);
	glBindVertexArray(mouseArray);
	glGenBuffers(1, &mouseBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mouseBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),2, GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}

//Create texture and set attach it to a framebuffer object.
//https://www.opengl.org/discussion_boards/showthread.php/169270-Subset-of-blending-modes-for-32-bit-integer-renderre 
GLuint createTexture(int W1, int H1, int i){

	GLuint tex1;
	glGenTextures(1, &tex1);
	glActiveTexture(GL_TEXTURE0 + i); 
	glBindTexture(GL_TEXTURE_2D, tex1); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, W1, H1, 0, GL_RED, GL_UNSIGNED_INT, NULL);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex1;
}

GLuint createTexture2(int W1, int H1, int i){

	GLuint tex1;
	glGenTextures(1, &tex1);
	glActiveTexture(GL_TEXTURE0 + i); 
	glBindTexture(GL_TEXTURE_2D, tex1); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, W1, H1, 0, GL_RG, GL_UNSIGNED_INT, NULL);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex1;
}

GLuint createFbo(GLuint tex1){
	GLuint fbo1;
	glBindTexture(GL_TEXTURE_2D, tex1); 
	glGenFramebuffers(1, &fbo1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){

		cout<<"texture creation not successful"<<endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return fbo1;
}

void createParallelArray(float texArray[][H], GLuint tex, float &maxValue){

	maxValue = 0;
	float* texture = new float[ W*H ];
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(0);

	int i = 0, row = 0, arrayRow = H-1, col = 0, xpos, ypos;

	while(i<W*H){
		if(texture[i] > maxValue){
			maxValue = texture[i];
			xpos = col; 
			ypos = arrayRow; 
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

	cout << "maximum value in parallel plot: "<< maxValue<< " pos: "<<xpos << " : "<<ypos << endl;

}
void createScatterArray(float readTex[][sH], GLuint texIn, float &scatterMax){
	float* texture2 = new float[sW*sH];

		scatterMax=0;

		glActiveTexture(GL_TEXTURE1);
	
		glBindTexture(GL_TEXTURE_2D, texIn);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, texture2);
		glBindTexture(GL_TEXTURE_2D, 0);
	
		int i = 0, row = sH-1, col = 0, xpos=0, ypos=0;//, row2=0;
		int count = 0;

		while(i<sW*sH){
			if(texture2[i] > scatterMax){
				scatterMax = texture2[i];
				xpos = col; 
				ypos = row;
				//cout << "changed stuff "<< endl;
			}
			if(texture2[i] != 0)
				count+=texture2[i];
			readTex[col][row] = texture2[i];
			col++;
			i++;

			if(col == sW){
				col = 0;
				row--;
				//row2++;
			}
		}

		cout << "maximum value in scatterplot: "<< scatterMax<< " pos: "<<xpos << " : "<<ypos << endl;
		cout << "counter: "<<count<<endl;
}
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
GLuint loadBMP(string bmpFile){
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;
	
	FILE * file = fopen(bmpFile.c_str(),"rb");
	if (!file){printf("YOOOOOOOOOOO Image could not be opened\n"); return 0;}

	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return false;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return textureID;
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");



}
