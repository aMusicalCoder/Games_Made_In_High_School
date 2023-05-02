#include "Frame.h"

Frame::Frame()
{
	//Default Constructor.
	myWindow.create(sf::VideoMode(800, 600), "Chapman's Adventure! Go Collect Them All!");
	myChildrenPos = 0;
}

Frame::Frame(int width, int height)
{
	//Custom Resolution.
	myWindow.create(sf::VideoMode(width, height), "Chapman's Adventure! Go Collect Them All!");
	myChildrenPos = 0;
}

Frame::Frame(int width, int height, std::string title)
{
	//Custom Resolution and Custom Title.
	myWindow.create(sf::VideoMode(width, height), title);
	myChildrenPos = 0;
}

void Frame::render()
{
	//Renders current images in the buffer.
	myWindow.display();
}

void Frame::renderMap(Map &m, Camera &c, Data &d)
{
	removeChildren();
	
	for (int a = 0; a < 20; a++)
	{
		for (int b = 0; b < 15; b++)
		{
			int value = m.getType(a, b);
			int cornerX = c.getTopViewCorner().x;
			int cornerY = c.getTopViewCorner().y;

			if (value == 0)
			{
				sf::Sprite tile;
				tile.setTexture(*d.tile0);
				tile.setPosition(cornerX + (40 * a), cornerY + (40 * b));
				addChild(tile);
			}
			else if (value == 1)
			{
				sf::Sprite tile;
				tile.setTexture(*d.tile1);
				tile.setPosition(cornerX + (40 * a), cornerY + (40 * b));
				addChild(tile);
			}
			else if (value == 2)
			{
				sf::Sprite tile;
				tile.setTexture(*d.tile2);
				tile.setPosition(cornerX + (40 * a), cornerY + (40 * b));
				addChild(tile);
			}
			else if (value == 3)
			{
				sf::Sprite tile;
				tile.setTexture(*d.tile3);
				tile.setPosition(cornerX + (40 * a), cornerY + (40 * b));
				addChild(tile);
			}
		}
	}
}

void Frame::addChild(sf::Sprite &child)
{
	myChildren[myChildrenPos] = child;
	myChildrenPos++;
}

void Frame::addChild(sf::Texture &child)
{
	myChildren[myChildrenPos].setTexture(child);
	myChildrenPos++;
}

void Frame::addChild(sf::Texture &child, int x, int y)
{
	myChildren[myChildrenPos].setTexture(child);
	myChildren[myChildrenPos].setPosition(x, y);
	myChildrenPos++;
}

void Frame::addChild(sf::Sprite &child, int x, int y)
{
	myChildren[myChildrenPos] = child;
	myChildren[myChildrenPos].setPosition(x, y);
	myChildrenPos++;
}

void Frame::drawChildren()
{
	//Filter through all children and add to buffer. 
	for (int i = 0; i < myChildrenPos; i++)
	{
		if (myChildrenPos == 0)
			break;
		myWindow.draw(myChildren[i]);
	}
}

void Frame::removeChildren()
{
	//Set Array Manager to 0, so anything added will overwrite previous stuff.
	myChildrenPos = 0;
}

void Frame::clear()
{
	//Clear to default. Black.
	myWindow.clear();
}

void Frame::clear(sf::Color &color)
{
	//Use for buffer cycling with a specific background color.
	myWindow.clear(color);
}

bool Frame::isClosed()
{
	//Returns true if a close window event has been detected. Meant to be run in a loop with this as a condition.
	sf::Event e;
	
	if (myWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			return true;
	}

	return false;
}

int Frame::getEvent()
{
	sf::Event e;
	
	if (myWindow.pollEvent(e))
	{
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Left)
			{
				return 1;
			}
			else if (e.key.code == sf::Keyboard::Right)
			{
				return 2;
			}
			else if (e.key.code == sf::Keyboard::Up)
			{
				return 3;
			}
			else if (e.key.code == sf::Keyboard::Down)
			{
				return 4;
			}
			else if (e.key.code == sf::Keyboard::Z)
			{
				return 5;
			}
			else if (e.key.code == sf::Keyboard::X)
			{
				return 6;
			}
			else if (e.key.code == sf::Keyboard::Space)
			{
				return 7;
			}
			else if (e.key.code == sf::Keyboard::Return)
			{
				return 8;
			}
		}
	}

	return 0;//0 means no event.
}

void Frame::close()
{
	//Closes RenderWindow object.
	myWindow.close();
}
