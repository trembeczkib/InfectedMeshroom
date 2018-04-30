#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "mesh.h"
#include <SFML\Graphics.hpp>


bool AllPointsOnPlane(const std::vector<point> p_vector) {
	return 1;
}

//https://github.com/SFML/SFML/wiki/Source:-Zoom-View-At-%28specified-pixel%29
void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}



int main() {

	int boundary = 1024;
	sf::RenderWindow window(sf::VideoMode(boundary, boundary), "Boundary box");
	const float zoomAmount{ 1.1f };
	window.clear(sf::Color::White);
	sf::Vertex axis_x[2] = {
		sf::Vertex(sf::Vector2f(0,boundary / 2), sf::Color::Black),
		sf::Vertex(sf::Vector2f(boundary, boundary / 2), sf::Color::Black)
	};
	sf::Vertex axis_y[2] = {
		sf::Vertex(sf::Vector2f(boundary / 2, 0), sf::Color::Black),
		sf::Vertex(sf::Vector2f(boundary / 2, boundary), sf::Color::Black)
	};


	std::vector<segment> Segments;
	std::vector<point> Points;
	Points.push_back(point(2*-15, 2*-8.660254038, 3));
	Points.push_back(point(2*+15, 2*-8.660254038, 3));
	Points.push_back(point(0, 2*17.32050808, 3));
	Segments.push_back(segment(0, 1));
	Segments.push_back(segment(1, 2));
	Segments.push_back(segment(2, 0));



	//Points.push_back(point(-10, -10, 3));
	//Points.push_back(point(+10, -10, 3));
	//Points.push_back(point(+10, +10, 3));
	//Points.push_back(point(-10, +10, 3));


	//Segments.push_back(segment(0, 1));
	//Segments.push_back(segment(1, 2));
	//Segments.push_back(segment(2, 3));
	//Segments.push_back(segment(3, 0));

	front Front = front(Segments);
	mesh Mesh = mesh(Points, Front, boundary / 4, boundary / 2, 8);
	Mesh.splitSegmentsInFront(10, 1.5);
	Mesh.advanceFront(1, 20);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / zoomAmount));
				else if (event.mouseWheelScroll.delta < 0)
					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, zoomAmount);
			}
			if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(sf::Color::White);
			//t.draw(sf::Color::Green, boundary, window, Points);
			//Points[0].draw(sf::Color(128, 60, 162), boundary, window);
			//Points[1].draw(sf::Color(128, 60, 162), boundary, window);
			//Points[2].draw(sf::Color(128, 60, 162), boundary, window);
			//Segments[0].draw(sf::Color::Blue, sf::Color(128, 60, 162), boundary, window, Points);
			//Segments[1].draw(sf::Color::Blue, sf::Color(128, 60, 162), boundary, window, Points);
			//Segments[2].draw(sf::Color::Blue, sf::Color(128, 60, 162), boundary, window, Points);
			Mesh.draw(sf::Color(206, 229, 237), sf::Color::Green, sf::Color::Blue, sf::Color::Red, sf::Color(128, 60, 162), boundary, window);
			window.draw(axis_x, 2, sf::Lines);
			window.draw(axis_y, 2, sf::Lines);
			window.display();
	
	}


	std::cout << "byeWorld!";
//	std::getchar();
	return 0;
}
