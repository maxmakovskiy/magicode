#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const static char* SAMPLE = "XMAS";
const static int SAMPLE_LEN = 4;

// for each point in given matrix:
// - brut force for horizontal occurences
// - brut force for vertical occurences
// - brut force for diagonal occurences
int
count_horizontal(
  const std::vector<std::string>& m, 
  size_t i_line, 
  size_t i_target
) {
  // so, here we should from left-to-right (usual order)
  // as well as right-to-left (inverse order)
  // BUT
  // if we are checking m[0][0] obviously that
  // we couldn't go more to the left
  // since "XMAS" is 4-charactes long
  // this has sence from m[i][0] to m[i][2], whatever is 'i'
  // for everything starts from [i][3] we could make the checks in INVERSE order
  // THE SAME for m[0][9] (let's suppose that lines are 10-charactes wide)
  // we could make the check to the right in this position
  // so we need to make step to m[0][6]
  // SO
  // in both cases our delta is equals 3
  const static int DELTA = 3;
  bool can_right_to_left = i_target >= DELTA;
  bool can_left_to_right = i_target + DELTA <= m[i_line].length();

  int left_to_right = 0;
  if (can_left_to_right) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line][i_target+i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { left_to_right = 1; }
  }

  int right_to_left = 0;
  if (can_right_to_left) {
    size_t i = 0;
    for(; i < SAMPLE_LEN; i++) {
      if(m[i_line][i_target-i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { right_to_left = 1; }
  }
  
  return left_to_right + right_to_left;
}

int
count_vertical(
  const std::vector<std::string>& m,
  size_t i_line,
  size_t i_target
) {
  const static int DELTA = 3;
  bool can_downup = i_line >= DELTA;
  bool can_updown = i_line + DELTA <= m.size();
  
  int updown = 0;
  if (can_updown) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line+i][i_target] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { updown = 1; }
  }

  int downup = 0;
  if (can_downup) {
    size_t i = 0;
    for(; i < SAMPLE_LEN; i++) {
      if(m[i_line-i][i_target] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { downup = 1; }
  }
  
  return updown + downup;
}

int 
count_diagonal(
  const std::vector<std::string>& m,
  size_t i_line,
  size_t i_target
) {
  const static int DELTA = 3;
  bool can_top_left = (i_line >= DELTA) && (i_target >= DELTA);
  bool can_top_right = (i_line >= DELTA) && (i_target + DELTA <= m.size());
  bool can_bottom_right = (i_line + DELTA < m.size()) && (i_target + DELTA < m.size());
  bool can_bottom_left = (i_line + DELTA < m.size()) && (i_target >= DELTA);
  
  int top_left = 0;
  if (can_top_left) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line-i][i_target-i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { top_left = 1; }
  }

  int top_right = 0;
  if (can_top_right) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line-i][i_target+i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { top_right = 1; }
  }

  int bottom_right = 0;
  if (can_bottom_right) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line+i][i_target+i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { bottom_right = 1; }
  }
  
  int bottom_left = 0;
  if (can_bottom_left) {
    size_t i = 0;
    for (; i < SAMPLE_LEN; i++) {
      if (m[i_line+i][i_target-i] != SAMPLE[i]) { break; }
    }
    if (i == SAMPLE_LEN) { bottom_left = 1; }
  }
  
  return top_left + top_right + bottom_right + bottom_left;
}


void part1() {
  // const char* filename = "test_data.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Cannot read given file : " << filename << std::endl;
    return;
  }

  std::vector<std::string> m;

  while (file) {
    std::string line;
    std::getline(file, line);
    if (line.empty()) { continue; }
    m.push_back(line);
  }
  
  int counter_hor = 0;
  int counter_vert = 0;
  int counter_diag = 0;

  for (size_t j = 0; j < m.size(); j++) {
    if (m[j].empty()) { continue; }
    for (size_t i = 0; i < m[j].length(); i++) {
      counter_hor += count_horizontal(m, j, i);
      counter_vert += count_vertical(m, j, i);
      counter_diag += count_diagonal(m, j, i);
    }
  }

  std::cout << "Horizontal : " << counter_hor << "\n";
  std::cout << "Vertical : " << counter_vert << "\n";
  std::cout << "Diagonal : " << counter_diag << "\n";
  std::cout << "------------------------\n";
  std::cout << "In total : " << (counter_hor + counter_vert + counter_diag) << std::endl;
}


bool
is_match(
  const std::vector<std::string>& data,
  size_t i_row,
  size_t i_col
) {
  const static char* PATTERN = "MAS";
  const static size_t P_LEN = 3;

  bool part1_forward = false;
  size_t i = 0;
  for (; i < P_LEN; i++) {
    if (data[i_row-1+i][i_col-1+i] != PATTERN[i]) { break; }
  }
  if (i == P_LEN) { part1_forward = true; }
  
  // try read it backwords
  bool part1_backword = false;
  i = 0;
  for (; i < P_LEN; i++) {
    if (data[i_row+1-i][i_col+1-i] != PATTERN[i]) { break; }
  }
  if (i == P_LEN) {
    part1_backword = true;
  }
  bool part1 = part1_forward ^ part1_backword;

  // top-right to bottom-left
  bool part2_forward = false;
  i = 0;
  for (; i < P_LEN; i++) {
    if (data[i_row-1+i][i_col+1-i] != PATTERN[i]) { break; }
  }
  if (i == P_LEN) {
    part2_forward = true;
  }

  // bottom-left to top-right
  bool part2_backward = false;
  i = 0;
  for (; i < P_LEN; i++) {
    if (data[i_row+1-i][i_col-1+i] != PATTERN[i]) { break; }
  }
  if (i == P_LEN) {
    part2_backward = true;
  }
  bool part2 = part2_forward ^ part2_backward;

  return part1 && part2;
}

void part2() {
  // const char* filename = "test_data_2.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Cannot read this file : " << filename << std::endl;
    return;
  }

  std::vector<std::string> data;
  while (file) {
    std::string line;
    std::getline(file, line);

    if (line.empty()) { continue; }
    
    data.push_back(line);
  }

  // it's pointless to check top and bottom strings
  // as well as leftest and rightest chars of each string
  // since target will be cut somewhere there
  // so, we would like to keep padding of 1 char

  int n_occurences = 0;
  for (size_t i = 1; i < data.size()-1; i++) {
    for (size_t j = 1; j < data[j].size()-1; j++) {
      if (data[i][j] == 'A') {
        // so we are checking it for the pattern
        n_occurences += static_cast<int>(is_match(data, i, j));
      }
    }
  }

  std::cout << "X-MAS count " << n_occurences << std::endl;
  
}




int main() {
  // part1();
  part2();

  return 0;
}
