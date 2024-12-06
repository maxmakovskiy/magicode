#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


int main() {
  const char* filename = "test_data.txt";
  // const char* filename = "data.txt";
  std::ifstream file(filename);

  std::vector<int> data;

  if (!file) {
    std::cerr << "Can't read the file : " << filename << std::endl;
    return -1;
  }

  while(file) {
    std::string buf;
    std::getline(file, buf);

    std::stringstream ss;
    ss << buf;

    std::string token;
    while(std::getline(ss, token, ',')) {
      int t = std::stoi(token);
      data.push_back(t);
    }
  }

  // const size_t n_days = 80;
  const size_t n_days = 256;

  for(size_t day_idx = 0; day_idx < n_days; day_idx++) {
    size_t n_new = 0;
    for(size_t i = 0; i < data.size(); i++) {
      data[i] -= 1;
      if (data[i] < 0) {
        data[i] = 6;
        n_new += 1;
      }
    }
    for(size_t j = 0; j < n_new; j++) {
      data.push_back(8);
    }
    
  }

  std::cout << "after " << n_days << " days we have " << data.size() << " fish" << std::endl;
  
  
  
  return 0;
}
