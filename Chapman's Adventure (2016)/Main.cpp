#include "Frame.h"
#include "Camera.h"
#include "Map.h"
#include "Data.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
	Data d = Data();
	Frame window(800, 600);
	Map m = Map();
	Camera c = Camera();
	m.setType(0, 0, 1);

	while (true)
	{
		if (window.isClosed() == true)
		{
			break;
		}
		window.clear();
		window.renderMap(m, c, d);
		window.drawChildren();
		window.render();
	}
	window.close();
	return 0;
}