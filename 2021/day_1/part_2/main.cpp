#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <list>

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
        int power = static_cast<int>(std::pow(10, l-j));
        res += power*(digit-ASCII_SHIFT);
        j += 1;
    }

    return res;
}

int analyze(const std::vector<int>& data) {
    int counter = 0;
    size_t iter = 0;
    int prev = data[iter++];
    for (; iter < data.size(); iter++) {
        if (prev < data[iter]) {
            counter += 1;
        }
        prev = data[iter];
    }
    return counter;
}

std::vector<int> squish(const std::vector<int>& data) {
    std::list<int> input(data.begin(), data.end());
    const int WINDOW_SIZE = 3;
    std::vector<int> result;

    while(!input.empty() && input.size() >= WINDOW_SIZE) {
        int temp = 0;
        auto j = input.begin();
        for (int i = 0; i < WINDOW_SIZE; i++, j++) {
            temp += (*j);
        }
        input.erase(input.begin());
        result.push_back(temp);
    }

    return result;
}

int main() {
//    const std::string filename = "test_data.txt";
    const std::string filename = "full_data.txt";
    std::ifstream file(filename, std::fstream::in);
    std::vector<int> raw;

    if (!file.is_open()) {
        std::cerr << "Impossible to read file: " << filename << std::endl;
        return -1;
    }

    while(!file.eof()) {
        std::string buffer;
        file >> buffer;
        raw.push_back(str_to_int(buffer));
    }
    file.close();

    auto transformed = squish(raw);
    int result = analyze(transformed);

    std::cout << result << std::endl;

    return 0;
}
