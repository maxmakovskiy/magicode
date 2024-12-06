#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


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
fix(
  std::vector<int>& update,
  const Rule& rule
) {
  auto l_pos = std::find(update.begin(), update.end(), rule.left);
  auto r_pos = std::find(update.begin(), update.end(), rule.right);
  std::iter_swap(l_pos, r_pos);
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

void
part2(
  const std::vector<Rule>& rules, 
  const std::vector<std::vector<int>>& updates
) {
  std::vector<std::vector<int>> corrupted;
  for (auto& update : updates) {
    for (const auto& r : rules) {
      if (!is_rule_correct(update, r)) {
        corrupted.push_back(update);
        break;
      }
    }
  }
  
  int res = 0;
  for (auto& update : corrupted) {
    std::vector<Rule> to_verify;
    for (const auto& r : rules) {
      auto l_pos = std::find(update.begin(), 
        update.end(), r.left);
      auto r_pos = std::find(update.begin(), 
        update.end(), r.right);
      if (l_pos != update.end() && r_pos != update.end()) {
        to_verify.push_back(r);
      }
    }

    // sort them - maybe
    // or just apply twice

    for (size_t i = 0; i < to_verify.size()*2; i++) {
      size_t j = i%to_verify.size();
      if (is_rule_correct(update, to_verify[j])) { continue; }

      auto l_pos = std::find(update.begin(), 
        update.end(), to_verify[j].left);
      auto r_pos = std::find(update.begin(), 
        update.end(), to_verify[j].right);
      std::iter_swap(l_pos, r_pos);
    }

    
    size_t i_middle = update.size() / 2;
    res += update[i_middle];
  }
  
  std::cout << res << std::endl;
}


int main() {
  const char* rules_filename = "test_rules.txt";
  // const char* rules_filename = "rules.txt";
  const char* updates_filename = "test_updates.txt";
  // const char* updates_filename = "updates.txt";

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
