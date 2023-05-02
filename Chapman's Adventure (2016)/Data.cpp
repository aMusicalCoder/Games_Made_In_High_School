#include "Data.h"

Data::Data()
{
	tile0 = new sf::Texture();
	tile1 = new sf::Texture();
	tile2 = new sf::Texture();
	tile3 = new sf::Texture();
	quickChar = new sf::Texture();

	tile0->loadFromFile("C:/Game Development/Chapman\'s Adventure/Chapman\'s Adventure/assets/grassTile.png");
	tile1->loadFromFile("C:/Game Development/Chapman\'s Adventure/Chapman\'s Adventure/assets/rockyGrassTile.png");
	tile2->loadFromFile("C:/Game Development/Chapman\'s Adventure/Chapman\'s Adventure/assets/grassTile.png");
	tile3->loadFromFile("C:/Game Development/Chapman\'s Adventure/Chapman\'s Adventure/assets/grassTile.png");
	quickChar->loadFromFile("C:/Game Development/Chapman\'s Adventure/Chapman\'s Adventure/assets/quickChar.png");
	//Load Images.
}

Data::Data(const Data& d)
{
	tile0 = new sf::Texture(*d.tile0);
	tile1 = new sf::Texture(*d.tile1);
	tile2 = new sf::Texture(*d.tile2);
	tile3 = new sf::Texture(*d.tile3);
	quickChar = new sf::Texture(*d.quickChar);
}

Data & Data::operator=(const Data& d)
{
	tile0 = new sf::Texture(*d.tile0);
	tile1 = new sf::Texture(*d.tile1);
	tile2 = new sf::Texture(*d.tile2);
	tile3 = new sf::Texture(*d.tile3);
	quickChar = new sf::Texture(*d.quickChar);
	return *this;
}

Data::~Data()
{
	//Destructor.
	delete tile0;
	delete tile1;
	delete tile2;
	delete tile3;
	delete quickChar;
	quickChar = 0;
	tile0 = 0;
	tile1 = 0;
	tile2 = 0;
	tile3 = 0;
}