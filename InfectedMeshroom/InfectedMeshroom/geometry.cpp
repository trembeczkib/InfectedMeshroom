#include <iostream>
#include <vector>
#include "geometry.h"

std::ostream& operator << (std::ostream& os, const point& p) {
	os << "P(" << p.get_x() << "," << p.get_y() << "," << p.get_z() << ")";
	return os;
}

std::ostream& operator << (std::ostream& os, const segment& p) {
	return os;
}

point::point() {
	x = 0;
	y = 0;
	z = 0;
}

point::point(double value_x, double value_y, double value_z) {
	x = value_x;
	y = value_y;
	z = value_z;
}

point::point(double value_x, double value_y) {
	x = value_x;
	y = value_y;
	z = 0;
}

void point::set_x(double value) {
	x = value;
}

void point::set_y(double value) {
	y = value;
}

void point::set_z(double value) {
	z = value;
}

double point::get_x() const {
	return x;
}

double point::get_y() const {
	return y;
}

double point::get_z() const {
	return z;
}

//////////////////////////////////////////////////////////


segment::segment() {
	initial_point = 0;
	terminal_point = 0;
}

segment::segment(int initial, int terminal) {
	initial_point = initial;
	terminal_point = terminal;
}

void segment::set_terminal(int new_terminal) {
	terminal_point = new_terminal;
}

void segment::set_initial(int new_initial) {
	initial_point = new_initial;

}

int segment::get_inital() {
	return initial_point;
}


int segment::get_terminal() {
	return terminal_point;
}

///////////////////////////////////////////////////////////////////////////

/*
	std::vector<int> points; //points belonging to this node
	octree *children[8]; //array of children
	int min_space; //minimal size of enclosed region (stop splitting the space)
	int min_size; //minimal number of points on node (stop splitting the space)
	point origo;

	octree(const point o, std::vector<int> points);
	void insert_point(const int p);
	int getContainerChild(const point p);
	bool isLeaf();
*/

	octree::octree() {
		points = std::vector<int>();
		for (int i = 0; i++; i < 8) children[i] = NULL;
		min_range = 1;
		min_size = 1;
		range = 0;
		origo = point();
	}

	octree::octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values) {
		points = p;
		min_range = m_range;
		range = r;
		min_size = m_size;
		origo = o;

		if (min_range <= range) {
			if (m_size > p.size()) {
				std::vector<int> oct0;
				std::vector<int> oct1;
				std::vector<int> oct2;
				std::vector<int> oct3;
				std::vector<int> oct4;
				std::vector<int> oct5;
				std::vector<int> oct6;
				std::vector<int> oct7;
				std::vector<int> final;

				for (auto it = points.begin(); it != points.end(); ++it) {


					if (o.get_x() > p_values[*it].get_x() && o.get_y() > p_values[*it].get_y() && o.get_z() > p_values[*it].get_z()) {
						oct0.push_back(*it);
					}
					else if (o.get_x() > p_values[*it].get_x() && o.get_y() > p_values[*it].get_y() && o.get_z() < p_values[*it].get_z()) {
						oct1.push_back(*it);
					}
					else if (o.get_x() > p_values[*it].get_x() && o.get_y() < p_values[*it].get_y() && o.get_z() > p_values[*it].get_z()) {
						oct2.push_back(*it);
					}
					else if (o.get_x() > p_values[*it].get_x() && o.get_y() < p_values[*it].get_y() && o.get_z() < p_values[*it].get_z()) {
						oct3.push_back(*it);
					}
					else if (o.get_x() < p_values[*it].get_x() && o.get_y() > p_values[*it].get_y() && o.get_z() > p_values[*it].get_z()) {
						oct4.push_back(*it);
					}
					else if (o.get_x() < p_values[*it].get_x() && o.get_y() > p_values[*it].get_y() && o.get_z() < p_values[*it].get_z()) {
						oct5.push_back(*it);
					}
					else if (o.get_x() < p_values[*it].get_x() && o.get_y() < p_values[*it].get_y() && o.get_z() > p_values[*it].get_z()) {
						oct6.push_back(*it);
					}
					else if (o.get_x() < p_values[*it].get_x() && o.get_y() < p_values[*it].get_y() && o.get_z() < p_values[*it].get_z()) {
						oct7.push_back(*it);
					}
					else final.push_back(*it);
				}
				//oct0
				point o0 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
				children[0] = new octree(o0, oct0, min_range, range/2, min_size, p_values);

				//oct1
				point o1 =  point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
				children[1] = new octree(o1, oct1, min_range, range / 2, min_size, p_values);

				//oct2
				point o2 =  point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
				children[2] = new octree(o2, oct2, min_range, range / 2, min_size, p_values);

				//oct3
				point o3 =  point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
				children[3] = new octree(o3, oct3, min_range, range / 2, min_size, p_values);

				//oct4
				point o4 =  point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
				children[4] = new octree(o4, oct4, min_range, range / 2, min_size, p_values);

				//oct5
				point o5 =  point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
				children[5] = new octree(o5, oct5, min_range, range / 2, min_size, p_values);

				//oct6
				point o6 =  point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
				children[6] = new octree(o6, oct6, min_range, range / 2, min_size, p_values);

				//oct7
				point o7 =  point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
				children[7] = new octree(o7, oct7, min_range, range / 2, min_size, p_values);

				p = final;

			}
		}
		
	}
