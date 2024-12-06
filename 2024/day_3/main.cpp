#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

// Having line:
// xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))
// we should parse all correct (uncorrupted) 'mul(_, _)'
// then evaluate each and add up together
// 2*4 + 5*5 + 11*8 + 8*5 = 161 


// size_t try_exract_int(const std::string& src, size_t pos, int& dst) {
//   size_t i = pos;
//   for (; (bool)isdigit(src[i]) && i < 3; i++) {}
//   return pos;
// }

enum class InstrType {
  MUL, DO, DONT
};

struct Instruction {
  InstrType type = InstrType::MUL;
  int op1 = 0;
  int op2 = 0;
  size_t pos = 0;
};

bool 
lth_instrs(
  const Instruction& lhs,
  const Instruction& rhs
) {
  return lhs.pos < rhs.pos;
}

int main() {
  // const char* filename = "test_data.txt";
  // const char* filename = "test_data_2.txt";
  // const char* filename = "test_data_3.txt";
  const char* filename = "data.txt";
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Can not read the given file : " << filename << std::endl;
    return -1;
  }


  // PART 2
  int acc = 0;

  std::string program;
  
  while (file) {
    std::string buffer;
    std::getline(file, buffer);
    program.append(buffer);
  }

  const char* token_mul = "mul(";
  const char* token_do = "do()";
  const char* token_dont = "don't()";

  std::vector<Instruction> instructions;
  
  size_t pos_mul = program.find(token_mul);

  // find all mul
  while (pos_mul != std::string::npos) {
    const size_t op1_begin = pos_mul + 4;
    size_t i = op1_begin; // so, "mul(" has length 4
    for (; (bool)isdigit(program[i]) && i < (3+op1_begin); i++) {}

    if (i == op1_begin) { 
      pos_mul = program.find(token_mul, 4 + pos_mul);
      continue; 
    }
    size_t op1_token_len = i - op1_begin;
    int operand_1 = std::stoi(program.substr(op1_begin, op1_token_len));

    if (program[i] != ',') { 
      pos_mul = program.find(token_mul, 4 + op1_token_len + pos_mul);
      continue; 
    }

    i += 1;
    const size_t op2_begin = i;
    for (; (bool)isdigit(program[i]) && i < (3+op2_begin); i++) {}
    if (i == op2_begin) { 
      pos_mul = program.find(token_mul, 5 + op1_token_len + pos_mul);
      continue; 
    }
 
    size_t op2_token_len = i - op2_begin;
    int operand_2 = std::stoi(program.substr(op2_begin, op2_token_len));

    if (program[i] != ')') {
      pos_mul = program.find(token_mul, 5 + op1_token_len + op2_token_len + pos_mul);
      continue;
    }

    instructions.push_back({
      .type=InstrType::MUL,
      .op1=operand_1,
      .op2=operand_2,
      .pos=pos_mul 
    });

    pos_mul = program.find(token_mul,
      (6 + op1_token_len + op2_token_len) + pos_mul);
  }

  // find all do
  size_t pos_do = program.find(token_do);
  const static size_t do_str_shift = 4;
  while (pos_do != std::string::npos) {
    instructions.push_back({
      .type=InstrType::DO,
      .pos=pos_do 
    });
    pos_do = program.find(token_do, do_str_shift + pos_do);
  }

  // find all do
  size_t pos_dont = program.find(token_dont);
  const static size_t dont_str_shift = 7;
  while (pos_dont != std::string::npos) {
    instructions.push_back({
      .type=InstrType::DONT,
      .pos=pos_dont 
    });
    pos_dont = program.find(token_dont, do_str_shift + pos_dont);
  }

  std::sort(instructions.begin(), instructions.end(), lth_instrs);
  
  bool mul_on = true;
  for (const auto& instr : instructions) {
    if (instr.type == InstrType::DO) {
      mul_on = true;
      continue;
    } 
    if (instr.type == InstrType::DONT) {
      mul_on = false;
      continue;
    }
    if (mul_on) {
      acc += (instr.op1 * instr.op2);
    } 
  }


  // ////////////////////////////////////////
  // PART 1
  // ////////////////////////////////////////
  // int acc = 0;
  // while (file) {
  //   std::string buffer;
  //   std::getline(file, buffer);

  //   // need to parse:
  //   // 1. find 'mul('
  //   // 2. verify that right after we have 1-3 digits number
  //   // 3. then ','
  //   // 4. another 1-3 digits number
  //   // 5. all this ends with ')'
  //   const char* token_mul = "mul(";
  //   size_t pos = buffer.find(token_mul);
  //   while (pos != std::string::npos) {
  //     // test for step 2
  //     const size_t op1_begin = pos + 4;
  //     size_t i = op1_begin; // so, "mul(" has length 4
  //     for (; (bool)isdigit(buffer[i]) && i < (3+op1_begin); i++) {}

  //     // it means there is no 1st operand hence it's corrupted
  //     if (i == op1_begin) { 
  //       pos = buffer.find(token_mul, 4 + pos);
  //       continue; 
  //     }
  //     size_t op1_token_len = i - op1_begin;
  //     int operand_1 = std::stoi(buffer.substr(op1_begin, op1_token_len));

  //     // step 3
  //     // if there is no comma right after, so it's corrupted
  //     if (buffer[i] != ',') { 
  //       pos = buffer.find(token_mul, 4 + op1_token_len + pos);
  //       continue; 
  //     }

  //     // step 4
  //     i += 1;
  //     const size_t op2_begin = i;
  //     for (; (bool)isdigit(buffer[i]) && i < (3+op2_begin); i++) {}
  //     // it means there is no 1st operand hence it's corrupted
  //     if (i == op2_begin) { 
  //       pos = buffer.find(token_mul, 5 + op1_token_len + pos);
  //       continue; 
  //     }
     
  //     size_t op2_token_len = i - op2_begin;
  //     int operand_2 = std::stoi(buffer.substr(op2_begin, op2_token_len));

  //     // step 5
  //     if (buffer[i] != ')') {
  //       pos = buffer.find(token_mul, 5 + op1_token_len + op2_token_len + pos);
  //       continue;
  //     }

  //     acc += (operand_1 * operand_2);

  //     // "mul(" -> 4 +
  //     // op1_token_len + 1 for ',' + op2_token_len + 1 for ')'
  //     // 6 + op1 + op2
  //     pos = buffer.find(token_mul, (6 + op1_token_len + op2_token_len) + pos);
  //   }
  // }

  std::cout << acc << std::endl;
  
	
  return 0;
}
