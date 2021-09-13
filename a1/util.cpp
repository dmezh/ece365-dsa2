#include "util.h"

#include <cstring>
#include <fstream>
#include <iostream>

std::string get_path(std::string_view file_type) {
    std::string path;
    std::cout << "Please enter the name of the " << file_type << " file: ";
    std::cin >> path;
    return path;
}
