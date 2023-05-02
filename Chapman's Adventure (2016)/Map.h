#pragma once
#include<SFML/Graphics.hpp>

class Map
{
public:
	Map();
	void setType(int row, int column, int value);
	int getType(int row, int column);
private:
	int tileMap[80][60];
};