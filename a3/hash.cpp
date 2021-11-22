#include "hash.h"

#include <iostream>
#include <string>
#include <tuple>

#define HASH_SEED 5381
#define MAX_LOAD_FACTOR 0.75

int hashTable::insert(std::string_view key, void *pv) {
    if (static_cast<float>(this->filled) / this->capacity > MAX_LOAD_FACTOR) {
        if (!this->rehash()) return 2;
    }
    
    bool exists; Hash hash;
    std::tie(exists, hash) = this->findPos(key);
    if (exists)
        return 1;

    hashItem *i = &this->data[hash];
    i->key = key;
    i->isOccupied = true;
    i->isDeleted = false;
    i->pv = pv;

    this->filled++;

    return 0;
}

bool hashTable::rehash() {
    Hash newSize = getPrime(this->capacity * 2);

    try {
        std::vector<hashItem> newData(newSize);
        std::vector<hashItem> oldData = this->data;
        this->data.swap(newData);
        this->capacity = newSize;

        for (auto item : oldData) {
            if (item.isOccupied && !item.isDeleted) {
                this->insert(item.key, item.pv);
            }
        }
    } catch (std::bad_alloc const&) {
        return false;
    }

    return true;
}

std::pair<bool, Hash> hashTable::findPos(std::string_view key) {
    Hash hash = this->hash(key);

    while (this->data[hash].isOccupied && !this->data[hash].isDeleted) { // linear probing
        if (this->data[hash].key == key)
            return {true, hash};

        if (++hash >= this->capacity) // wrap around
            hash = 0;
    }

    return {false, hash};
}

hashTable::hashTable(unsigned size) {
    this->capacity = getPrime(size);
    this->data.resize(this->capacity);
}

unsigned hashTable::getPrime(int size) {
    static const int primes[] = {
        7,11,19,31,53,71,109,163,233,547,877,1229,1993,4001,7001,
        10181,17393,26407,38923,52363,66107,84673,99551,121441,200033,
        300497,526441,1050139,2711899,5630279,8994599,12353267,15485863
    };

    for (auto prime : primes)
        if (prime > size)
            return prime;

    std::cerr << "Error: Size of hash table exceeds maximum listed prime!" << "\n";
    abort(); // maybe generate more primes on the spot instead of giving up
}

// http://www.cse.yorku.ca/~oz/hash.html (djb2)
Hash hashTable::hash(std::string_view key) {
    Hash hash = HASH_SEED;
    for (char c : key)
        hash = ((hash << 5) + hash) + c;
    return hash % this->capacity;
}

bool hashTable::contains(std::string_view key) {
    bool exists;
    std::tie(exists, std::ignore) = this->findPos(key);
    return exists;
}

bool hashTable::setPointer(std::string_view key, void *pv) {
    bool exists;
    Hash h;

    std::tie(exists, h) = this->findPos(key);
    if (!exists)
        return false;

    this->data[h].pv = pv;
    return true;
}

std::pair<bool, void*> hashTable::getPointer(std::string_view key) {
    bool exists;
    Hash h;

    std::tie(exists, h) = this->findPos(key);
    if (!exists)
        return {false, nullptr};

    return {true, this->data[h].pv};
}

bool hashTable::remove(std::string_view key) {
    bool exists; Hash h;
    std::tie(exists, h) = findPos(key);
    if (!exists) return false;

    data[h].isDeleted = true;
    filled--;
    return true;
}
