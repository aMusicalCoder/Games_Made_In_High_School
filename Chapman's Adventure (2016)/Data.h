#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Data
{
public:
	Data();//Loads Tiles for use in program. Sets up other stuff.
	Data(const Data& d);//Copy Constructor. User should not be using multiple data classes but just in case.
	Data& operator=(const Data& d);//Shouldn't be needed but just in case.
	~Data();//Destructor. Destroys shit.
	sf::Texture *tile0;
	sf::Texture *tile1;
	sf::Texture *tile2;
	sf::Texture *tile3;
	sf::Texture *quickChar;
private:
	
};