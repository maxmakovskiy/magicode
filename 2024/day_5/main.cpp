#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <utility>


struct Rule {
  int left;
  int right;
};

bool 
is_correct(
  const std::vector<int>& update,
  const std::vector<Rule>& rules
) {
  for (const Rule& r : rules) {
    auto l_pos = std::find(update.begin(), update.end(), r.left);
    auto r_pos = std::find(update.begin(), update.end(), r.right);

    if (l_pos == update.end() || r_pos == update.end()) {
      continue;
    }

    if (l_pos >= r_pos) {
      return false;
    }
  }
  return true;
}

bool is_rule_correct(
  const std::vector<int>& update,
  const Rule& rule
) {
  auto l_pos = std::find(update.begin(), update.end(), rule.left);
  auto r_pos = std::find(update.begin(), update.end(), rule.right);

  bool nothing_found = (l_pos == update.end()) || (r_pos == update.end());
  
  return (std::distance(l_pos, r_pos) > 0) || nothing_found;
}


void 
part1(
  const std::vector<Rule>& rules, 
  const std::vector<std::vector<int>> updates
) {
  int res = 0;
  for (const auto& update : updates) {
    if (is_correct(update, rules)) {
      size_t i_middle = update.size() / 2;
      res += update[i_middle];
    }
  }
  std::cout << res << std::endl;
}


int extract_left(const Rule& r) {
  return r.left;
}

template <typename VecIter>
VecIter make_unique(VecIter begin, const VecIter end) {
  // create array of iterators
  // so we would have a posibility to preserve order in target array
  // but still have some room for the moves
  std::vector<VecIter> iters;
  for(VecIter i = begin; i != end; i++) {
    iters.push_back(i);
  }

  // let's sort VecIter's array depending on underlying values
  auto iter_less = [](const VecIter& lhs, const VecIter& rhs) {
    return *lhs < *rhs;
  };
  std::stable_sort(iters.begin(), iters.end(), iter_less);

  // let's throw away iterators that point to the redundant elements
  auto iter_equal = [](const VecIter& lhs, const VecIter& rhs) {
    return *lhs == *rhs;
  };
  auto last_unique = std::unique(iters.begin(), iters.end(), iter_equal);
  iters.erase(last_unique, iters.end());

  // idea: put in the begging only unique values based on sorted iterators 
  size_t j = 0;
  for (VecIter itr = begin; itr != end && j < iters.size(); itr++) {
    if (itr == iters[j]) {
      std::iter_swap(itr, begin);
      ++begin;
      ++j;
    }
  }
  
  // return new end
  return begin;
}


template<typename T>
size_t unique_in_order(std::vector<T>& vec) {
  std::set<T> seen;

  auto newEnd = std::remove_if(vec.begin(), vec.end(), 
    [&seen](const T& value) {
      if (seen.find(value) != std::end(seen))
          return true;
      seen.insert(value);
      return false;
    }
  );

  vec.erase(newEnd, vec.end());

  return vec.size();
}





void
part2(
  const std::vector<Rule>& rules, 
  const std::vector<std::vector<int>>& updates
) {
  std::map<int, std::set<int>> dependencies;

  for (const Rule& r : rules) {
    dependencies[r.left].insert(r.right);
  }


  int res = 0;

  for (auto update : updates) {
    if (!is_correct(update, rules)) {
      std::sort(update.begin(), update.end(), 
        [&dependencies](int lhs, int rhs) {
          return dependencies[lhs].count(rhs) > 0;
        });
      size_t i_middle = update.size() / 2;
      res += update[i_middle];
    }
    
  }

  std::cout << res << std::endl;
  

}


// void
// part2(
//   const std::vector<Rule>& rules, 
//   const std::vector<std::vector<int>>& updates
// ) {
//   std::vector<std::vector<int>> corrupted;
//   for (auto& update : updates) {
//     for (const auto& r : rules) {
//       if (!is_rule_correct(update, r)) {
//         corrupted.push_back(update);
//         break;
//       }
//     }
//   }
  
//   int res = 0;
//   for (auto& update : corrupted) {
//     std::vector<Rule> to_verify;
//     for (const auto& r : rules) {
//       auto l_pos = std::find(update.begin(), 
//         update.end(), r.left);
//       auto r_pos = std::find(update.begin(), 
//         update.end(), r.right);
//       if (l_pos != update.end() && r_pos != update.end()) {
//         to_verify.push_back(r);
//       }
//     }

//     // sort them - maybe

   
//     std::vector<int> sorted;
//     std::vector<int> keys;

//     std::transform(to_verify.cbegin(), to_verify.cend(), 
//       std::back_inserter(keys), extract_left);
//     // auto new_end = make_unique(keys.begin(), keys.end());
//     // keys.erase(new_end, keys.end());
//     unique_in_order(keys);
    
//     for (int k : keys) {
//       auto left_match = [k](Rule& rule) { return (rule.left) == k; };
      
//       auto p = std::find_if(to_verify.begin(), 
//         to_verify.end(), left_match);
      
//       std::vector<int> rightest;
//       while (p != to_verify.end()) {
//         rightest.push_back(p->right);
//         p = std::find_if(p+1, to_verify.end(), left_match);
//       }


//       auto pos_k = std::find(sorted.begin(), sorted.end(), k);
//       if (pos_k == sorted.end()) {
//         // search for where to put k, before its every right element
//         std::vector<std::pair<size_t,
//           std::vector<int>::iterator>> distances;

//         std::vector<int> to_append;
//         for (int r : rightest) {
//           auto pos = find(sorted.begin(), sorted.end(), r);
//           if (pos != sorted.end()) {
//             distances.push_back(std::make_pair(
//               std::distance(pos , sorted.end()),
//               pos
//             ));
//           } else {
//             to_append.push_back(r);
//           }
//         }
//         auto iter_max = std::max_element(distances.begin(),
//           distances.end(), 
//           [](auto& lhs, auto& rhs) {
//              return lhs.first < rhs.first;
//           }
//         );
//         if (iter_max != distances.end()) {
//           // could be a bug here
//           sorted.insert((iter_max->second), k);
//         } else {
//           // this case occurs only if sorted is empty
//           sorted.push_back(k);
//         }
//         for (int r : to_append) {
//           sorted.push_back(r);
//         }
//       } else {
//         // otherwise key is alredy in the 'sorted'
//         // so we should figure out where to put 'rightest'
//         for (int r : rightest) {
//           auto pos_r = std::find(sorted.begin(), sorted.end(), r);
//           if (pos_r != sorted.end()) {
//             auto d = std::distance(pos_k, pos_r);
//             if (d < 0) {
//               std::iter_swap(pos_k, pos_r);
//               pos_k = std::find(sorted.begin(), sorted.end(), k);
//             }
//           } else {
//             sorted.push_back(r);
//           }
//         }
//       }
//     }

//     // let's suppose that our sorted consists [31, 11, 15]
//     // but update = [11, 13, 5, 31, 15, 40]
//     // how to keep elements that aren't included in sorted on theirs places
//     // whereas force to change places of elements that are acutally included in sorted
//     // so update becomes
//     // [31, 13, 5, 11, 40]

//     for (size_t i = 0, j = 0; i < update.size(); ++i) {
//       auto pos = std::find(sorted.begin(), sorted.end(), update[i]);
//       if (pos != sorted.end()) {
//         update[i] = sorted[j];
//         ++j;
//       }
//     }

//     size_t i_middle = update.size() / 2;
//     res += update[i_middle];
    
//   }
  
//   std::cout << res << std::endl;
// }


int main() {
  // const char* rules_filename = "test_rules.txt";
  const char* rules_filename = "rules.txt";
  // const char* updates_filename = "test_updates.txt";
  const char* updates_filename = "updates.txt";

  std::ifstream file;

  file.open(rules_filename);
  if (!file) {
    std::cerr << "Cannot read RULES file : " << rules_filename << std::endl;
    return -1;
  }
 
  std::vector<Rule> rules;
  while (file) {
    std::string buf;
    std::getline(file, buf);
    
    if (buf.empty()) { continue; }

    auto pos = buf.find("|");
    Rule r;
    r.left = std::stoi(buf.substr(0, pos));
    r.right = std::stoi(buf.substr(pos+1));
    rules.push_back(r);
  }
  file.close();

  file.open(updates_filename);
  if (!file) {
    std::cerr << "Cannot read UPDATES file : " << updates_filename << std::endl;
    return -1;
  }

  std::vector<std::vector<int>> updates;
  while (file) {
    std::string buf;
    std::getline(file, buf);

    if (buf.empty()) { continue; }
    
    std::vector<int> update;
    size_t pos_beg = 0;
    size_t pos_end = buf.find(",");
    while(true) {
      if (pos_end == std::string::npos) {
        int t = std::stoi(buf.substr(pos_beg));
        update.push_back(t);
        break;
      } else {
        int t = std::stoi(buf.substr(pos_beg, pos_end-pos_beg));
        update.push_back(t);
      }
      
      pos_beg = pos_end + 1;
      pos_end = buf.find(",", pos_beg);
    }
    updates.push_back(update);
  }
  file.close();

  // part1(rules, updates);
  part2(rules, updates);
  
  return 0;
}
