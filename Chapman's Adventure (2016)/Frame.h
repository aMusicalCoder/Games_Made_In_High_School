#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Camera.h"
#include "Data.h"
#include <iostream>
#include <string>

class Frame
{
public:
	Frame();//Constructor. Creates Window. 800 x 600 Resolution, by default.
	Frame(int width, int height);//Creates a window with a custom resolution.
	Frame(int width, int height, std::string title);//Creates a window with a custom resolution and a custom title.
	void render();//Display Current Buffer.
	void renderMap(Map &m, Camera &c, Data &d);//Render a map with a camera.
	void addChild(sf::Sprite &child);//Add Child to myChildren array for buffer cycle.
	void addChild(sf::Texture &child);//Add child to myChildren array for buffer cycle.
	void addChild(sf::Sprite &child, int x, int y);//add child with pos.
	void addChild(sf::Texture &child, int x, int y);//add child with pos.
	void removeChildren();//Removes all children
	void drawChildren();//Cycle through array and draw it to the undrawn buffer.
	int getEvent();//Returns number for whichever event happened.
	void clear();//Clears Current Screen. Used for Buffer Cycling.
	void clear(sf::Color &color);//If you need to clear to a certain color template for drawing.
	bool isClosed();//Checks if a close event has been detected. Returns true if so.
	void close();//Closes window. Destroys attached resources too.
private:
	sf::RenderWindow myWindow;
	sf::Sprite myChildren[1500];
	int myChildrenPos;
};