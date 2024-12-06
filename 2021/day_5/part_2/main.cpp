#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>


struct Point {
	int x = 0;
	int y = 0;
};


std::ostream& operator<<(std::ostream& out, const Point& p) {
	out << "(" << p.x << ", " << p.y << ")";
	return out;	
}

void extract_point(std::istream& in, Point& point) {
  std::string buf;
	std::getline(in, buf, ',');
	point.x = std::stoi(buf);
		
	std::getline(in, buf);				
	point.y = std::stoi(buf);
}


bool is_horizontal(const Point& p1, const Point& p2) {
	return p1.y == p2.y;
}

bool is_vertical(const Point& p1, const Point& p2) {
	return p1.x == p2.x;
}

// #define N (10)
#define N (1000)

void print_matrix(int** m) {
  for (size_t i = 0; i < N; i++) {
		std::cout << std::setw(N*2 + 1);
		std::stringstream ss;
		ss << " ";
		for (size_t j = 0; j < N; j++) {
			if (m[i][j] > 0) {
				ss << m[i][j] << " ";
			} else {
				ss << ". ";
			}
		}	
		std::cout << ss.str() << "\n";
	}
  
}


int main() {
	// std::ifstream file("test_data.txt");
	std::ifstream file("data.txt");

	int** matrix = (int**)malloc(sizeof(int*)*N);
  if (matrix == nullptr) {
    std::cerr << "Failed while allcating memory for matrix" << std::endl;
    return -1;
  }

	for (size_t i = 0; i < N; i++) {
		matrix[i] = (int*)calloc(N, sizeof(int));
	}


	if (!file) {
		std::cerr << "Can't read a file" << std::endl;
		return -1;
	}

	const static char* DELIM = "->";
	 
    std::string line;
	while(std::getline(file, line)) {
		std::stringstream left, right;
		std::string token;

		auto pos = line.find(DELIM);

		if (pos == std::string::npos) { continue; }

		left << line.substr(0, pos);
		right << line.substr(pos+2);

		Point p1, p2;
		extract_point(left, p1);
		extract_point(right, p2);
		
		//lines.push_back(std::make_pair(p1, p2));

		//std::cout << p1 << " | " << p2 << "\n";

		// add line to array "Matrix"
		if (is_horizontal(p1, p2)) {
      // std::cout << "horizontal line: " << p1 << " | " << p2 << "\n";
			if (p2.x - p1.x > 0) {
				for (int i = p1.x; i <= p2.x; i++) {
					matrix[p1.y][i] += 1;	
				}	
			} else {
				for (int i = p2.x; i <= p1.x; i++) {
					matrix[p1.y][i] += 1;	
				}	
			} 
		} else if (is_vertical(p1, p2)) {
      // std::cout << "vertical line: " << p1 << " | " << p2 << "\n";
			if (p2.y - p1.y > 0) {
				for (int i = p1.y; i <= p2.y; i++) {
					matrix[i][p1.x] += 1;	
				}	
			} else {
				for (int i = p2.y; i <= p1.y; i++) {
					matrix[i][p1.x] += 1;	
				}	
			}
		} else {
      //(6,4) and (2,0)
      if (p2.x > p1.x && p2.y > p1.y) {
        // case 1
        // std::cout << "down-right line: " << p1 << " | " << p2 << "\n";
        for (int i = 0; i <= p2.x-p1.x; i++) {
          matrix[p1.y+i][p1.x+i] += 1;	
        }	
      } else if (p1.x > p2.x && p1.y > p2.y) {
        // case 2
        // (6,4) and (2,0)
        // 
        // std::cout << "up-left line: " << p1 << " | " << p2 << "\n";
        for (int i = 0; i <= p1.x-p2.x; i++) {
          matrix[p2.y+i][p2.x+i] += 1;	
        }	
      } else {
        int d_x = p2.x - p1.x;
        int d_y = p2.y - p1.y;
        if (d_x > 0 && d_y <= 0) {
          // case 3
          // (5,5) -> (6,4) -> (7,3) -> (8,2) -> (9,1)
          // std::cout << "up-right line: " << p1 << " | " << p2 << "\n";
          for (int i = 0; i <= d_x; i++) {
            matrix[p1.y-i][p1.x+i] += 1;	
          }	
        } else {
          // case 4
          // (5,5) -> (4,6) -> (3,7) -> (2,8) -> (1,9)
          // std::cout << "down-left line: " << p1 << " | " << p2 << "\n";
          for (int i = 0; i <= d_y; i++) {
            matrix[p1.y+i][p1.x-i] += 1;	
          }	
        }
      }
    }
  }

	int overlap_counter = 0;
	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			if (matrix[i][j] > 1) {
				overlap_counter += 1;
			}
		}	
	}

	std::cout << overlap_counter << std::endl;
	// print_matrix(matrix);

	for (size_t i = 0; i < N; i++) {
		free(matrix[i]);
	}
	free(matrix);

	
	return 0;
}
