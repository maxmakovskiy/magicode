// 199 (N/A - no previous measurement)
// 200 (increased)
// 208 (increased)
// 210 (increased)
// 200 (decreased)
// 207 (increased)
// 240 (increased)
// 269 (increased)
// 260 (decreased)
// 263 (increased)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>


int str_to_int(const std::string& source) {
  int res = 0;
  auto l = source.length();
  const int ASCII_SHIFT = 48;
  int j = 1;
  for (const char digit : source) {
    // 231
    // l = 3
    // i = 0
    // pow(10, l-1 = 2) = 200
    // pow(10, l-2 = 1) = 20
    // pow(10, l-3 = 0) = 1
    int power = std::pow(10, l-j);
    res += power*(digit-ASCII_SHIFT);
    j += 1;
  }

  return res;
}


int main() {
  // std::string filename = "data.txt";
  std::string filename = "full_data.txt";
  std::vector<int> data {};
  std::ifstream file {filename};

  if (!file.is_open()) {
    std::cout << "Failed to open file: " << filename << std::endl;
    return -1;
  }

  while(!file.eof()) {
    std::string b;
    file >> b;
    if (!b.empty()) {
      data.push_back(str_to_int(b));
    }
  }

  int counter = 0;
  size_t iter = 0;
  int prev = data[iter++];
  for (; iter < data.size(); iter++) {
    if (prev < data[iter]) {
      counter++;
    }
    prev = data[iter];
  }
  
  std::cout << counter << std::endl;
 
  return 0;
}

