#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


const static int ASCII_DIGIT_SHIFT = 48;

const int map_width = 4;  
const int map_height = 4;  
const char* filename = "test_data_1.txt";
// const static int map_width = 8;  
// const static int map_height = 8;  
// const static char* filename = "test_data_2.txt";

struct Point {
  int x;
  int y;
  int height;
};


std::ostream& 
operator<<(
  std::ostream& os, 
  const Point& p
) {
  os << "(x=" << p.x 
     << ",y=" << p.y
     << ",height=" << p.height << ")";
  return os;
}


int 
make_step(
  std::vector<Point> map,
  int x, int y, int height,
  int x_target, int y_target
) {
  int counter = 0;
  if (map[x + y * map_width].height == 0 &&
      x == x_target && y == y_target)
  {
    return 1;
  }
  
  int step_up = y - 1;
  int idx_cell = x + step_up * map_width;
  if (step_up >= 0 &&
      map[idx_cell].height == height-1)
  {
    // if we could step up
    y = step_up;
    height -= 1;
    return make_step(map, x, y, height, x_target, y_target);
  }
  
  // try step right 
  int step_right = x + 1;
  idx_cell = step_right + y * map_width;
  if (step_right < map_width &&
      map[idx_cell].height == height-1)
  {
    x = step_right;
    height -= 1;
    counter += make_step(map, x, y, height, x_target, y_target);
  }
  
  // try step left
  int step_left = x - 1;
  idx_cell = step_left + y * map_width;
  if (step_left >= 0 &&
      map[idx_cell].height == height-1)
  {
    x = step_left;
    height -= 1;
    counter += make_step(map, x, y, height, x_target, y_target);
  }
  
  // try step down
  int step_down = y - 1;
  idx_cell = x + step_down * map_width;
  if (step_down >= 0 &&
      map[idx_cell].height == height-1)
  {
    y = step_down;
    height -= 1;
    counter += make_step(map, x, y, height, x_target, y_target);
  }
  
  return counter;
}


int 
try_find_path(
  const std::vector<Point>& map,
  const Point& start,
  const std::vector<Point>& finishes,
  int map_width, int map_height
) {
  int counter = 0;
  // try to do it backwords
  // from the position of each finish try
  // to find given start
  for (const Point& finish : finishes) {
    int x = finish.x;
    int y = finish.y;
    int height = finish.height;

    int x_target = start.x;
    int y_target = start.y;
    counter += make_step(
      map,
      x, y, height,
      x_target, y_target
    );
    
  } // loop of picking verticies
  
  return counter;
}


int main() {
  using namespace std;

  ifstream file(filename);

  if (!file) {
    cerr << "Cannot read the file : " << filename << endl;
    return -1;
  }

  vector<Point> map;
  vector<Point> trailheads;
  vector<Point> finishes;

  string buffer;
  for (int y = 0; getline(file, buffer);) {
    if (buffer.empty()) { continue; }
    
    for(int x = 0; x < buffer.length(); ++x) {
      int target = buffer[x] - ASCII_DIGIT_SHIFT; 
      Point p {
        .x = x,
        .y = y,
        .height = target 
      };
      map.push_back(p);
      if (target == 0) {
        trailheads.push_back(p);
      } else if (target == 9) {
        finishes.push_back(p);
      }
    }
    y += 1;
  }

  // iterate over the rows
  // easy - 'cause we have stored it in this liniar way
  // for (const Point& p : map) {
  //   cout << p << "\n";
  // }

  // // iterate over columns
  // const int row_len = 4;
  // const int col_len = 4;
  // // col_len is basically just a number of rows
  // for (int i = 0; i < col_len; ++i) {
  //   for (int j = 0; j < row_len; ++j) {
  //     cout << map[i + (j*row_len)] << "\n";
  //   }
  // }
  
  // // iterate over columns
  // const int row_len = 4;
  // const int col_len = 4;
  // for (int i = 0; i < map.size(); ++i) {
  //   cout << map[(i%row_len) + ((i/col_len)*row_len)] << "\n";
  // }

  int res = 0;
  for (Point& start : trailheads) {
    res += try_find_path(
      map,
      start, finishes,
      map_width, map_height
    );
  }

  cout << "result : " << res << endl;
  
  return 0;
}
