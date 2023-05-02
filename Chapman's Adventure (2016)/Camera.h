#pragma once
#include <SFML/Graphics.hpp>
class Camera
{
public:
	Camera();//Initializes viewport 0,0 and 20, 15.
	void autoPos();//Moves viewport 0,0 and 20, 15.
	void shiftLeft();//If applicable, moves viewport 1 unit to the left.
	void shiftRight();//If applicable, moves viewport 1 unit to the right.
	void shiftUp();//If applicable, moves viewport 1 unit up.
	void shiftDown();//If applicable moves viewport 1 unit down.
	bool inViewPort(int row, int column);//Checks if part of map is currently in view.
	sf::Vector2i getTopViewCorner();//Returns top left corner.
	sf::Vector2i getBottomViewCorner();//Returns bottom right corner.
private:
	sf::Vector2i topLeftCorner;
	sf::Vector2i bottomRightCorner;
};