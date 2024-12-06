#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>



int calc_distance_score(
  const std::vector<int>& left, 
  const std::vector<int>& right
) {
  int result = 0;
  for (size_t i = 0; i < left.size(); i++) {
    result += std::abs(left[i]-right[i]);
  }
  return result;
}


int calc_similarity_score(
  const std::vector<int>& left,
  const std::vector<int>& right
) {
  int result = 0;
  for (const int& target : left) {
    int n_items = std::count(right.begin(), right.end(), target);
    result += n_items * target;
    
  }
  return result;
}



int main() {
  // const char* filename = "test_data.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);
  std::vector<int> left, right;
  
  if (!file) {
    std::cerr << "Can't read the file : " << filename << std::endl;
    return -1;
  }

  while(file) {
    std::string line;
    std::getline(file, line);
    if (line.empty()) continue;
    
    std::stringstream ss;
    ss << line;
    
    int t;
    ss >> t;
    left.push_back(t);
  
    ss >> t;
    right.push_back(t);
  }

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());
  
  int d = calc_distance_score(left, right);
  std::cout << "distance: " << d << std::endl;

  int s = calc_similarity_score(left, right);
  std::cout << "similarity: " << s << std::endl;


  // std::cout << "Left\n";
  // for(auto e : left) {
  //   std::cout << e << " ";
  // }
  // std::cout << "\n";
  // std::cout << "Right\n";
  // for(auto e : right) {
  //   std::cout << e << " ";
  // }

  return 0;
}

