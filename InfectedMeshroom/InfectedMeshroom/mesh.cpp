#define _USE_MATH_DEFINES
#include "geometry.h"
#include "mesh.h"
#include <algorithm> 


front::front(std::vector<segment> front_vector) {
	edges = front_vector;
}

front::front() {
	edges = std::vector<segment>();
}

std::vector<segment> front::get_front() const {
	return edges;
}

void front::push_edge(segment edge) {
	edges.push_back(edge);
}

std::vector<segment> front::get_edges() {
	return edges;
}

//////////////////////////


mesh::mesh(std::vector<point> points, front front, double m_r, double r, double m_s) {
	p = points;
	f = front;
	std::vector<int> pos;
	for (int i = 0; i < p.size(); i++) pos.push_back(i);
	p_octree = octree(point(0, 0), pos, m_r, r, m_s, p);
}

void mesh::set_front(front front) {
	f = front;
}

void mesh::set_triangle(std::vector<triangle> triangles) {
	t = triangles;
}

front mesh::get_front() {
	return f;
}

std::vector<triangle>&  mesh::get_triangles() {
	return t;
}

std::vector<point>& mesh::get_points() {
	return p;
}

octree mesh::get_octree() {
	return p_octree;
}

void mesh::initilaizeOctree(double m_r, double r, double m_s) {
	std::vector<int> points;
	for (int i = 0; i < p.size(); i++) points.push_back(i);
	p_octree = octree(point(0, 0), points, m_r, r, m_s, p);
}

void mesh::advanceFront(double segment_length, double epsilon) {
	while (get_front().get_edges().size() > 0) {
		
		const int shortest_pos = get_shortestSegment();

		point ideal = get_idealPoint_2d(shortest_pos);
		std::vector<int> nearby_points;


/*		
		if (nearby) ideal = nearby;
		if (!isInsideFront) {
			ideal = getBetterInside();
		}
		addPoint();
		addSegments();
		addTriangle();
		removeSegment;
	*/	
	}
}	

	void mesh::splitSegmentsInFront(double length, double min) {
		std::vector<segment> new_front;
		const std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {

			if (it->get_length(p) > (length * min)) {
				double modulus = fmod(it->get_length(p),length);
				double egeszresz = (it->get_length(p) - modulus) / length;
				double actual_length = length + (modulus / egeszresz);
				point new_terminal;
				int new_terminal_pos;
				point new_initial;
				int new_initial_pos = it->get_initial();
				double x = p[it->get_initial()].get_x();
				double y = p[it->get_initial()].get_y();
				for (int i = 1; i < egeszresz; i++) {
					 x = x + cos(it->get_angle_2d(p)) * actual_length;
					 y = y + sin(it->get_angle_2d(p)) * actual_length;
					new_terminal = point(x,y,3);
					p.push_back(new_terminal);
					new_terminal_pos = p.size();
					new_front.push_back(segment(new_initial_pos,new_terminal_pos));
					new_initial = new_terminal;
					new_initial_pos = new_terminal_pos;
				}
				new_front.push_back(segment(new_initial_pos, it->get_terminal()));
			}
			else new_front.push_back(*it);
		}

		f = new_front;
	}

	int mesh::get_shortestSegment() {
		const std::vector<segment> f_copy = f.get_edges();
		int shortest_pos = 0;
		int counter = 0;
		double shortest_length = f_copy[0].get_length(p);		
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
				shortest_length = (shortest_length < it->get_length(p) ? shortest_length : it->get_length(p));
				shortest_pos = (shortest_length < it->get_length(p) ? shortest_pos : counter);
			
			counter++;
		}
		return shortest_pos;
	}

	point mesh::get_idealPoint_2d(int segment_pos) {
		point initial	= p[(get_front().get_edges())[segment_pos].get_initial()];
		point terminal	= p[(get_front().get_edges())[segment_pos].get_terminal()];
		double x = cos(M_PI / 3) * (terminal.get_x() - initial.get_x()) - sin(M_PI / 3) * (terminal.get_y() - initial.get_y()) + initial.get_x();
		double y = sin(M_PI / 3) * (terminal.get_x() - initial.get_x()) + cos(M_PI / 3) * (terminal.get_y() - initial.get_y()) + initial.get_y();
		double z = initial.get_z();

		return point(x,y,z);
	}

	// -:p0 is right of the line defined by lp1 and lp2
	// 0:p0 is on the line
	// +:p0 is left of the line
	int mesh::isLeft_2d(point lp1, point lp2, point p0) {
		return ((lp2.get_x() - lp1.get_x()) * (p0.get_y() - lp1.get_y()) - (p0.get_x() - lp1.get_x()) * (lp2.get_y() - lp1.get_y()));
	}



	//winding-number algorithm, wn = 0 if p0 is ousside 
	int mesh::nw_wn_pointInsideFront(point p0) {
		int wn = 0;
		const std::vector<segment> f_copy = f.get_edges();
		int counter = 0;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			counter++;
			if (p[it->get_initial()].get_y() <= p0.get_y()) {
				if (p[it->get_terminal()].get_y() > p0.get_y()) {
					if (isLeft_2d(p[it->get_initial()], p[it->get_terminal()], p0) > 0)
						++wn;
				}
			}
			else {
				if (p[it->get_terminal()].get_y() <= p0.get_y()) {
					if (isLeft_2d(p[it->get_initial()], p[it->get_terminal()], p0) > 0)
						--wn;
				}
			}
			std::cout << counter;
		}
		return wn;
	}

	bool mesh::nw_angle_pointInsideFront(point p0) {
		const std::vector<segment> f_copy = f.get_edges();
		double angle = 0;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			point p1 = p[it->get_initial()];
			point p2 = p[it->get_terminal()];
			double dtheta = atan2(p2.get_y(), p2.get_x()) - atan2(p1.get_y(), p1.get_x());
			while (dtheta > M_PI)
				dtheta -= (2*M_PI);
			while (dtheta < -M_PI)
				dtheta += (2*M_PI);
			angle += dtheta;
		}
		if (abs(angle) < M_PI)
			return(0);
		else
			return(1);
	}


	bool mesh::last_resort_pointInsideFront(point p0) {
		bool c = 0;
		const std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			point initial = p[it->get_initial()];
			point terminal = p[it->get_terminal()];

			if (((initial.get_y() > p0.get_y()) != (terminal.get_y() > p0.get_y())) && 
				(p0.get_x() < (terminal.get_x() - initial.get_x() * (p0.get_y() - initial.get_y()) / (terminal.get_y() - initial.get_y()) + initial.get_x()) ))
			c = !c;

		}
		return c;
	}

	int mesh::cn_PnPoly_2d(point p0) {
		const std::vector<segment> f_copy = f.get_edges();
		int cn = 0;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			point initial = p[it->get_initial()];
			point terminal = p[it->get_terminal()];
			if ((((initial.get_y() <= p0.get_y()) && (terminal.get_y() > p0.get_y())))
				|| ((initial.get_y() > p0.get_y()) && (terminal.get_y() <= p0.get_y()))) {
				float vt = (float)(p0.get_y() - initial.get_y()) / (terminal.get_y() - initial.get_y());
				if (p0.get_x() < initial.get_x() + vt * (terminal.get_x() - initial.get_x()))
					++cn;
			}
		}
		return (cn & 1);
	}

