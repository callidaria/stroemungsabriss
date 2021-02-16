#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Shader2D
{
public:
	Shader2D() { }
	void compile()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
		std::cout<<"\ncreating 2D shader\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"reading 2D vertex shader\n";
		std::string vertexSrc=read("shader/vertex2d.shader");
		const char* vertexSource = vertexSrc.c_str();

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"reading 2D fragment shader\n";
		std::string fragmentSrc=read("shader/fragment2d.shader");
		const char* fragmentSource = fragmentSrc.c_str();

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"\ncompiling 2D shaders\n";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "2D vertex shader error:\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			std::cout << buffer << "\n";
		} else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "2D vertex shader compiled successfully\n";
		}

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "2D fragment shader error\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			std::cout << buffer << "\n";
		} else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "2D fragment shader compiled successfully\n" << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColour");
		glLinkProgram(shaderProgram);
		enable();

		int posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),0);

		int texAttrib = glGetAttribLocation(shaderProgram, "texCoords");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),(void*)(2*sizeof(float)));

		modelUni = glGetUniformLocation(shaderProgram, "model");
		viewUni = glGetUniformLocation(shaderProgram, "view");
		projUni = glGetUniformLocation(shaderProgram, "proj");
	}
	void enable()
	{
		glUseProgram(shaderProgram);
	}
private:
	std::string read(const char* path)
	{
		std::string ptr;std::ifstream file(path);
		if (!file.is_open()) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "no shader found\n";
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "shader loaded\n";
		}
		std::string line = ""; while (!file.eof()) {
			std::getline(file,line);
			ptr.append(line + "\n");
		} file.close();
		return ptr;
	}
private:
	unsigned int shaderProgram;
public:
	int modelUni, viewUni, projUni;
};
