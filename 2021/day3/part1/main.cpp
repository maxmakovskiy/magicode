#include <iostream>
#include <fstream>
#include <string>
//#include <vector>


int main() {
    const std::string filename = "test_data.txt";
    std::ifstream file {filename};

    if (!file.is_open()) {
        std::cerr << "Impossible to open a file: " << filename << std::endl;
        return -1;
    }

    while(!file.eof()) {
        std::string buffer;
        std::getline(file, buffer);
        std::cout << buffer << std::endl;
    }

    return 0;
}
