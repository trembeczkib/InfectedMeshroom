#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include "geometry.h"

class front {
	private:
		std::vector<segment> edges;

	public:
		front();
		front(std::vector<segment> front_vector);
		std::vector<segment> get_front() const;
		void push_edge(segment new_edge);
		void divide_edge(double length, int pos);		
		bool isInsideFront(const point &p);
		point findIdealPoint(const int pos);
		std::vector<segment> get_edges();
		friend std::ostream& operator << (std::ostream & os, const front & f);
};

class mesh {
	private:
		front f;
		std::vector<triangle> t;
		std::vector<point> p;
		octree p_octree;
	public:
		mesh(std::vector<point> points, front f , double m_r, double r, double m_s);
		void set_front(front front);
		void set_triangle(std::vector<triangle> triangles);
		front get_front();
		octree get_octree();
		std::vector<triangle>&  get_triangles();
		std::vector<point>& get_points();
		void sortFrontByLength();
		void advanceFront(double segment_length, double search_radius);
		bool length_sort(segment s1, segment s2);
		void initilaizeOctree(double m_r, double r, double m_s);
		void splitSegmentsInFront(double length, double min);
};


#endif
