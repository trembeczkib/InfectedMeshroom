#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include "geometry.h"
#include "mesh.h"

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

double point::get_distance(point other) {
	return sqrt(pow(other.get_x() - get_x(), 2) + pow(other.get_y() - get_y(), 2) + pow(other.get_z() - get_z(), 2));
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

int segment::get_initial() const{
	return initial_point;
}


int segment::get_terminal() const{
	return terminal_point;
}

double segment::get_length(std::vector<point> &points) const{
	point t = points[get_terminal()];
	point i = points[get_initial()];
	return sqrt(pow(t.get_x() - i.get_x(), 2) + pow(t.get_y() - i.get_y(), 2) + pow(t.get_z() - i.get_z(), 2));
}

double segment::get_angle_2d(std::vector<point> &points) const {
	
	return atan2((points[terminal_point].get_y() - points[initial_point].get_y()) , (points[terminal_point].get_x() - points[initial_point].get_x()));
}


///////////////////////////////////////////////////////////////////////////


	octree::octree() {
		points = std::vector<int>();
		min_range = 1;
		max_size = 1;
		range = 0;
		origo = point();
	}
	
	octree::octree(const octree &other) {
		points = other.points;
		min_range = other.min_range;
		max_size = other.max_size;
		origo = other.origo;
		range = other.range;

		for (int i = 0; i < 8; i++) {
			children[i] = other.children[i];
		}
	}

	octree::octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values) {
		points = p;
		min_range = m_range;
		range = r;
		max_size = m_size;
		origo = o;
		children;

		if (min_range <= range) {
			if (max_size < p.size()) {
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
				if (oct0.size() != 0) {
					point o0 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[0] = new octree(o0, oct0, min_range, range / 2, max_size, p_values);
				}
			
				//oct1
				if (oct1.size() != 0) {
					point o1 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[1] = new octree(o1, oct1, min_range, range / 2, max_size, p_values);
				}
				

				//oct2
				if (oct2.size() != 0) {
					point o2 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[2] = new octree(o2, oct2, min_range, range / 2, max_size, p_values);
				}
				

				//oct3
				if (oct3.size() != 0) {
					point o3 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[3] = new octree(o3, oct3, min_range, range / 2, max_size, p_values);
				}
			

				//oct4
				if (oct4.size() != 0) {
					point o4 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[4] = new octree(o4, oct4, min_range, range / 2, max_size, p_values);
				}


				//oct5
				if (oct5.size() != 0) {
					point o5 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[5] = new octree(o5, oct5, min_range, range / 2, max_size, p_values);
				}

				//oct6
				if (oct6.size() != 0) {
					point o6 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[6] = new octree(o6, oct6, min_range, range / 2, max_size, p_values);
				}


				//oct7
				if (oct7.size() != 0) {
					point o7 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[7] = new octree(o7, oct7, min_range, range / 2, max_size, p_values);
				}
	

				points = final;

			}
		}
	}


	bool octree::isLeaf() {
		
		return (children[0] == NULL && children[1] == NULL && children[2] == NULL && children[3] == NULL && children[4] == NULL && children[5] == NULL && children[6] == NULL && children[7] == NULL);
	}

	//tribute to brandonpelfrey
	int octree::getContainerChild(const int p, std::vector<point>& p_values) {
		point pos = p_values[p];
		int oct = 0;
		if (pos.get_x() >= origo.get_x()) oct |= 4;
		if (pos.get_y() >= origo.get_y()) oct |= 2;
		if (pos.get_z() >= origo.get_z()) oct |= 1;
		return oct;
	}

	int octree::getContainerChild(const point pos) {
		int oct = 0;
		if (pos.get_x() >= origo.get_x()) oct |= 4;
		if (pos.get_y() >= origo.get_y()) oct |= 2;
		if (pos.get_z() >= origo.get_z()) oct |= 1;
		return oct;
	}

	void octree::insert_point(const int p, std::vector<point>& p_values) {
		if (isLeaf()) {
			points.push_back(p);
			if (points.size() > max_size) {
				if (min_range <= range) {
					std::vector<int> oct0;
					std::vector<int> oct1;
					std::vector<int> oct2;
					std::vector<int> oct3;
					std::vector<int> oct4;
					std::vector<int> oct5;
					std::vector<int> oct6;
					std::vector<int> oct7;
					std::vector<int> final;
					point o = origo;

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
				if (oct0.size() != 0) {
					point o0 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[0] = new octree(o0, oct1, min_range, range / 2, max_size, p_values);
				}

				//oct1
				if (oct1.size() != 0) {
					point o1 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[1] = new octree(o1, oct1, min_range, range / 2, max_size, p_values);
				}
				

				//oct2
				if (oct2.size() != 0) {
					point o2 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[2] = new octree(o2, oct2, min_range, range / 2, max_size, p_values);
				}
				

				//oct3
				if (oct3.size() != 0) {
					point o3 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[3] = new octree(o3, oct3, min_range, range / 2, max_size, p_values);
				}
			

				//oct4
				if (oct4.size() != 0) {
					point o4 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[4] = new octree(o4, oct4, min_range, range / 2, max_size, p_values);
				}


				//oct5
				if (oct5.size() != 0) {
					point o5 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[5] = new octree(o5, oct5, min_range, range / 2, max_size, p_values);
				}

				//oct6
				if (oct6.size() != 0) {
					point o6 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[6] = new octree(o6, oct6, min_range, range / 2, max_size, p_values);
				}


				//oct7
				if (oct7.size() != 0) {
					point o7 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[7] = new octree(o7, oct7, min_range, range / 2, max_size, p_values);
				}

					points = final;
				}
			}
			else
				for (int i = 0; i < 8; i++) children[i] = NULL;
		}
		else {
			int oct = getContainerChild(p, p_values);
			children[oct]->insert_point(p, p_values);			
		}
	}

	int octree::get_size() {
		return this->points.size();
	}

	std::vector<int> octree::get_points() {
		return points;
	}

	std::vector<octree*> octree::get_children() {
		return children;
	}



