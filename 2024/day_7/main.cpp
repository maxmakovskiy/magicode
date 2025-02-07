#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using ll = unsigned long long;

struct Equation {
  ll result;
  std::vector<ll> operands;
};

std::ostream& operator<<(std::ostream& os, const Equation& eq) {
  os << eq.result << ": ";
  for (ll operand : eq.operands) {
    os << operand << " ";
  }
  return os;
}

std::vector<std::string>
generate_ops(int n_ops) {
  std::vector<std::string> res;
  res.emplace_back(n_ops, '+');
  res.emplace_back(n_ops, '*');
  
  for (int n_muls = 1; n_muls < n_ops; ++n_muls) {
    std::string variation(n_ops, '+');
    for (int i = 0; i < n_muls; ++i) {
      variation[i] = '*';
    }
    std::sort(variation.begin(), variation.end());
    do {
      res.push_back(variation);
    } while(std::next_permutation(
      variation.begin(), variation.end()));
    
  }

  return res;
}


ll concat_numbers(ll lhs, ll rhs) {
  int n_digits_rhs = 0;
  for(ll test = rhs; test; test /= 10) {
    n_digits_rhs += 1;
  }
  lhs *= std::pow(10, n_digits_rhs);

  return lhs + rhs;
}


bool is_solvable(const Equation& eq) {
  int n_ops = eq.operands.size()-1;
  std::vector<std::string> ops = generate_ops(n_ops);

  for (const std::string& ops_set : ops) {
    ll test = eq.operands[0];
    for (size_t i = 0; i < n_ops; ++i) {
      if (ops_set[i] == '+') {
        test += eq.operands[i+1];
      } else {
        test *= eq.operands[i+1];
      }
      if (test > eq.result) {
        break;
      }
    }
    
    if (test == eq.result) {
      // std::cout << "For the equations : "
      //           << eq << "\n>>> "
      //           << ops_set <<  std::endl;
      
      return true;
    }
  }

  return false;
}


bool is_correct(const Equation& eq, ll operation) {
  ll res = eq.operands.front();
  for (size_t i = 1; i < eq.operands.size(); ++i) {
    if (operation % 3 == 2) {
      res += eq.operands[i];
    } else if (operation % 3 == 1) {
      res *= eq.operands[i];
    } else {
      res = concat_numbers(res, eq.operands[i]);
    }
    if (res > eq.result) {
      return false;
    }
    operation /= 3;
  }
  return res == eq.result;
}


bool is_solvable2(const Equation& eq, ll n_ops) {
  for (ll op = 0; op < n_ops; ++op) {
    if (is_correct(eq, op)) {
      return true;
    }
  }
  return false;
}



int main() {
  using namespace std;

  // const char* filename = "test_data.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Cannot read the file : " << filename << std::endl;
    return -1;
  }

  vector<Equation> equations;
  
  string buf;
  while (getline(file, buf)) {
    if (buf.empty()) { continue; }

    auto pos_delim = buf.find(":");
    Equation eq;
    eq.result = stoull(buf.substr(0, pos_delim));

    stringstream ss;
    ss << buf.substr(pos_delim+1);
    ll temp;
    while (ss >> temp) {
      eq.operands.push_back(temp);
    }
    equations.push_back(eq);
  }

  ll part_1 = 0;
  for (const Equation& eq : equations) {
    if (is_solvable(eq)) {
      part_1 += eq.result;
    }
  }
  std::cout << "Part 1: What is the sum of the test values\n" 
            << "from just the equations that could possibly be true? "
            << part_1 << std::endl;
  
  ll part_2 = 0;
  for (const Equation& eq : equations) {

    // ll n_ops = 1;
    // for (size_t i = 0; i < eq.operands.size(); ++i) {
    //   n_ops *= 3;
    // }

    ll n_ops = std::pow(3, eq.operands.size());
    
    // guided by:
    // https://gist.github.com/NXE212/1fd1e7172b576a098951e86dad0235fb
    if (is_solvable2(eq, n_ops)) {
      part_2 += eq.result;
    }
  }
  std::cout << "Part 2: What is the sum of the test values\n"
            << "with the new concatenation operator? "
            << part_2 << std::endl; 

  
  return 0;
}
