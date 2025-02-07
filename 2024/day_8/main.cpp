#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <cmath>

#define DEBUG_INFO
#define DRAW_MAP_P2

struct Antenna {
  int x;
  int y;
  char type;
  bool is_node;
};

std::ostream& operator<<(std::ostream& os, const Antenna& ant) {
  os << "(" << ant.x << "," << ant.y << ") type '" << ant.type << "'";
  return os;
}

bool check_save(
  std::vector<Antenna>& nodes, 
  int node_x, int node_y, char type,
  int width, int height
) {
  if (node_x < 0 || node_x >= width ||
      node_y < 0 || node_y >= height)
  { 
#ifdef DEBUG_INFO    
    std::cout << "Discarded (out of map): " << node_x << ", " << node_y << std::endl;
#endif
    return false; 
  }
  
  auto p = std::find_if(nodes.begin(), nodes.end(),
    [node_x, node_y, type] (Antenna& ant){
      return (node_x == ant.x &&
              node_y == ant.y);
    }
  );
  if (p == nodes.end()) { 
    nodes.push_back(Antenna {
      node_x, node_y,
      type, true
    });
  }
  
  return true;
}



int main() {
  using namespace std;
  
  // const char* filename = "test_data.txt";
  // const int width = 12;
  // const int height = 12;
  
  // const char* filename = "test_data_2.txt";
  // const int width = 10;
  // const int height = 10;
  
  const char* filename = "data.txt";
  const int width = 50;
  const int height = 50;
  
  ifstream file(filename);

  if (!file) {
    cerr << "Cannot read the file : " << filename << endl;
    return -1;
  }  
  
  vector<Antenna> ants;
  set<char> types;

  string buf;
  int y = 0;
  while (getline(file, buf)) {
    if (buf.empty()) { continue; }
    
    for (int x = 0; x < buf.length(); ++x) {
      if (buf[x] == '.') { continue; }
      ants.push_back(Antenna {
        x, y, buf[x], false
      });
      types.insert(buf[x]);
    }

    y += 1;
  }

  sort(ants.begin(), ants.end(),
    [](Antenna& lhs, Antenna& rhs) {
      return lhs.type < rhs.type;
    }
  );

  using ants_type_bounds = pair<
    vector<Antenna>::iterator,
    vector<Antenna>::iterator
  >;
  vector<ants_type_bounds> bounds;

  vector<Antenna>::iterator itr = ants.begin();
  for(char type : types) {
    if (itr == ants.end()) { break; }
    
    auto point = partition_point(itr,
      ants.end(), 
      [type] (const Antenna& ant) {
        return ant.type == type;
      }
    );
    bounds.emplace_back(itr, point);

    // cout << "type : " << type << " => ";
    // for (auto j = itr; j != point; ++j) {
    //   cout << "(" << j->x << "," << j->y << "); ";
    // }
    // cout << endl;
    
    itr = point;
  }  

/*
  // ///////////////////// 
  // PART 1 //////////////
  vector<Antenna> nodes;
  for (auto [first, last] : bounds) {

    sort(first, last, 
      [](Antenna& lhs, Antenna& rhs) {
        return lhs.x < rhs.x;
      }
    );
    
    for (auto iter = first; iter != last; ++iter) {
      Antenna& p1 = *iter;

      for (auto itr_p2 = iter+1; 
           itr_p2 != last; 
           ++itr_p2)
      {
        Antenna& p2 = *itr_p2;

        // cout << p1.type << " | " << p1 << " and " << p2 << "\n";

        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;

        int node_1_x = p1.x - dx;
        int node_1_y = p1.y - dy;
        
        int node_2_x = p2.x + dx;
        int node_2_y = p2.y + dy;

        check_save(nodes, 
          node_1_x, node_1_y, p1.type,
          width, height
        );
        check_save(nodes, 
          node_2_x, node_2_y, p1.type,
          width, height
        );
      } // 'for' of picking 2nd antenna
    } // 'for' of picking the 1st antenna
  } // 'for' of picking iterators over antennas of the same type
*/

  // ///////////////////// 
  // PART 2 //////////////
  vector<Antenna> nodes_2;
  for (auto [first, last] : bounds) {
    sort(first, last, 
      [](Antenna& lhs, Antenna& rhs) {
        return lhs.x < rhs.x;
      }
    );
    
    for (auto iter = first; iter != last; ++iter) {
      Antenna& p1 = *iter;

      for (auto itr_p2 = iter+1; 
           itr_p2 != last; 
           ++itr_p2)
      {
        Antenna& p2 = *itr_p2;

        cout << p1.type << " | " << p1 << " and " << p2 << "\n";

        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;

        for (int node_x = p1.x,
                 node_y = p1.y;
            check_save(nodes_2, 
              node_x, node_y, p1.type,
              width, height
            );
            node_x -= dx, node_y -= dy)
        {
        }
        
        for (int node_x = p2.x,
                 node_y = p2.y;
            check_save(nodes_2, 
              node_x, node_y, p1.type,
              width, height
            );
            node_x += dx, node_y += dy)
        {
        }
      } // 'for' of picking 2nd antenna
    } // 'for' of picking the 1st antenna
  } // 'for' of picking iterators over antennas of the same type

#ifdef DEBUG_INFO    
  // DRAW MAP for part 1, for part 2 we should replace nodes with nodes_2
  file.clear();
  file.seekg(0, ios::beg);

  vector<string> map;
  while (std::getline(file, buf)) {
    if (buf.empty()) { continue; }
    map.push_back(buf);
  }

#ifdef DRAW_MAP_P2
  for (const Antenna& n : nodes_2) {
#else    
  for (const Antenna& n : nodes) {
#endif    
    if (map[n.y][n.x] == '.') {
      map[n.y][n.x] = '#';
    }
  }

  cout << "  ";
  for (size_t i = 0; i < map.front().length(); ++i) {
    cout << (i%10);
  }
  cout << "\n";
  for (size_t i = 0; i < map.size(); ++i) {
    cout << " " << (i%10) << map[i] << "\n";
  }
#endif  

  /*
  // compute intersection
  vector<Antenna> ants_inter;
  for_each(ants.begin(), ants.end(),
    [&nodes, &ants_inter] (Antenna& ant) {
      auto p = find_if(nodes.begin(), nodes.end(),
        [&ant](Antenna& nant) {
          return (nant.x == ant.x && 
                  nant.y == ant.y);
          // return (nant.x == ant.x && 
          //         nant.y == ant.y &&
          //         nant.type != ant.type);
        }
      );
      if (p != nodes.end()) {
        cout << "Found intersection : " << ant << " with " << *p << endl;
        ants_inter.push_back(*p);
      }
    }
  );
  */

  // cout << "Antinodes total: " << nodes.size() << "\n"
  //      << "Intersection with antennas: " 
  //      << ants_inter.size() << "\n"
  //      << "Antinodes w/o intersection: "
  //      << (nodes.size()-ants_inter.size()) << endl;
  
  // cout << "Part 1: \n >>> Antinodes total: " << nodes.size() << endl;
  cout << "Part 2: \n >>> Antinodes total: " << nodes_2.size() << endl;

  
  return 0;
}
