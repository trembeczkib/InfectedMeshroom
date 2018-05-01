#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>
#include <cmath>
#include <SFML\Graphics.hpp>


class point {
	private:
		double x, y, z;
	public:
		point();
		point(double value_x, double value_y, double value_z);
		point(double value_x, double value_y);
		void set_x(double value);
		void set_y(double value);
		void set_z(double value);
		double get_x() const;
		double get_y() const;
		double get_z() const;
		double get_distance(point other);
		friend std::ostream& operator << (std::ostream& os, const point& p);
		void draw(sf::Color c, int boundary, sf::RenderWindow& w);
		bool operator == (const point& other);
};

class triangle {
private:
	int a, b, c;
public:
	triangle(int p1, int p2, int p3);
	void set_a(int p);
	void set_b(int p);
	void set_c(int p);
	int get_a();
	int get_b();
	int get_c();
	void draw(sf::Color c, sf::Color e, int boundary, sf::RenderWindow& w, std::vector<point>& p);
	friend std::ostream& operator << (std::ostream& os, const triangle& t);
};

class segment {
	private: 
		int initial_point, terminal_point;
	public:
		segment();
		segment(int initial, int terminal);
		void set_terminal(int new_terminal);
		void set_initial(int new_initial);
		int get_initial() const;
		int get_terminal() const;
		double get_length(std::vector<point> &points) const;
		double get_angle_2d(std::vector<point> &points) const;
		friend std::ostream& operator << (std::ostream& os, const segment& p);
		point intersection(segment other, std::vector<point>& points);
		bool isIntersected(triangle t, std::vector<point>& points);
		bool isIntersected(segment other, std::vector<point>& points);
		void draw(sf::Color c_line, sf::Color c_point, int boundary, sf::RenderWindow& w, std::vector<point>& p);
//		friend bool length_sort(const segment s1, const segment s2, std::vector<point> points);
		bool isEqual(const segment& other);
};



class octree {
	private:
		std::vector<int> points; //points belonging to this node
		std::vector<octree*> children = std::vector<octree*>(8);
		octree* parent;
		double min_range; //minimal size of enclosed region (stop splitting the space)
		int max_size; //maximal number of points on node before we stop splitting the space
		point origo;
		double range;
	public:
		octree();
		octree(const octree &other);
		octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values, octree* parent);
		void insert_point(const int p, std::vector<point>& p_values);
		void delete_point(const int p, std::vector<point>& p_values);
		int getContainerChild(const int p, std::vector<point>& p_values);
		int getContainerChild(const point pos);
		std::vector<int> get_points();
		std::vector<octree*> get_children();
		bool isLeaf();
		int get_size();
		octree* get_parent();
		double get_range();


};


#endif