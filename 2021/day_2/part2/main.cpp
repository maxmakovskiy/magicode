#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cmath>


//forward 5
//down 5
//up 3

enum cmd {
    forward,
    down,
    up
};

int str_to_int(const std::string& source) {
    int res = 0;
    auto l = source.length();
    const int ASCII_SHIFT = 48;
    int j = 1;
    for (const char digit : source) {
        int power = std::pow(10, l-j);
        res += power*(digit-ASCII_SHIFT);
        j += 1;
    }

    return res;
}

std::pair<int, cmd> parse(const std::string& raw) {
    const auto FORWARD_SHIFT = 7+1;
    const auto DOWN_SHIFT = 4+1;
    const auto UP_SHIFT = 2+1;

    int forward_comp = raw.substr(0, FORWARD_SHIFT).compare("forward ");
    int down_comp = raw.substr(0, DOWN_SHIFT).compare("down ");
//    int up_comp = raw.compare(UP);

    std::string intermediate;

    cmd cmd_type;
    if (forward_comp == 0) {
        intermediate = raw.substr(FORWARD_SHIFT);
        cmd_type = forward;
    } else if (down_comp == 0) {
        intermediate = raw.substr(DOWN_SHIFT);
        cmd_type = down;
    } else {
        intermediate = raw.substr(UP_SHIFT);
        cmd_type = up;
    }

    auto num = str_to_int(intermediate);

    return std::make_pair(num, cmd_type);
}


int main() {
//    const std::string filename {"test_data.txt"};
    const std::string filename {"full_data.txt"};
    std::ifstream file {filename};

    if (!file.is_open()) {
        std::cerr << "Impossible to read a file: " << filename << std::endl;
        return -1;
    }

    int aim = 0;
    int depth = 0;
    int position = 0;

    while(!file.eof()) {
        std::string buffer;
        std::getline(file, buffer);
        auto r = parse(buffer);
        if (r.second == forward) {
            position += r.first;
            depth += (r.first * aim);
        } else if (r.second == down) {
            aim += r.first;
        } else {
            aim -= r.first;
        }
    }

    std::cout << "AIM: " << aim << "\n";
    std::cout << "DEPTH: " << depth << "\n";
    std::cout << "POSITION: " << position << "\n";
    std::cout << "DEPTH x POSITION = " << (depth*position) << std::endl;

    return 0;
}
