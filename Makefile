all : main.cpp
	g++ main.cpp cld_lin/src/* script/*.cpp -o yomisensei -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal
