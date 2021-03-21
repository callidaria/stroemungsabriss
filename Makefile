unx : main.cpp
	g++ main.cpp ccb/src/* script/*.cpp -o yomisensei -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal

win : main.cpp
	g++ main.cpp ccb/src/* script/*.cpp -o yomisensei.exe -L"C:/MinGW/lib" -lSDL2 -lSDL2_net -lSDL2main -lglew32 -lglew32s -lopengl32 -lOpenAL32 -DGLEW_STATIC -DSDL_MAIN_HANDLED -DSTB_IMAGE_STATIC
