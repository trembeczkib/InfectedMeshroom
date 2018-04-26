#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <vector>
#include "geometry.h"
#include "mesh.h"


bool AllPointsOnPlane(const std::vector<point> p_vector) {
	return 1;
}


int main() {
	
	//init test points


	std::vector<point> Points;
	//Points.push_back(point(-15, -8.660254038, 3));
	//Points.push_back(point(+15, -8.660254038, 3));
	//Points.push_back(point(0, 17.32050808, 3));
	//Points.push_back(point(128, 128, 3));
	//Points.push_back(point(127, 129, 3));
	//Points.push_back(point(127, 127, 3));
	//Points.push_back(point(129, 127, 3));
	//Points.push_back(point(129, 127, 3));
	//Points.push_back(point(130, 127, 3));
	//Points.push_back(point(130, 130, 3));


	//init test objects
	std::vector<segment> s;
	//s.push_back(segment(0, 1));
	//s.push_back(segment(1, 2));
	//s.push_back(segment(2, 0));
	s.push_back(segment(0, 1));
	s.push_back(segment(1, 2));
	s.push_back(segment(2, 3));
	s.push_back(segment(3, 0));

	


	front f = front(s);
	mesh Mesh = mesh(Points, f, 16, 256, 4);
	
//	Mesh.initilaizeOctree(16, 256, 2);
//	std::cout << atan2(-10, 10) * 180 / M_PI;
//	Mesh.splitSegmentsInFront(10, 1.5);
//	std::cout << Mesh.get_shortestSegment();
//	point ideal = Mesh.get_idealPoint_2d(0);

	point origo = point(0, 0, 3);
	point test1 = point(5, 5, 3);
	point test2 = point(-10, 0, 3);
	point test3 = point(-7, +7, 3);
	point test4 = point(30, 21, 3);
	point test5 = point(-40, -15, 3);

	int	inside0 = Mesh.nw_wn_pointInsideFront(test5);
	bool inside1 = Mesh.nw_angle_pointInsideFront(test5);
	bool inside2 = Mesh.last_resort_pointInsideFront(test5);
	bool inside3 = Mesh.cn_PnPoly_2d(point(test5));

	//std::vector<int> neighbours1 = Mesh.nearby_points_leaf(point(129, 129, 3), 10);
	//std::vector<int> neighbours2 = Mesh.nearby_points_nearby(point(129, 129, 3), 10);


	std::getchar();
	return 0;
}
