#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>

// #define PRINT_INFO


std::ostream& operator<<(
  std::ostream& os,
  const std::vector<int>& v
) {
  for (int val : v) {
    if (val >= 0) {
      os << val;
    } else {
      os << ".";
    }
  }
  
  return os;
}

struct Index {
  int pos;
  int n;
};


int main() {
  using namespace std;
  
  // const char* filename = "test_data_1.txt";
  // const char* filename = "test_data_2.txt";
  // const char* filename = "test_data_3.txt";
  const char* filename = "data.txt";
  ifstream file(filename);

  if (!file) {
    cerr << "Cannot read this file : " << filename << endl;
    return -1;
  }

  string buffer;
  getline(file, buffer);

  vector<int> deciphered;

  bool is_file = true;
  int id = 0;

  vector<Index> files;
  vector<Index> spaces;
  
  for (char ch : buffer) {
    int n = ch - 48;
    int beginning = deciphered.size();
    if (is_file) {
      for (int i = 0; i < n; ++i) {
        deciphered.push_back(id);
      }
      id += 1;
      files.push_back(Index {
        .pos = beginning,
        .n   = n
      });
    } else {
      for (int i = 0; i < n; ++i) {
        deciphered.push_back(-1);
      }
      int end = deciphered.size()-1;
      spaces.push_back(Index{
        .pos = beginning,
        .n   = n
      });
    }
    is_file = !is_file;
  }

#ifdef PRINT_INFO
  cout << "Init: " << buffer << "\n"
       << "Recognized: " << deciphered << endl;
#endif  



/*
  // /////////////
  // PART 1
  int free_sp_itr = 0;
  for (int j = 0; j < deciphered.size(); ++j) {
    if (deciphered[j] == -1) {
      free_sp_itr = j;
      break;
    }
  }

  for (int i = deciphered.size()-1; i >= 0 && free_sp_itr < i; --i) {
    if (deciphered[i] < 0) { continue; }

    swap(deciphered[free_sp_itr], deciphered[i]);

    for (int j = free_sp_itr+1; j < deciphered.size(); ++j) {
      if (deciphered[j] < 0) {
        free_sp_itr = j;
        break;
      }
    }
  }
*/

  // /////////////////
  // PART 2

  for (int i_file = files.size()-1;
       i_file >= 0;
       --i_file)
  {
    const Index& f_index = files[i_file];
    
    int i_spaces = 0;
    Index sp_index = spaces[i_spaces];
    while (sp_index.n < f_index.n) {
      i_spaces += 1;
      if (i_spaces >= spaces.size()) { break; }
      sp_index = spaces[i_spaces];
    }

    if (i_spaces >= spaces.size() || 
        sp_index.pos >= f_index.pos) 
    { continue; }
    
    // 1. swap
    for (int i = 0;
         i < f_index.n; 
         ++i)
    {
      swap(
        deciphered[sp_index.pos+i],
        deciphered[f_index.pos+i]
      );
    }
    
    // 2. update indexes of current free space range
    files[i_file].pos = sp_index.pos;
    spaces[i_spaces].n -= f_index.n;
    spaces[i_spaces].pos += f_index.n;
  }

  
  
#ifdef PRINT_INFO
  cout << "Deciphered: " << deciphered << endl;
#endif  

/*
  // PART 1
  // checksum calculation
  long long checksum = 0;
  for (int block_i = 0; deciphered[block_i] != -1; ++block_i) {
    checksum += block_i * deciphered[block_i];
  }
*/

  // PART 2
  // checksum calculation
  long long checksum = 0;
  for (int pos = 0; pos < deciphered.size(); ++pos) {
    if (deciphered[pos] >= 0) {
      checksum += pos * deciphered[pos]; 
    }
  }

  cout << "Checksum: " << checksum << endl;


  
  return 0;
}
