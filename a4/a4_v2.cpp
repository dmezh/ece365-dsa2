#include <iostream>

#include <cstring>
#include <string>

#include "util.h"

bool solve(const char *wA, const char *wB, const char *target, unsigned x, unsigned y);
void print_map(const char *target);
std::string get_solution(const char *wA, const char *wB, const char *target);
void reset_solver();

int main() {
    auto input = open_file<std::ifstream>("input");
    auto output = open_file<std::ofstream>("output");

    std::string wA, wB, target;

    while (std::getline(input, wA) &&
            std::getline(input, wB) &&
            std::getline(input, target)) {

        bool m = solve(wA.c_str(), wB.c_str(), target.c_str(), 0, 0);
        
        //print_map(target.c_str());

        if (m)
            output << get_solution(wA.c_str(), wB.c_str(), target.c_str()) << "\n";
        else
            output << "*** NOT A MERGE ***\n";

        reset_solver();
    }
}

char map[3002][3002] = {0};

void reset_solver() {
    std::cout << "clearing: " << sizeof(map) << "\n";
    memset(map, 0, sizeof(map));
}

std::string get_solution(const char *wA, const char *wB, const char *target) {
    std::string sol;
    unsigned x = 0, y = 0;

    for (size_t i = 0; i < strlen(target); i++)
    {
        switch (map[x][y]) {
            case 'A':
                sol += toupper(*wA);
                wA++;
                x++; y++;
                break;
            case 'B':
                sol += *wB;
                wB++;
                x++; y++;
                break;
            case '%':
            case 'a':
                sol += toupper(*wA);
                wA++;
                x++;
                break;
            case 'b':
                sol += *wB;
                wB++;
                y++;
                break;
            default:
                std::cerr << "Encountered unexpected char " << map[x][y] << " during solution search at " << x << ", " << y << "\n";
                std::cerr << "Surrounding: left: " << map[x-1][y] << ", right: " << map[x+1][y] << ", above: " << map[x][y-1] << ", below: " << map[x][y+1] << ", diagleft: " << map[x-1][y-1] << "\n";
                return "*** NOT A MERGE ***";
//              abort();
        }
    }

    return sol;
}

// Algorithm:
// 
// 1. Start at 0, 0.
// 
// 2. Check location for matches in words
//   1. Match one word only: set square to that word, move diagonal, increment word
//   2. Match two words: Split - A branch to the right, B branch down (goto 2)
//   3. Match no words: mark as no solution

bool solve(const char *wA, const char *wB, const char *target, unsigned x, unsigned y) {
    if (!*target)
        return true;

    bool match_A = *wA && *wA == *target;
    bool match_B = *wB && *wB == *target;

    if (map[x][y]) {
        if (map[x][y] == 'a' || map[x][y] == 'b' || map[x][y] == 'A' || map[x][y] == 'B')
            return true;
        return false;
    }

    if (match_A && match_B) {
        map[x][y] = 'i';
        bool a = solve(wA+1, wB, target+1, x+1, y);
        bool b = solve(wA, wB+1, target+1, x, y+1);

        if (a && b) {
            map[x][y] = '%';
            return true;
        } else if (a) {
            map[x][y] = 'a';
            return true;
        } else if (b) {
            map[x][y] = 'b';
            return true;
        } else {
            map[x][y] = 'x';
            return false;
        }
    } else if (match_A) {
        map[x][y] = 'A';
        bool r = solve(wA+1, wB, target+1, x+1, y+1);
        return r;
    } else if (match_B) {
        map[x][y] = 'B';
        return solve(wA, ++wB, ++target, ++x, ++y);
    } else {
        map[x][y] = 'X';
        return false;
    }

    abort();
}

void print_map(const char *target) {
    std::cout << "  ";
    for (size_t i = 0; i < strlen(target); i++) {
        std::cout << " " << target[i];
    }
    std::cout << "\n";
    
    for (size_t i = 0; i < strlen(target); i++) {
        std::cout << target[i] << " "; 
        for (size_t j = 0; j < strlen(target); j++) {
            char c = map[j][i];
            std::cout << ' ' << (c ? c : ' ');
        }
        std::cout << "\n";
    }

}

