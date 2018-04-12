#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

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
		double get_x();
		double get_y();
		double get_z();
		friend std::ostream& operator << (std::ostream& os, const point& p);
};

class segment {
	private: 
		point& initial_point, terminal_point;
	public:
		segment();
		segment(point& initial, point& terminal);
		void set_terminal(point& new_terminal);
		void set_initial(point& new_initial);
		point& get_inital();
		point& get_terminal();
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

};
#endif