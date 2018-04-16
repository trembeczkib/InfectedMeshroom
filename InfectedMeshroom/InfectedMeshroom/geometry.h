#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>

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
		int get_inital();
		int get_terminal();
		friend std::ostream& operator << (std::ostream& os, const segment& p);
};

class triangle {
	private:
		point& a, b, c;
	public:
		triangle();
		triangle(point& p1, point& p2, point& p3);
		void set_a(point& p);
		void set_b(point& p);
		void set_c(point& p);
		point& get_a();
		point& get_b();
		point& get_c();
		friend std::ostream& operator << (std::ostream& os, const triangle& t);


};

class octree {
	private:
		std::vector<int> points; //points belonging to this node
		octree *children[8]; //array of children
		double min_range; //minimal size of enclosed region (stop splitting the space)
		int min_size; //minimal number of points on node (stop splitting the space)
		point origo;
		double range;
	public:
		octree();
		octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values);
		void insert_point(const int p);
		int getContainerChild(const point p);
		bool isLeaf();

};

#endif