#include "Map.h"

Map::Map()
{
	//Initialize Elements.
	for (int i = 0; i < 80; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			tileMap[i][j] = 0;
		}
	}
}

void Map::setType(int row, int column, int value)
{
	tileMap[row][column] = value;
}

int Map::getType(int row, int column)
{
	return tileMap[row][column];
}