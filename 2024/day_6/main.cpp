#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <utility>

enum class Direction {
  Never, Up, Down, Right, Left
};

struct Point {
  int x;
  int y;
};

void 
traverse(
  std::vector<std::string>& map, 
  int x_guard, int y_guard, Direction dir,
  int map_width, int map_height
) {
  bool game_over = false;
  while (!game_over) {
    switch (dir) {
      case Direction::Up: {
        int next_y = y_guard-1;
        if (next_y >= 0) {
          char& step = map[next_y][x_guard];
          if (step == '#') {
            dir = Direction::Right;
          } else {
            step = 'X';
            y_guard = next_y;
          }
        } else {
          game_over = true;
        }
      } break;
      case Direction::Right: {
        int next_x = x_guard+1;
        if (x_guard < map_width) {
          char& step = map[y_guard][next_x];
          if (step == '#') {
            dir = Direction::Down;
          } else {
            step = 'X';
            x_guard = next_x;
          } 
        } else {
            game_over = true;
        }
      } break;
      case Direction::Down: {
        int next_y = y_guard+1;
        if (next_y < map_width) {
          char& step = map[next_y][x_guard];
          if (step == '#') {
            dir = Direction::Left;
          } else {
            step = 'X';
            y_guard = next_y;
          }  
        } else {
          game_over = true;
        }
      } break;
      case Direction::Left: {
        int next_x = x_guard-1;
        if (next_x >= 0) {
          char& step = map[y_guard][next_x];
          if (step == '#') {
            dir = Direction::Up;
          } else {
            step = 'X';
            x_guard = next_x;
          }  
        } else {
          game_over = true;
        }
      } break;
    } // switch
  } // while (!game_over)
}

const static std::vector<std::pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

bool could_step(
  const std::vector<std::string>& map, 
  int x, int y, int direction, 
  int map_width, int map_height
) {
  int next_y = y + DIRECTIONS[direction].first;
  int next_x = x + DIRECTIONS[direction].second;

  if (next_x < 0 || next_x >= map_width || 
      next_y < 0 || next_y >= map_height
  ) {
    return true;
  }
  
  return map[next_y][next_x] != '#';
}

int simulate(
  std::vector<std::string> map,
  int x, int y, int direction,
  int map_width, int map_height
) {
  int counter = 0;
  // basically trick is here
  // it's to keep score of directions for each cell
  // (-1 for never visited)
  // so, if we has already visited the cell
  // and it has been done with the same direction we are moving right now
  // we could conclude that we are in loop
  std::vector<std::vector<int>> map_dir(map_height, 
    std::vector<int>(map_width, -1));
  
  while(x >= 0 && x < map_width && y >= 0 && y < map_height) {
    // if we could move forward, so there is no obstruction
    // we move forward even though we are escaping the map
    if (could_step(map, x, y, direction, map_width, map_height)) {
      if (map[y][x] == 'X' && map_dir[y][x] == direction) {
        return 1;
      }
      map[y][x] = 'X';
      map_dir[y][x] = direction;
      y += DIRECTIONS[direction].first;
      x += DIRECTIONS[direction].second;
    } else {
      // change direction otherwise
      direction = (direction + 1) % 4;
    }
    
  }

  return 0;
}


int main() {
  // const char* filename = "test_data.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Cannot read file : " << filename << std::endl;
    return -1;
  }

  std::vector<std::string> map;
  std::string buf;

  int x_guard = 0;
  int y_guard = 0;
  for (int y = 0; std::getline(file, buf); ++y) {
    if (buf.empty()) { continue; }

    for (int x = 0; x < buf.length(); ++x) {
      if (buf[x] == '^') {
        x_guard = x;
        y_guard = y;
      }
    }
    map.push_back(buf);
  }
  
  int map_width = map.front().size();
  int map_height = map.size();
  
  traverse(
    map, 
    x_guard, y_guard, Direction::Up,
    map_width, map_height
  );

  int guard_path_len = 0;
  for (const std::string& row : map) {
    for (char cell : row) {
      if (cell == 'X') {
        guard_path_len += 1;
      }
    }
  }
  // +1 cause we don'nt put 'X' in the initial position of guard, there is '^'
  std::cout << "Part 1 : " << guard_path_len+1 << std::endl << std::endl;

  int n_loops = 0;
  
  for (size_t i = 0; i < map.size(); ++i) {
    for (size_t j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 'X') {
        // std::cout << "Checking " << "(x=" << j << ", y=" << i << ")\n";
        map[i][j] = '#';
        n_loops += simulate(
          map,
          // init direction is 0 which is {-1, 0} that suggests moving up by reducing the Y
          x_guard, y_guard, 0,
          map_width, map_height
        );
        map[i][j] = 'X'; 
      }
    }
  }
 
  std::cout << "Part 2 : " << n_loops << std::endl << std::endl;




/*
  int map_width = 0;
  int map_height = 0;
  
  std::vector<Point> map;
  Point guard;
  
  std::string buf;
  while (std::getline(file, buf)) {
    if (buf.empty()) { continue; }

    for (int i = 0; i < buf.size(); ++i) {
      if (map_width == 0) { 
        map_width = buf.size(); 
      }
      if (buf[i] == '#') {
        map.push_back(Point {i, map_height});
      } else if (buf[i] == '^') {
        guard.x = i;
        guard.y = map_height;
        guard.dir = GuardDir::UP;
      }
    }
    map_height += 1;
  }

  std::vector<Point> covered = part1(map, map_width, map_height, guard);
  
  std::cout << "Part 1: How many distinct positions \nwill the guard visit before leaving the mapped area? "
    << covered.size() << "\n\n";
*/
  
  return 0;
}
