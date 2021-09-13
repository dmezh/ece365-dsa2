#ifndef UTIL_H
#define UTIL_H

#include <cstring>
#include <iostream>
#include <string>

// get file path input from the user
std::string get_path(std::string_view file_type);

// open input/output file with error checking, reporting, and retry
// T should be either std::ifstream or std::ofstream
template <typename T>
T open_file(std::string_view file_type) {
    T f;
    while (true) {
        f = T(get_path(file_type));
        if (f.is_open())
            return f;
        else
            std::cerr << "Error opening " << file_type << " file: " << strerror(errno) << '\n';
    }
}

#endif // UTIL_H