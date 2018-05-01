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

void front::set_edges(std::vector<segment> e) {
	this->edges = e;
}

void front::add_segment(segment s) {
	this->edges.push_back(s);
}
//////////////////////////


mesh::mesh(std::vector<point> points, front front, double m_r, double r, double m_s) {
	p = points;
	f = front;
	std::vector<int> pos;
	for (int i = 0; i < p.size(); i++) pos.push_back(i);
	p_octree = octree(point(0, 0, 0), pos, m_r, r, m_s, p, NULL);
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
	p_octree = octree(point(0, 0), points, m_r, r, m_s, p, NULL);
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
					p_octree.insert_point(p.size() - 1, p);
					new_terminal_pos = p.size() - 1;
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

	int mesh::isLeft_2d(point p0, point p1, point p2) {
		return ( (p1.get_x() - p0.get_x()) * (p2.get_y() - p0.get_y()) 
				  - (p2.get_x() - p0.get_x()) * (p1.get_y() - p0.get_y()));
	}

	bool mesh::nw_wn_pointInsideFront(point p0) {
		int wn = 0;
		const std::vector<segment> f_copy = f.get_edges();
		int counter = 0;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			counter++;
			if (p[it->get_initial()].get_y() <= p0.get_y()) {          // start y <= p0.get_y()
				if (p[it->get_terminal()].get_y()  > p0.get_y())      // an upward crossing
					if (isLeft_2d(p[it->get_initial()], p[it->get_terminal()], p0) > 0)  // P left of  edge
						++wn;            // have  a valid up intersect
			}
			else {                        // start y > p0.get_y() (no test needed)
				if (p[it->get_terminal()].get_y() <= p0.get_y())     // a downward crossing
					if (isLeft_2d(p[it->get_initial()], p[it->get_terminal()], p0) < 0)  // P right of  edge
						--wn;            // have  a valid down intersect
			}
		}
		return (!wn == 0);
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

	///http://alienryderflex.com/polygon/
	bool mesh::pip5(point p0) {
		bool  oddNodes = false;
		float x = p0.get_x();
		float y = p0.get_y();
		int polyCorners = f.get_edges().size();
		std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			float polyYj = p[it->get_terminal()].get_y();
			float polyXj = p[it->get_terminal()].get_x();
			float polyYi = p[it->get_initial()].get_y();
			float polyXi = p[it->get_initial()].get_x();

			if (polyYi<y && polyYj >= y
				|| polyYj<y && polyYi >= y) {
				if (polyXi + (y - polyYi) / (polyYj - polyYi)*(polyXj - polyXi)<x) {
					oddNodes = !oddNodes;
				}
			}
		}
		return oddNodes;
	}

	bool mesh::pip6(point p0) {
		double y = p0.get_y();
		double x = p0.get_x();
		int leftIntersection = 0;
		int rightIntersection = 0;
		std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			double initial_x = p[it->get_initial()].get_x();
			double initial_y = p[it->get_initial()].get_y();
			double terminal_x = p[it->get_terminal()].get_x();
			double terminal_y = p[it->get_terminal()].get_y();
			if ((initial_y < y && terminal_y >= y) || (initial_y >= y && terminal_y < y)) {

			if (terminal_y - initial_y == 0) return false;
				double x_intersection = ((terminal_x - initial_x) * (y - initial_y)) / (terminal_y - initial_y) + initial_x;
				if (x_intersection < x) leftIntersection++;
				else if (x_intersection > x) rightIntersection++;
				else return false;
			}
		}	
		if ((leftIntersection % 2) == 1 && (rightIntersection % 2) == 1)
		{
			return true;
		}
		return false;
	}

	bool mesh::last_resort_pointInsideFront(point p0) {
		std::vector<segment> copy_front = f.get_edges();
		for (auto it = copy_front.begin(); it != copy_front.end(); ++it) {
			if ((p[it->get_initial()] == p0 || p[it->get_terminal()] == p0)) return true;
		}
		int i, j, c = 0;
		std::vector<float> verty;
		std::vector<float> vertx;
		float testx = p0.get_x();
		float testy = p0.get_y();
		//front darabok keresése
		std::vector<segment> front1;
		std::vector<segment> front2;
		std::vector<segment> copy_front1 = f.get_edges();
		std::vector<segment> copy_front2 = f.get_edges();
		std::vector<segment> copy_front3 = f.get_edges();
		int c1 = 0;
		int c2 = 0;
		int c3 = 0;
		bool talalt = false;
		int elagazas;
		int egyik;
		int masik;
		if (0) {
			for (auto it1 = copy_front1.begin(); it1 != copy_front1.end(); ++it1) {
				for (auto it2 = copy_front2.begin(); it2 != copy_front2.end(); ++it2) {
					for (auto it3 = copy_front3.begin(); it3 != copy_front3.end(); ++it3) {
						if ((it1->get_initial() == it2->get_terminal()) && (it1->get_initial() == it3->get_terminal()) && (it2->get_initial() != it3->get_initial())) {
							elagazas = c1;
							egyik = c2;
							masik = c3;
							talalt = true;
							break;
						}

						c3++;
					}
					if (talalt) break;
					c2++;
					c3 = 0;
				}
				if (talalt) break;
				c1++;
				c2 = 0;
			}
			if (talalt) {
				front1.push_back(copy_front2[egyik]);
				front2.push_back(copy_front3[masik]);
				int first = copy_front1[elagazas].get_initial();
				int last1 = copy_front2[egyik].get_initial();
				while (first != last1) {
					for (auto it = copy_front.begin(); it != copy_front.end(); ++it) {
						if (it->get_terminal() == last1) {
							front1.push_back(*it);
							last1 = it->get_initial();
							break;
						}
					}
				}
				int last2 = copy_front3[masik].get_initial();
				int error = 0;
				while (first != last2 || error > 200) {
					for (auto it = copy_front.begin(); it != copy_front.end(); ++it) {
						if (it->get_terminal() == last2) {
							front2.push_back(*it);
							last2 = it->get_initial();
							break;
						}
					}
					error++;
				}
				copy_front = front1;
				vertx.push_back(0);
				verty.push_back(0);
				for (auto it = copy_front.begin(); it != copy_front.end(); ++it) {
					vertx.push_back(p[it->get_initial()].get_x());
					verty.push_back(p[it->get_initial()].get_y());
				}
				vertx.push_back(p[copy_front.begin()->get_initial()].get_x());
				verty.push_back(p[copy_front.begin()->get_initial()].get_y());

				copy_front = front2;
				vertx.push_back(0);
				verty.push_back(0);
				for (auto it = copy_front.begin(); it != copy_front.end(); ++it) {
					vertx.push_back(p[it->get_initial()].get_x());
					verty.push_back(p[it->get_initial()].get_y());
				}
				vertx.push_back(p[copy_front.begin()->get_initial()].get_x());
				verty.push_back(p[copy_front.begin()->get_initial()].get_y());
				vertx.push_back(0);
				verty.push_back(0);
				int nvert = verty.size();
				for (i = 0, j = nvert - 1; i < nvert; j = i++) {
					if (((verty[i] > testy) != (verty[j] > testy)) &&
						(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
						c = !c;
				}
				return c;
			}
		}
		int nvert = f.get_edges().size();
		std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			vertx.push_back(p[it->get_initial()].get_x());
			verty.push_back(p[it->get_initial()].get_y());
		}
		for (i = 0, j = nvert - 1; i < nvert; j = i++) {
			if (((verty[i]>testy) != (verty[j]>testy)) &&
				(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
				c = !c;
		}
		return c;
	}





	int mesh::pip4(point p0) {
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


	std::vector<int> mesh::nearby_points_nearby(const point p0, double epsilon) {
		std::vector<int> result;
		octree* parent = NULL;
		octree* node = &p_octree;
		while (!node->isLeaf()) {
			for (int i = 0; i < node->get_points().size(); ++i) {
				double dist = sqrt(pow(p0.get_x() - p[node->get_points()[i]].get_x(), 2) + pow(p0.get_y() - p[node->get_points()[i]].get_y(), 2) + pow(p0.get_z() - p[node->get_points()[i]].get_z(), 2)) - 0.001;
				if (dist < epsilon ) {
					result.push_back(node->get_points()[i]);
				}
			}
			node = node->get_children()[node->getContainerChild(p0)];
			parent = node->get_parent();
		}
		std::vector<int> p_copy = parent->get_points();
		for (auto it0 = p_copy.begin(); it0 != p_copy.end(); ++it0) {
			int counter = 0;
			double dist0 = sqrt(pow(p0.get_x() - p[*it0].get_x(), 2) + pow(p0.get_y() - p[*it0].get_y(), 2) + pow(p0.get_z() - p[*it0].get_z(), 2)) - 0.001;
			if (dist0 <= epsilon) {
				std::vector<int> result_copy = result;
				for (auto it1 = result_copy.begin(); it1 != result_copy.end(); ++it1) {
					if (result_copy.size() == 0)	result.push_back(*it0);
					else {
						double dist1 = sqrt(pow(p0.get_x() - p[*it1].get_x(), 2) + pow(p0.get_y() - p[*it1].get_y(), 2) + pow(p0.get_z() - p[*it1].get_z(), 2)) - 0.001;
						if ((((dist0 - dist1) < 0.001) && (*it0 != *it1)) || (dist0 < dist1 && (*it0 != *it1))) {
							result.insert(result.begin() + counter, *it0);
							break;
						}
					}
					counter++;
				}
			}
		}
		for (int i = 0; i < 8; ++i) {
			if (parent->get_children()[i] != NULL) {
				std::vector<int> p_copy = parent->get_children()[i]->get_points();
				for (auto it0 = p_copy.begin(); it0 != p_copy.end(); ++it0) {
					int counter = 0;
					double dist0 = sqrt(pow(p0.get_x() - p[*it0].get_x(), 2) + pow(p0.get_y() - p[*it0].get_y(), 2) + pow(p0.get_z() - p[*it0].get_z(), 2));
					if (dist0 <= epsilon) {
						if (result.size() == 0)	result.push_back(*it0);
						std::vector<int> result_copy = result;
						for (auto it1 = result_copy.begin(); it1 != result_copy.end(); ++it1) {
								double dist1 = sqrt(pow(p0.get_x() - p[*it1].get_x(), 2) + pow(p0.get_y() - p[*it1].get_y(), 2) + pow(p0.get_z() - p[*it1].get_z(), 2));
								if ((((dist0 - dist1) < 0.001) && (*it0 != *it1)) || ((dist0 < dist1 && (*it0 != *it1)))) {
									result.insert(result.begin() + counter, *it0);
									break;
								}
							counter++;
						}
					}
				}
			}
		}
		return result;
	}

	std::vector<int> mesh::nearby_points_leaf(const point p0, double epsilon) {
			octree node = p_octree;
			std::vector<int> result;
			while (!node.isLeaf()) {
				node = *node.get_children()[node.getContainerChild(p0)];
			}
			std::vector<int> p_copy = node.get_points();
			for (auto it = p_copy.begin(); it != p_copy.end(); ++it) {
				double dist = sqrt(pow(p0.get_x() - p[*it].get_x(), 2) + pow(p0.get_y() - p[*it].get_y(), 2) + pow(p0.get_z() - p[*it].get_z(), 2));

				if (dist <= epsilon)
					result.push_back(*it);
			}
			return result;
	}

	bool mesh::isIntersectingFront(segment s) {
		point p0 = p[s.get_initial()];
		point p1 = p[s.get_terminal()];
		std::vector<int> nearby_points = nearby_points_nearby(p0, s.get_length(p) * 2);
		std::vector<int> nearby_points_temp = nearby_points_nearby(p1, s.get_length(p) * 2);
		for (auto it0 = nearby_points_temp.begin(); it0 != nearby_points_temp.end(); ++it0) {
			bool new_element = true;
			for (auto it1 = nearby_points.begin(); it1 != nearby_points.end(); ++it1) {
				if (*it1 == *it0) new_element = false;
			}
			if (new_element) nearby_points.push_back(*it0);
		}
		std::vector<segment> nearby_segments;
		std::vector<segment> f_copy = f.get_edges();
		for (auto it0 = f_copy.begin(); it0 != f_copy.end(); ++it0) {
			for (auto it1 = nearby_points.begin(); it1 != nearby_points.end(); ++it1) {
				if (it0->get_initial() == *it1 || it0->get_terminal() == *it1) nearby_segments.push_back(*it0);
			}
		}
		bool intersecting = false;
		for (auto it = nearby_segments.begin(); it != nearby_segments.end() && !intersecting; ++it) {
			intersecting = it->isIntersected(s, p);
		}
		return intersecting;
	}

	bool mesh::isIntersectingFront(segment s, segment shortest) {
		point p0 = p[s.get_initial()];
		point p1 = p[s.get_terminal()];
		if (last_resort_pointInsideFront(p0) && ((s.get_terminal() == shortest.get_initial() || s.get_terminal() == shortest.get_terminal()))) return false;
		if (last_resort_pointInsideFront(p1) && ((s.get_initial() == shortest.get_initial() || s.get_initial() == shortest.get_terminal()))) return false;

		std::vector<int> nearby_points = nearby_points_nearby(p0, s.get_length(p) * 2);
		std::vector<int> nearby_points_temp = nearby_points_nearby(p1, s.get_length(p) * 2);
		for (auto it0 = nearby_points_temp.begin(); it0 != nearby_points_temp.end(); ++it0) {
			bool new_element = true;
			for (auto it1 = nearby_points.begin(); it1 != nearby_points.end(); ++it1) {
				if (*it1 == *it0) new_element = false;
			}
			if (new_element) nearby_points.push_back(*it0);
		}
		std::vector<segment> nearby_segments;
		std::vector<segment> f_copy = f.get_edges();
		for (auto it0 = f_copy.begin(); it0 != f_copy.end(); ++it0) {
			for (auto it1 = nearby_points.begin(); it1 != nearby_points.end(); ++it1) {
				if (it0->get_initial() == *it1 || it0->get_terminal() == *it1) nearby_segments.push_back(*it0);
			}
		}
		bool intersecting = false;
		for (auto it = nearby_segments.begin(); it != nearby_segments.end() && !intersecting; ++it) {
			intersecting = it->isIntersected(s, p);
		}
		return intersecting;
	}

	bool mesh::isInFront(segment s) {
		bool isit = false;
		std::vector<segment> f_copy = f.get_edges();
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			if ((s.get_initial() == it->get_initial() && s.get_terminal() == it->get_terminal()) || (s.get_initial() == it->get_terminal() && s.get_terminal() == it->get_initial())) isit = true;
		}
		return isit;
	}

	bool mesh::tryTriangle(segment& s, int p_pos) {
		segment s1 = segment(s.get_initial(),p_pos);
		segment s2 = segment(p_pos, s.get_terminal());

		bool elso = last_resort_pointInsideFront(p[p_pos]);
		bool masodik = isIntersectingFront(s1,s);
		bool harmadik = isIntersectingFront(s2,s);
		std::cout << "belul van:" << elso << "\n metszi s1 a frontot:" << masodik << "\n metszi s2 a frontot:" << harmadik << std::endl;
		if ( !last_resort_pointInsideFront(p[p_pos]) || isIntersectingFront(s1,s) || isIntersectingFront(s2,s)) 
			return false;
		if (isInFront(s1)) removeFromFront(s1);
		else f.add_segment(s1);
		if (isInFront(s2)) removeFromFront(s2);
		else f.add_segment(s2);
		if (isInFront(s)) removeFromFront(s);
		t.push_back(triangle(s.get_initial(), s.get_terminal(), p_pos));
		
		return true;
	}

	void mesh::advanceFront(double close_distance, double far_distance) {
		int infiniteloop = 0;
		while (get_front().get_edges().size() > 0) {
			infiniteloop++;
			//if (get_front().get_edges().size() >= 3 && 1) {

			//				//try to find 3 long loop in front
			//	std::vector<segment> front_copy1 = f.get_edges();
			//	std::vector<segment> front_copy2 = f.get_edges();
			//	std::vector<segment> front_copy3 = f.get_edges();
			//	int loopcounter = 0;
			//	for (auto it1 = front_copy1.begin(); it1 != front_copy1.end(); ++it1) {

			//		for (auto it2 = front_copy2.begin(); it2 != front_copy2.end(); ++it2) {
			//			for (auto it3 = front_copy3.begin(); it3 != front_copy3.end(); ++it3) {
			//				if ((it1->get_terminal() == it2->get_initial() && it2->get_terminal() == it3->get_initial() && it3->get_terminal() == it1->get_initial()) && 
			//					(!it1->isEqual(*it2) && !it1->isEqual(*it3) && !it2->isEqual(*it3))) {
			//					if (isInFront(*it1)) removeFromFront(*it1);
			//					if (isInFront(*it2)) removeFromFront(*it2);
			//					if (isInFront(*it3)) removeFromFront(*it3);
			//					t.push_back(triangle(it2->get_initial(), it2->get_terminal(), it3->get_terminal()));
			//					break;
			//				}
			//			}
			//		}
			//	}
			//}

			if (t.size() > 50) break;
			//try to find 4 long loop in front
			if (get_front().get_edges().size() >= 4 && 0) {
				std::vector<segment> front_copy1 = f.get_edges();
				std::vector<segment> front_copy2 = f.get_edges();
				std::vector<segment> front_copy3 = f.get_edges();
				std::vector<segment> front_copy4 = f.get_edges();
				for (auto it1 = front_copy1.begin(); it1 != front_copy1.end(); ++it1) {

					for (auto it2 = front_copy2.begin(); it2 != front_copy2.end(); ++it2) {
						for (auto it3 = front_copy3.begin(); it3 != front_copy3.end(); ++it3) {
							if ((it1->get_terminal() == it2->get_initial() && it2->get_terminal() == it3->get_initial() && it3->get_terminal() == it1->get_initial()) &&
								(!it1->isEqual(*it2) && !it1->isEqual(*it3) && !it2->isEqual(*it3))) {
								if (isInFront(*it1)) removeFromFront(*it1);
								if (isInFront(*it2)) removeFromFront(*it2);
								if (isInFront(*it3)) removeFromFront(*it3);
								t.push_back(triangle(it2->get_initial(), it2->get_terminal(), it3->get_terminal()));
								break;
							}
							for (auto it4 = front_copy3.begin(); it4 != front_copy3.end(); ++it4) {
								if ((it1->get_terminal() == it2->get_initial() && it2->get_terminal() == it3->get_initial() && it3->get_terminal() == it4->get_initial()
									&& it4->get_terminal() == it1->get_initial()) &&
									((!it1->isEqual(*it2) || !it1->isEqual(*it3) || !it1->isEqual(*it4)) && (!it2->isEqual(*it3) || !it2->isEqual(*it4)) && (!it3->isEqual(*it4))))
								{
									segment temp1 = segment(it2->get_initial(), it3->get_terminal());
									segment temp2 = segment(it3->get_initial(), it4->get_terminal());
									double length1 = temp1.get_length(p);
									double length2 = temp2.get_length(p);
									if (length1 < length2) {
										if (isInFront(*it1)) removeFromFront(*it1);
										if (isInFront(*it2)) removeFromFront(*it2);
										if (isInFront(*it3)) removeFromFront(*it3);
										if (isInFront(*it4)) removeFromFront(*it4);
										t.push_back(triangle(it2->get_initial(), it2->get_terminal(), it3->get_terminal()));
										t.push_back(triangle(it4->get_initial(), it4->get_terminal(), it1->get_terminal()));
									}
									else {
										if (isInFront(*it1)) removeFromFront(*it1);
										if (isInFront(*it2)) removeFromFront(*it2);
										if (isInFront(*it1)) removeFromFront(*it3);
										if (isInFront(*it4)) removeFromFront(*it4);
										t.push_back(triangle(it1->get_initial(), it1->get_terminal(), it1->get_terminal()));
										t.push_back(triangle(it3->get_initial(), it3->get_terminal(), it4->get_terminal()));
									}

									break;
								}
							}
						}
					}
				}
		}
			int shortest_pos;
			if (f.get_edges().size() != 0) {
				if(abs(f.get_edges()[get_shortestSegment()].get_length(p) - f.get_edges()[0].get_length(p)) < 0.001) shortest_pos = 0;
				else shortest_pos = get_shortestSegment();
			}

			else break;

			point ideal = get_idealPoint_2d(shortest_pos);
			int ideal_pos;
			std::vector<int> nearby_points = nearby_points_nearby(ideal, close_distance);

			bool add_ideal_point = true;
			for (auto it = nearby_points.begin(); it != nearby_points.end(); ++it) {
				if (p[*it] == ideal) add_ideal_point = false;
			}
			int new_point_pos = -19940531;
			if (add_ideal_point) {
				p.push_back(ideal);
				ideal_pos = p.size() - 1;
				p_octree.insert_point(ideal_pos, p);
				nearby_points.push_back(ideal_pos);
				new_point_pos = nearby_points.size() - 1;
			}
			
			std::vector<int> nearby_points_temp = nearby_points_nearby(ideal, far_distance);
			for (auto it0 = nearby_points_temp.begin(); it0 != nearby_points_temp.end(); ++it0) {
				bool new_element = true;
				for (auto it1 = nearby_points.begin(); it1 != nearby_points.end(); ++it1) {
					if ((*it1 == *it0) || (*it0 == f.get_edges().at(shortest_pos).get_initial()) || (*it0 == f.get_edges().at(shortest_pos).get_terminal()))	new_element = false;
				}
				if (new_element == true) nearby_points.push_back(*it0);
			}
			int counter = 0;
			int tsize = t.size();
			if (tsize == 38) {
				tsize = tsize;
		//		break;
			}
			while (counter < nearby_points.size()) {
				if (tryTriangle((f.get_edges().at(shortest_pos)) , (nearby_points.at(counter)))) 
					break;
				counter++;
			}
			if (add_ideal_point) {
				if (counter != new_point_pos) {
					p_octree.delete_point(ideal_pos,p);
					p.pop_back(); //if we did not use the "new" ideal point 
				}
			}
		if (infiniteloop > 19) {
		//	break;
			}
		if (infiniteloop > 16) {
			infiniteloop = infiniteloop;
//			break;
		}
			}

		}

	void mesh::removeFromFront(segment s) {
		int ssegment,inverted_segment;
		bool found_segment = false;
		bool found_inverted_segment = false;
		std::vector<segment> f_copy = f.get_edges();
		int counter = 0;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			if (s.get_initial() == it->get_initial() && s.get_terminal() == it->get_terminal()) {
				found_segment = true;
				ssegment = counter;
			}
			if (s.get_initial() == it->get_terminal() && s.get_terminal() == it->get_initial()) {
				found_inverted_segment = true;
				inverted_segment = counter;
			}
				counter++;
			}
		if (found_segment && found_inverted_segment) f_copy.erase(f_copy.begin() + inverted_segment);
		if (!found_segment && found_inverted_segment) f_copy.erase(f_copy.begin() + inverted_segment);
		if (found_segment && !found_inverted_segment) f_copy.erase(f_copy.begin() +ssegment);
		f.set_edges(f_copy);
		}
	
	void mesh::draw(sf::Color c_triangle, sf::Color c_triangle_edge, sf::Color c_front, sf::Color c_active, sf::Color c_point, int boundary, sf::RenderWindow& w) {
		for (int i = 0; i < t.size(); ++i) {
			t[i].draw(c_triangle,c_triangle_edge, boundary, w, p); 
		}
		for (int i = 0; i < p.size(); ++i) {
			p[i].draw(c_point, boundary, w);
		}
		for (int i = 0; i < f.get_edges().size(); ++i) {
			f.get_edges()[i].draw(c_front, c_active, boundary, w, p);
		}

	}

	bool mesh::pip7(const point& p0)
	{
		std::vector<segment> f_copy = f.get_edges();
		std::vector<point> points_list;
		for (auto it = f_copy.begin(); it != f_copy.end(); ++it) {
			points_list.push_back(p[it->get_initial()]);
		}
		
		// The winding number counter.
		int winding_number = 0;

		// Loop through all edges of the polygon.
		typedef std::vector<point>::size_type size_type;

		size_type size = points_list.size();

		for (size_type i = 0; i < size; ++i)             // Edge from point1 to points_list[i+1]
		{
			point point1(points_list[i]);
			point point2;

			// Wrap?
			if (i == (size - 1))
			{
				point2 = points_list[0];
			}
			else
			{
				point2 = points_list[i + 1];
			}

			if (point1.get_y() <= p0.get_y())                                    // start y <= point.y
			{
				if (point2.get_y() > p0.get_y())                                 // An upward crossing
				{
						if (((point2.get_x() - point1.get_x()) * (p0.get_y() - point1.get_y()) -
							(p0.get_x() - point1.get_x()) * (point2.get_y() - point1.get_y())) > 0)
					{
						++winding_number;                               // Have a valid up intersect
					}
				}
			}
			else
			{
				// start y > point.y (no test needed)
				if (point2.get_y() <= p0.get_y())                                // A downward crossing
				{
					if (((point2.get_x() - point1.get_x()) * (p0.get_y() - point1.get_y()) -
						(p0.get_x() - point1.get_x()) * (point2.get_y() - point1.get_y())) < 0)
					{
						--winding_number;                               // Have a valid down intersect
					}
				}
			}
		}

		return (winding_number != 0);
	}
