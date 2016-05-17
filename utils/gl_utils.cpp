#define GL_GLEXT_PROTOTYPES
#include "gl_utils.h"
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>


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

GLuint changeScatterPlot(int x, int y, GLfloat *data, int size, GLuint shader){

	GLuint tempArray, tempBuffer;
	glGenVertexArrays(1, &tempArray);
	glBindVertexArray(tempArray);
	glGenBuffers(1, &tempBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tempBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionX"),1, GL_FLOAT, GL_FALSE, 10*sizeof(GLfloat),(GLvoid*)((2*x-1)*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_PositionY"),1, GL_FLOAT, GL_FALSE, 10*sizeof(GLfloat), (GLvoid*)((2*y-1)*sizeof(GLfloat)));
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
