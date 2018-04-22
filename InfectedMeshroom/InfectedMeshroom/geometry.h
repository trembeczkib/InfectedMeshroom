#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>
#include <cmath>

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
		friend std::ostream& operator << (std::ostream& os, const point& p);
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
//		friend bool length_sort(const segment s1, const segment s2, std::vector<point> points);
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
	friend std::ostream& operator << (std::ostream& os, const triangle& t);


};

class octree {
	private:
		std::vector<int> points; //points belonging to this node
		std::vector<octree*> children = std::vector<octree*>(8);

		double min_range; //minimal size of enclosed region (stop splitting the space)
		int max_size; //maximal number of points on node before we stop splitting the space
		point origo;
		double range;
	public:
		octree();
		octree(const octree &other);
		octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values);
		void insert_point(const int p, std::vector<point>& p_values);
		int getContainerChild(const int p, std::vector<point>& p_values);
		int getContainerChild(const point pos);
		std::vector<int> get_points();
		std::vector<octree*> get_children();
		bool isLeaf();
		int get_size();
//		std::vector<int> get_neighbours(const point p);

};


#endif