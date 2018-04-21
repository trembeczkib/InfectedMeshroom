#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "mesh.h"


bool AllPointsOnPlane(const std::vector<point> p_vector) {
	return 1;
}

bool IsVertexInside(const point p, const front f) {
	return 1;
}

int main() {
	
	//init test points
	point origo = point(0, 0, 0);
	std::vector<point> Points;
	Points.push_back(point(-15, -8.660254038, 3));
	Points.push_back(point(+15, -8.660254038, 3));
	Points.push_back(point(0, 17.32050808, 3));

	//init test objects
	std::vector<segment> s;
	s.push_back(segment(0, 1));
	s.push_back(segment(1, 2));
	s.push_back(segment(2, 0));


	front f = front(s);
	mesh Mesh = mesh(Points, f, 16, 256, 2);
	
//	Mesh.initilaizeOctree(16, 256, 2);
//	std::cout << atan2(-10, 10) * 180 / M_PI;
	Mesh.splitSegmentsInFront(10, 1.5);

	std::getchar();
	return 0;
}
