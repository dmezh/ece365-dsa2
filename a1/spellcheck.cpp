#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "hash.h"
#include "util.h"

#define MAX_WORD_LENGTH 20

void process_dictionary(std::ifstream& dict, hashTable& ht) {
    std::string line;
    while (std::getline(dict, line)) {
        if (std::regex_search(line, std::regex("[^A-Za-z0-9-\']"))) {
            continue; // just ignore invalid word
        }

        for (char& c : line) {
            c = std::tolower(c);
        }

        if (ht.insert(line) == 2) {
            std::cerr << "Error allocating memory; exiting" << "\n";
            abort();
        }
    }
}

void process_doc(std::ifstream& file, std::ofstream& output, hashTable& ht) {
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        line_count++;

        for (char& c : line) {
            c = std::tolower(c);
        }

        line = std::regex_replace(line, std::regex("[^a-z0-9-\']"), "@");

        std::stringstream words(line);

        std::string word;
        while (getline(words, word, '@')) {
            if (word == "") continue;

            if (std::regex_search(word, std::regex("[0-9]"))) continue;

            if (word.length() > MAX_WORD_LENGTH) {
                output << "Long word at line " << line_count << 
                    ", starts: " << word.substr(0, MAX_WORD_LENGTH) << "\n";
                continue;
            }
            
            if (!ht.contains(word)) {
                output << "Unknown word at line " << line_count << ": " << word << "\n";
                continue;
            }

        }
    }
}

int main() {
    hashTable ht(1000);

    auto dict = open_file<std::ifstream>("dictionary");
    auto doc = open_file<std::ifstream>("document");
    auto output = open_file<std::ofstream>("output");

    std::clock_t dict_start = std::clock();
    process_dictionary(dict, ht);
    std::clock_t dict_end = std::clock();
    std::cout << "Total time (in seconds) to load dictionary: " <<
        static_cast<double>(dict_end - dict_start) / CLOCKS_PER_SEC << "\n";

    std::clock_t doc_start = std::clock();
    process_doc(doc, output, ht); 
    std::clock_t doc_end = std::clock();
    std::cout << "Total time (in seconds) to check document: " <<
        static_cast<double>(doc_end - doc_start) / CLOCKS_PER_SEC << "\n";
}
