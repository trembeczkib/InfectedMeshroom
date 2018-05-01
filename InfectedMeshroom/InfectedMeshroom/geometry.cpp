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
	z = 3;
}

point::point(double value_x, double value_y, double value_z) {
	x = value_x;
	y = value_y;
	z = value_z;
}

point::point(double value_x, double value_y) {
	x = value_x;
	y = value_y;
	z = 3;
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

bool point::operator == (const point& other) {
	return ((abs(this->get_x() - other.get_x()) < 0.001 ) && (abs((this->get_y() - other.get_y())) < 0.001) && (abs(this->get_z() - other.get_z()) < 0.001));
}

double point::get_distance(point other) {
	return sqrt(pow(other.get_x() - get_x(), 2) + pow(other.get_y() - get_y(), 2) + pow(other.get_z() - get_z(), 2));
}

void point::draw(sf::Color c, int boundary, sf::RenderWindow& w) {
	sf::CircleShape Point(1, 4);
	Point.setFillColor(c);
	Point.setPosition(get_x() + boundary / 2, -get_y() + boundary / 2);
	Point.setOrigin(1,1);
	w.draw(Point);
}

//////////////////////////////////////////////////////////


bool segment::isEqual(const segment& other) {
	return (this->get_initial() == other.get_initial() && this->get_terminal() == other.get_terminal());
}

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

//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
point segment::intersection(segment other, std::vector<point>& points) {
	//"this" vector
	segment this_segment = *this;
	segment other_segment = other;

	if ((abs(points[this->get_terminal()].get_x() - points[other.get_terminal()].get_x()) < 0.001) && (abs(points[this->get_terminal()].get_y() - points[other.get_terminal()].get_y()) < 0.001))
		this_segment.set_terminal(other_segment.get_terminal());
	if ((abs(points[this->get_initial()].get_x() - points[other.get_initial()].get_x()) < 0.001) && (abs(points[this->get_initial()].get_y() - points[other.get_initial()].get_y()) < 0.001))
		this_segment.set_initial(other_segment.get_initial());
	if ((abs(points[this->get_initial()].get_x() - points[other.get_terminal()].get_x()) < 0.001) && (abs(points[this->get_initial()].get_y() - points[other.get_terminal()].get_y()) < 0.01))
		this_segment.set_initial(other_segment.get_terminal());
	if ((abs(points[this->get_terminal()].get_x() - points[other.get_initial()].get_x()) < 0.001) && (abs(points[this->get_terminal()].get_y() - points[other.get_initial()].get_y()) < 0.001))
		this_segment.set_terminal(other_segment.get_initial());

	double s1x = points[this_segment.get_terminal()].get_x() - points[this_segment.get_initial()].get_x();
	double s1y = points[this_segment.get_terminal()].get_y() - points[this_segment.get_initial()].get_y();
	//"other" vector
	double s2x = points[other_segment.get_terminal()].get_x() - points[other_segment.get_initial()].get_x();
	double s2y = points[other_segment.get_terminal()].get_y() - points[other_segment.get_initial()].get_y();
	//coordinate geometry magic
	double s = (-s1y * (points[this_segment.get_initial()].get_x() - points[other_segment.get_initial()].get_x()) + s1x * (points[this_segment.get_initial()].get_y() - points[other_segment.get_initial()].get_y())) / (-s2x * s1y + s1x * s2y);
	double t = ( s2x * (points[this_segment.get_initial()].get_y() - points[other_segment.get_initial()].get_y()) - s2y * (points[this_segment.get_initial()].get_x() - points[other_segment.get_initial()].get_x())) / (-s2x * s1y + s1x * s2y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		double ix = points[this_segment.get_initial()].get_x() + (t * s1x);
		double iy = points[this_segment.get_initial()].get_y() + (t * s1y);
		return point(ix, iy);
	}

	return point(999999,999999,999999);
}

bool segment::isIntersected(segment other, std::vector<point>& points) {
	point intersection = this->intersection(other, points);
	//if z == 3 would be enough
	return !(intersection.get_x() == 999999 && intersection.get_y() == 999999 && intersection.get_z() == 999999);
}

bool segment::isIntersected(triangle t, std::vector<point>& points) {
	segment c = segment(t.get_a(), t.get_b());
	segment a = segment(t.get_b(), t.get_c());
	segment b = segment(t.get_c(), t.get_a());
	return this->isIntersected(a, points) || this->isIntersected(b, points) || this->isIntersected(c, points);
}

void segment::draw(sf::Color c_line, sf::Color c_point, int boundary, sf::RenderWindow& w, std::vector<point>& p) {
	point initial = p[initial_point];
	point terminal = p[terminal_point];
	sf::Vertex segment [2] = {
		sf::Vertex(sf::Vector2f(initial.get_x() + boundary / 2, -initial.get_y() + boundary / 2), c_line),
		sf::Vertex(sf::Vector2f(terminal.get_x() + boundary / 2, -terminal.get_y() + boundary / 2), c_line)
	};


	w.draw(segment, 2, sf::Lines);
	initial.draw(c_point, boundary, w);
	terminal.draw(c_point, boundary, w);
}


///////////////////////////////////////////////////////////////////////////

	triangle::triangle(int p1, int p2, int p3) {
		a = p1;
		b = p2;
		c = p3;
	}

	void triangle::set_a(int p) {
		a = p;
	}

	void triangle::set_b(int p) {
		b = p;
	}

	void triangle::set_c(int p) {
		c = p;
	}

	int triangle::get_a() {
		return a;
	}

	int triangle::get_b() {
		return b;
	}

	int triangle::get_c() {
		return c;
	}

	void triangle::draw(sf::Color c,sf::Color e, int boundary, sf::RenderWindow& w, std::vector<point>& p) {
		point A = p[this->a];
		point B = p[this->b];
		point C = p[this->c];
		sf::ConvexShape triangle;
		triangle.setPointCount(3);
		triangle.setPoint(0, sf::Vector2f(A.get_x() + boundary / 2, -A.get_y() + boundary / 2));
		triangle.setPoint(1, sf::Vector2f(B.get_x() + boundary / 2, -B.get_y() + boundary / 2));
		triangle.setPoint(2, sf::Vector2f(C.get_x() + boundary / 2, -C.get_y() + boundary / 2));
		triangle.setFillColor(c);
		triangle.setOutlineThickness(0.2);
		triangle.setOutlineColor(e);
		w.draw(triangle);
	}


///////////////////////////////////////////////////////////////////////////


	octree::octree() {
		points = std::vector<int>();
		min_range = 1;
		max_size = 1;
		range = 0;
		origo = point();
		parent = NULL;
	}
	
	octree::octree(const octree &other) {
		points = other.points;
		min_range = other.min_range;
		max_size = other.max_size;
		origo = other.origo;
		range = other.range;
		parent = other.parent;
		for (int i = 0; i < 8; i++) {
			children[i] = other.children[i];
		}
	}

	octree::octree(const point o, std::vector<int> p, double m_range , double r, int m_size, std::vector<point>& p_values, octree* parent) {
		points = p;
		min_range = m_range;
		range = r;
		max_size = m_size;
		origo = o;
		children;
		this->parent = parent;
		if (min_range <= range/2) {
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
					children[0] = new octree(o0, oct0, min_range, range / 2, max_size, p_values,this);
				}
			
				//oct1
				if (oct1.size() != 0) {
					point o1 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[1] = new octree(o1, oct1, min_range, range / 2, max_size, p_values,this);
				}
				

				//oct2
				if (oct2.size() != 0) {
					point o2 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[2] = new octree(o2, oct2, min_range, range / 2, max_size, p_values,this);
				}
				

				//oct3
				if (oct3.size() != 0) {
					point o3 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[3] = new octree(o3, oct3, min_range, range / 2, max_size, p_values,this);
				}
			

				//oct4
				if (oct4.size() != 0) {
					point o4 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[4] = new octree(o4, oct4, min_range, range / 2, max_size, p_values,this);
				}


				//oct5
				if (oct5.size() != 0) {
					point o5 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[5] = new octree(o5, oct5, min_range, range / 2, max_size, p_values,this);
				}

				//oct6
				if (oct6.size() != 0) {
					point o6 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[6] = new octree(o6, oct6, min_range, range / 2, max_size, p_values,this);
				}


				//oct7
				if (oct7.size() != 0) {
					point o7 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[7] = new octree(o7, oct7, min_range, range / 2, max_size, p_values,this);
				}
	

				points = final;

			}
		}
	}


	bool octree::isLeaf() {
		
		return (children[0] == NULL && children[1] == NULL && children[2] == NULL && children[3] == NULL && children[4] == NULL && children[5] == NULL && children[6] == NULL && children[7] == NULL);
	}

	octree* octree::get_parent() {
		return parent;
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
				if (min_range <= range/2) {
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
					children[0] = new octree(o0, oct1, min_range, range / 2, max_size, p_values,this);
				}

				//oct1
				if (oct1.size() != 0) {
					point o1 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[1] = new octree(o1, oct1, min_range, range / 2, max_size, p_values,this);
				}
				

				//oct2
				if (oct2.size() != 0) {
					point o2 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[2] = new octree(o2, oct2, min_range, range / 2, max_size, p_values,this);
				}
				

				//oct3
				if (oct3.size() != 0) {
					point o3 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[3] = new octree(o3, oct3, min_range, range / 2, max_size, p_values,this);
				}
			

				//oct4
				if (oct4.size() != 0) {
					point o4 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[4] = new octree(o4, oct4, min_range, range / 2, max_size, p_values,this);
				}


				//oct5
				if (oct5.size() != 0) {
					point o5 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[5] = new octree(o5, oct5, min_range, range / 2, max_size, p_values,this);
				}

				//oct6
				if (oct6.size() != 0) {
					point o6 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[6] = new octree(o6, oct6, min_range, range / 2, max_size, p_values,this);
				}


				//oct7
				if (oct7.size() != 0) {
					point o7 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[7] = new octree(o7, oct7, min_range, range / 2, max_size, p_values,this);
				}

					points = final;
				}
			}
			else
				for (int i = 0; i < 8; i++) children[i] = NULL;
		}
		else {
			int oct = getContainerChild(p, p_values);
			if (children[oct] == NULL) {
				std::vector<int> empty;
				point o = origo;
				if (oct == 0) {
					point o0 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[0] = new octree(o0, empty, min_range, range / 2, max_size, p_values,this);
				}

				//oct1
				if (oct == 1) {
					point o1 = point(o.get_x() - range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[1] = new octree(o1, empty, min_range, range / 2, max_size, p_values,this);
				}


				//oct2
				if (oct == 2) {
					point o2 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[2] = new octree(o2, empty, min_range, range / 2, max_size, p_values,this);
				}


				//oct3
				if (oct == 3) {
					point o3 = point(o.get_x() - range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[3] = new octree(o3, empty, min_range, range / 2, max_size, p_values, this);
				}


				//oct4
				if (oct == 4) {
					point o4 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() - range / 2);
					children[4] = new octree(o4, empty, min_range, range / 2, max_size, p_values, this);
				}


				//oct5
				if (oct == 5) {
					point o5 = point(o.get_x() + range / 2, o.get_y() - range / 2, o.get_z() + range / 2);
					children[5] = new octree(o5, empty, min_range, range / 2, max_size, p_values, this);
				}

				//oct6
				if (oct == 6) {
					point o6 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() - range / 2);
					children[6] = new octree(o6, empty, min_range, range / 2, max_size, p_values, this);
				}


				//oct7
				if (oct == 7) {
					point o7 = point(o.get_x() + range / 2, o.get_y() + range / 2, o.get_z() + range / 2);
					children[7] = new octree(o7, empty, min_range, range / 2, max_size, p_values, this);
				}
			}
			children[oct]->insert_point(p, p_values);			
		}
	}

	void octree::delete_point(const int p, std::vector<point>& p_values) {
		std::vector<int> result;
		octree* parent = NULL;
		octree* node = this;
		while (!node->isLeaf()) {
			node = node->get_children()[node->getContainerChild(p_values[p])];
			parent = node->get_parent();
		}

		for (auto it = node->points.begin(); it != node->points.end(); ++it) {
			if (*it == p) {
			node->points.erase(it);
			break;
			}
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

	double octree::get_range() {
		return range;
	}


