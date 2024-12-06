#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <iomanip>
#include <vector>


std::ostream& operator<<(std::ostream& out, const std::vector<int> data) {
  out << "[ ";
  for (const auto& val : data) {
    out << val << " ";
  }
  out << "]";
  return out;
}


bool is_simple_safe(const std::vector<int>& data) {
  std::vector<int> processed;

  for (size_t i = data.size()-1; i > 0; i--) {
    processed.push_back(data[i]-data[i-1]);
  }
  
  int neg_counter = 0;
  int pos_counter = 0;
  bool is_safe = true;
  for (const int& v : processed) {
    if (v < 0) {
      neg_counter += 1;
    } else {
      pos_counter += 1;
    }
    int delta = std::abs(v);
    is_safe = is_safe && (delta >= 1) && (delta <= 3);
    if (!is_safe) { break; }
  }

  // SAFE : [ 7 6 4 2 1 ]
  // save diff of d[i+1]-d[i]
  // from right to left
  // [-1 -2 -2 -1] -> good like all have the same sign and 1 <= d[i] <= 3

  // UNSAFE: [ 1 3 2 4 5 ]
  // [2 -1 2 1] -> not good 'cause signs aren't the same

  return is_safe && (pos_counter == 0 || neg_counter == 0);
}

bool
is_just_safe(
    const std::vector<int>& data
) {
  bool is_asc = data[0] < data[1];
  for (size_t i = 0; i < data.size()-1; i++) {
    int delta = std::abs(data[i+1]-data[i]);

    bool not_safe = (delta < 1) || (delta > 3)
       || (is_asc && data[i] > data[i+1])
       || (!is_asc && data[i] < data[i+1]);
    if (not_safe) {
      return false;
    }
  }

  return true;
}



// part 2
// std::pair<bool,size_t>
bool
is_sophisticated_safe(
  std::vector<int> data
) {
  
  // auto sorted_end = std::is_sorted_until(
  //   data.begin(), data.end());
  // if (sorted_end != data.end()) {
  //   data.erase(sorted_end);
  //   sorted_end = std::is_sorted_until(
  //     data.begin(), data.end());
  // }

  if (data.front() > data.back()) {
    std::reverse(data.begin(), data.end());
  }

  std::vector<int> deltas;
  for (size_t i = 0; i < data.size()-1; i++) {
    deltas.push_back(data[i+1]-data[i]);
  }

  for (size_t i = 0; i < deltas.size(); i++) {
    if (deltas[i] == 0) {
      // case with dublication
      // 46 47 47 48 49
      // 1 0 1 1
      data.erase(data.begin()+i);
      return false;
    }
    bool in_range = (deltas[i] >= 1) && (deltas[i] <= 3);
    if (!in_range && (i == deltas.size()-1)) {
      // so the problem obviously with last one
      // 46 47 48 53
      // 1 1 6
      // return std::make_pair(false, i+1);
      data.erase(data.begin()+i+1);
      return false;
    }

    if (deltas[i] < 1) {
      // 46 43 47 48 49
      // -3 4 1 1
      // as for -3 we know right away that 
      // we should delete 43
      // return std::make_pair(false, i+1);
      data.erase(data.begin()+i+1);
      return false;
    }
    
    if (deltas[i] > 3) {
      // 46 50 53 55 57
      // 4 3 2 2
      // as 4 > 3, 53-46=7 which is also out of range
      // so we're deleting 46
      // ////////////
      // 46 50 47 48 49
      // 4 -3 1 1
      // as 4 > 3 
      // 47-46 = 1 = which in range 
      // so we're deleting 50
      // ////////////
      int delta_over = data[i+2]-data[i];
      size_t i_remove = (
        (delta_over < 1 || delta_over > 3)
         ? i : i+1
      );
      // return std::make_pair(false, i_remove);
      data.erase(data.begin()+i_remove);
      // return std::make_pair(false, i_remove);
      return false;
    }
  }

  // return std::make_pair(true, 0);
  return true;
}







int main() {
  // const char* filename = "test_data.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "can't read given file : " << filename << std::endl;
    return -1;
  }

  // std::vector<int> reports;

  int safe_counter = 0;
  // TODO: it could have different number of elements per line
  
  while (file) {
    std::string buf;
    std::getline(file, buf);

    if (buf.empty()) { continue; }
    std::vector<int> data;
    
    std::stringstream ss;
    ss << buf;

    int t;
    for (size_t i = 0; ss >> t; i++) {
      data.push_back(t);
    }

    // part 1
    // bool is_safe = is_simple_safe(data);

    // part 2
    bool is_safe = is_just_safe(data);

    if (!is_safe) {
      for (size_t i = 0; i < data.size(); i++) {
        std::vector<int> temp;
        for (size_t j = 0; j < data.size(); j++) {
          if (j == i) { continue; }
          temp.push_back(data[j]);
        }
        is_safe = is_just_safe(temp);
        if (is_safe) {
          safe_counter += 1;
          break;
        }
      }
    } else {
      safe_counter += 1;
    }
      
  }

  std::cout << "Number of the safe reports is " << safe_counter << std::endl;
  
  
  return 0;
}