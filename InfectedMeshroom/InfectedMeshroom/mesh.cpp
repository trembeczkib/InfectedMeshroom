#define _USE_MATH_DEFINES
#include "geometry.h"
#include "mesh.h"


front::front(std::vector<segment> front_vector) {
	edges = front_vector;
}

std::vector<segment> front::get_front() const {
	return edges;
}

void front::push_edge(segment edge) {
	edges.push_back(edge);
}