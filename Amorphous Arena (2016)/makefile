#Variables.
LNK_TAGS = -lSDL2main -lSDL2 -lSDL2_ttf
INC_DIR = -IA:\SoftwareGameDevelopmentTools\SDL\include
LNK_DIR = -LA:\SoftwareGameDevelopmentTools\SDL\lib\win32
FILES = main.cpp Game.cpp Game.h Map.cpp Map.h
all:
	g++ $(FILES) $(INC_DIR) $(LNK_DIR) $(LNK_TAGS) -o AmorphousArena.exe
clean:
	rm main.o AmorphousArena.exe
