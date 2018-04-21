#define _USE_MATH_DEFINES
#include "geometry.h"
#include "mesh.h"
#include <algorithm> 


front::front(std::vector<segment> front_vector) {
	edges = front_vector;
}

front::front() {
	edges = std::vector<segment>();
}

std::vector<segment> front::get_front() const {
	return edges;
}

void front::push_edge(segment edge) {
	edges.push_back(edge);
}

std::vector<segment> front::get_edges() {
	return edges;
}

//////////////////////////


mesh::mesh(std::vector<point> points, front front, double m_r, double r, double m_s) {
	p = points;
	f = front;
	std::vector<int> pos;
	for (int i = 0; i < p.size(); i++) pos.push_back(i);
	p_octree = octree(point(0, 0), pos, m_r, r, m_s, p);
}

void mesh::set_front(front front) {
	f = front;
}

void mesh::set_triangle(std::vector<triangle> triangles) {
	t = triangles;
}

front mesh::get_front() {
	return f;
}

std::vector<triangle>  mesh::get_triangles() {
	return t;
}

std::vector<point> mesh::get_points() {
	return p;
}

octree mesh::get_octree() {
	return p_octree;
}

void mesh::initilaizeOctree(double m_r, double r, double m_s) {
	std::vector<int> points;
	for (int i = 0; i < p.size(); i++) points.push_back(i);
	p_octree = octree(point(0, 0), points, m_r, r, m_s, p);
}

void mesh::advanceFront(double segment_length, double search_radius) {
	while (get_front().get_edges().size() > 0) {
		/*
		int shortest_pos = f.get_front().shortestPos();
		point ideal = get_front().findIdealPoint(shortest_pos);
		
		if (nearby) ideal = nearby;
		if (!isInsideFront) {
			ideal = getBetterInside();
		}
		addPoint();
		addSegments();
		addTriangle();
		removeSegment;
	*/	
	}
	
}

