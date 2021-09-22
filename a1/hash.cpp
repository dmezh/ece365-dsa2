#include "hash.h"

#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include <regex>

#define MAX_LOAD_FACTOR 0.75

// return type
int hashTable::insert(std::string_view key, void *pv) {
    if (static_cast<float>(this->filled) / this->capacity > MAX_LOAD_FACTOR) {
        if (!this->rehash()) return 2;
    }
    
    bool exists; Hash hash;
    std::tie(exists, hash) = this->findPos(key);
    if (exists)
        return 1;

    hashItem *i = &this->data.at(hash);
    i->key = key;
    i->isOccupied = true;

    this->filled++;

    return 0;
}

bool hashTable::rehash() {
    Hash newSize = getPrime(this->data.size() * 2);

    std::vector<hashItem> current = this->data;

    try {
        std::cout << "REHASHING" << std::endl;
        data.resize(newSize);
        this->capacity = newSize;

        for (auto item : this->data) {
            if (item.isOccupied) {
                this->data.at(this->hash(item.key)).key = item.key;
                this->data.at(this->hash(item.key)).isOccupied = true;
            }
        }
    } catch (std::bad_alloc) {
        std::cout << "REHASHING FAIL" << std::endl;
        return false;
    }

    std::cout << "REHASHING SUCCESS" << std::endl;
    return true;

}

std::pair<bool, Hash> hashTable::findPos(std::string_view key) {
    int hash = this->hash(key);

    while (this->data.at(hash).isOccupied) { // linear probing
        if (this->data.at(hash).key == key)
            return {true, hash};

        if (++hash >= this->capacity) // wrap around
            hash = 0;
    }

    return {false, hash};
}

hashTable::hashTable(int size) {
    this->capacity = getPrime(size);
    this->data.resize(this->capacity);
}

unsigned hashTable::getPrime(int size) {
    static const int primes[] = {
        2,11,19,31,53,71,109,163,233,547,877,1229,1993,4001,7001,
        10181,17393,26407,38923,52363,66107,84673,99551,121441,200033
    };

    for (auto prime : primes)
        if (prime > size)
            return prime;

    return primes[sizeof(primes)/sizeof(primes[0]) - 1];
}

// http://www.cse.yorku.ca/~oz/hash.html (djb2)
Hash hashTable::hash(std::string_view key) {
    Hash hash = 5381;
    for (char c : key)
        hash = ((hash << 5) + hash) + c;
    return hash % this->capacity;
}

bool hashTable::contains(std::string_view key) {
    bool exists;
    std::tie(exists, std::ignore) = this->findPos(key);
    return exists;
}

void process_dictionary(std::ifstream& dict, hashTable& ht) {
    std::string line;
    while (std::getline(dict, line)) {
        for (char& c : line) {
            c = std::tolower(c);
        }

        line = std::regex_replace(line, std::regex("[^a-z0-9-\']"), "@");

        std::stringstream words(line);

        std::string word;
        while (getline(words, word, '@')) {
            if (word == "") continue;
            ht.insert(word);
        }
    }
}

int main() {
    hashTable ht(1000);

    std::string dict_name, file_to_check_name, output_name;
    std::cout << "Dictionary name: ";
    std::cin >> dict_name;
    std::cout << '\n' << "Name of file to be spell-checked: ";
    std::cin >> file_to_check_name;
    std::cout << '\n' << "Name of output file: ";
    std::cin >> output_name;

    std::ifstream dict;
    dict.open(dict_name);

    process_dictionary(dict, ht);

    std::cout << ht.contains("hello") << std::endl;
    std::cout << ht.contains("hsr30") << std::endl;
    std::cout << ht.contains("doff") << std::endl;
    std::cout << ht.contains("hsr31") << std::endl;
}
