#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include "geometry.h"

class neighbour {
	public:
		int point_pos;
		int active_neighbours;
		std::vector<int> dir; //0:in 1:out
		std::vector<int> neigh;
		neighbour(int pos, std::vector<segment>& front);
};

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
		std::vector<segment> get_edges();
		void set_edges(std::vector<segment> e);
		void add_segment(segment s);
		friend std::ostream& operator << (std::ostream & os, const front & f);
};

class mesh {
	private:
		front f;
		std::vector<triangle> t;
		std::vector<point> p;
		octree p_octree;
		std::vector<neighbour> neighbours;
	public:
		mesh(std::vector<point> points, front f , double m_r, double r, double m_s);
		void set_front(front front);
		void set_triangle(std::vector<triangle> triangles);
		front get_front();
		octree get_octree();
		std::vector<triangle>&  get_triangles();
		std::vector<point>& get_points();
		void sortFrontByLength();
		void advanceFront(double close_distance, double far_distance);
		bool length_sort(segment s1, segment s2);
		void initilaizeOctree(double m_r, double r, double m_s);
		void splitSegmentsInFront(double length, double min);
		int get_shortestSegment();
		point get_idealPoint_2d(int segment_pos);
		bool tryTriangle(segment& s, int p_pos);
		bool isIntersectingFront(segment s);
		bool isIntersectingFront(segment s, segment shortest);
		bool isInFront(segment s);
		void removeFromFront(segment s);
		void draw(sf::Color c_triangle, sf::Color c_triangle_edge, sf::Color c_front, sf::Color c_active, sf::Color c_point, int boundary, sf::RenderWindow& w);
		std::vector<int> nearby_points_nearby(const point p0, double epsilon);
		std::vector<int> nearby_points_leaf(const point p0, double epsilon);
		//pip
		int isLeft_2d(point lp1, point lp2, point p0); 
		bool nw_wn_pointInsideFront(point p);	//decode: not_working_winding_number
		bool nw_angle_pointInsideFront(point p); //not_working_angles sum
		bool last_resort_pointInsideFront(point p);
		int pip4(point p0);
		bool pip5(point p0);
		bool pip6(point p0);
		bool pip7(const point& p0);
};


#endif
