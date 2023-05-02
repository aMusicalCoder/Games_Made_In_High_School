#include<iostream>
#include<SDL.h>
#ifndef MAP_H
#define MAP_H

class Map
{
	public:
		Map();//Constructor. Sets to default Map.
		int getMapValue(int x, int y);//Placed from top left corner to bottom right.
		void setMapValue(int x, int y, int set);//Set a value in the map.
		int myMap[24][12];
	
};

#endif