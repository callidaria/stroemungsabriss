#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class ShaderI
{
public:
	ShaderI() { }
	void compile(unsigned int ibo)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
		std::cout<<"\ncompiling inst shader\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"reading instance vertex shader\n";
		std::string vertexSrc = read("shader/vertex_inst.shader");
		const char* vertexSource = vertexSrc.c_str();

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"reading instance fragment shader\n";
		std::string fragmentSrc = read("shader/fragment_inst.shader");
		const char* fragmentSource = fragmentSrc.c_str();

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"\ncompiling instance shaders\n";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "instance vertex shader error:\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			std::cout << buffer << "\n";
		} else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "instance vertex shader compiled successfully\n";
		}

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "instance fragment shader error:\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			std::cout << buffer << "\n";
		} else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << "instance fragment shader compiled successfully\n" << std::endl;
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

		int offsetAttrib = glGetAttribLocation(shaderProgram, "offset");
		glEnableVertexAttribArray(offsetAttrib);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,
				2*sizeof(float),0);
		glVertexAttribDivisor(offsetAttrib,1);

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
