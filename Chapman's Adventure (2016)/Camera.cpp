#include "Camera.h"

Camera::Camera()
{
	//0,0 and 20, 15.
	topLeftCorner.x = 0;
	topLeftCorner.y = 0;
	bottomRightCorner.x = 20;
	bottomRightCorner.y = 15;
}

bool Camera::inViewPort(int row, int column)
{
	if (row >= topLeftCorner.x && row <= bottomRightCorner.x)
	{
		if (column >= topLeftCorner.y && column <= bottomRightCorner.y)
		{
			return true;
		}
	}
	return false;
}

void Camera::autoPos()
{
	//0,0 and 20, 15.
	topLeftCorner.x = 0;
	topLeftCorner.y = 0;
	bottomRightCorner.x = 20;
	bottomRightCorner.y = 15;
}

sf::Vector2i Camera::getTopViewCorner()
{
	return topLeftCorner;
}

sf::Vector2i Camera::getBottomViewCorner()
{
	return bottomRightCorner;
}

void Camera::shiftLeft()
{
	if (topLeftCorner.x > 0)
	{
		//Shifts Left if applicable.
		topLeftCorner.x = topLeftCorner.x - 1;
		bottomRightCorner.x = bottomRightCorner.x - 1;
	}
}

void Camera::shiftRight()
{
	//Shifts Right if Possible.
	if (bottomRightCorner.x < 80)
	{
		topLeftCorner.x = topLeftCorner.x + 1;
		bottomRightCorner.x = bottomRightCorner.x + 1;
	}
}

void Camera::shiftUp()
{
	//Shifts up if applicable.
	if (topLeftCorner.y > 0)
	{
		topLeftCorner.y = topLeftCorner.y - 1;
		bottomRightCorner.y = bottomRightCorner.y - 1;
	}
}

void Camera::shiftDown()
{
	//Shifts Down if possible.
	if (bottomRightCorner.y < 60)
	{
		topLeftCorner.y = topLeftCorner.y + 1;
		bottomRightCorner.y = bottomRightCorner.y + 1;
	}
}