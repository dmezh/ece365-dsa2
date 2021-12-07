#include <iostream>

#include <ctime>
#include <cstring>

std::string repeat(std::string s, size_t len) {
    std::string r;

    while (r.length() <= len) {
        r += s;
    }

    return r;
}

std::string gen_random(const int len) {
    static const char alphanum[] =
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main() {
    int seed = std::time(nullptr);
    std::srand(seed);
    int random = std::rand();

    std::string A = "v";
    std::string B = "v";
    std::string cat, cat_lower;

    A = repeat(A, 950);
    B = repeat(B, 1950);

    A += "ababababab";
    B += "bababababa";

    A = gen_random(950);
    B = gen_random(1950);

    size_t len = A.length() + B.length();
    auto A_it = A.begin();
    auto B_it = B.begin();

    while (cat.length() < len) {
        if (*A_it && *B_it && *A_it == *B_it) {
            char c = *A_it++;
            cat += toupper(c);
            cat_lower += c;
            continue;
        }

        if (*A_it && ((random % 2))) {
            char c = *A_it++;
            cat += toupper(c);
            cat_lower += c;
        } else if (*B_it) {
            char c = *B_it++;
            cat += c;
            cat_lower += c;
        }
        random = std::rand();
    }

    std::cout << A << "\n";
    std::cout << B << "\n";
    std::cout << cat_lower << "\n";
    std::cerr << cat << "\n";
}
