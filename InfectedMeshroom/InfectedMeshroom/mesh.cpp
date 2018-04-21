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

std::vector<triangle>&  mesh::get_triangles() {
	return t;
}

std::vector<point>& mesh::get_points() {
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

	void mesh::splitSegmentsInFront(double length, double min) {
		std::vector<segment> new_front;
		const std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {

			if (it->get_length(p) > (length * min)) {
				double modulus = fmod(it->get_length(p),length);
				double egeszresz = (it->get_length(p) - modulus) / length;
				double actual_length = length + (modulus / egeszresz);
				point new_terminal;
				int new_terminal_pos;
				point new_initial;
				int new_initial_pos = it->get_initial();
				double x = p[it->get_initial()].get_x();
				double y = p[it->get_initial()].get_y();
				for (int i = 1; i < egeszresz; i++) {
					 x = x + cos(it->get_angle_2d(p)) * actual_length;
					 y = y + sin(it->get_angle_2d(p)) * actual_length;
					new_terminal = point(x,y,3);
					p.push_back(new_terminal);
					new_terminal_pos = p.size();
					new_front.push_back(segment(new_initial_pos,new_terminal_pos));
					new_initial = new_terminal;
					new_initial_pos = new_terminal_pos;
				}
				new_front.push_back(segment(new_initial_pos, it->get_terminal()));
			}
			else new_front.push_back(*it);
		}

		f = new_front;
	}



