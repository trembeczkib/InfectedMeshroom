#include <iostream>
#include <vector>
#include "geometry.h"

std::ostream& operator << (std::ostream& os, const point& p) {
	os << "P(" << p.get_x() << "," << p.get_y() << "," << p.get_z() << ")";
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
