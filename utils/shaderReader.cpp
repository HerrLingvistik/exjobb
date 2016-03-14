#define GL_GLEXT_PROTOTYPES
#include "shaderReader.h"
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>



string readFile(const char* filename){

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
	string vertSource = readFile(vertexShader);
	string fragSource = readFile(fragmentShader);

	//Create handles for the shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Transform the string into something useful for the shadersource.
	const GLchar *source = (const GLchar *)vertSource.c_str();
	//Set source code for and compile shaders
	glShaderSource(vertShader, 1, &source, NULL);
	glCompileShader(vertShader);
	
	//Test if the compilation could be performed
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

	//Same process for the fragment shader
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

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program; 

}
