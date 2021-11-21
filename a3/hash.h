#ifndef HASH_H
#define HASH_H

#include <optional>
#include <string>
#include <tuple>
#include <vector>

typedef unsigned int Hash;

class hashTable {

 public:

  // The constructor initializes the hash table.
  // Uses getPrime to choose a prime number at least as large as
  // the specified size for the initial size of the hash table.
  hashTable(unsigned size = 100);

  // Insert the specified key into the hash table.
  // If an optional pointer is provided,
  // associate that pointer with the key.
  // Returns 0 on success,
  // 1 if key already exists in hash table,
  // 2 if rehash fails.
  int insert(std::string_view, void *pv = nullptr);

  // Check if the specified key is in the hash table.
  // If so, return true; otherwise, return false.
  bool contains(std::string_view key);

  // Get the pointer associated with the specified key.
  // If the key does not exist in the hash table, return nullptr.
  // If an optional pointer to a bool is provided,
  // set the bool to true if the key is in the hash table,
  // and set the bool to false otherwise.
  std::pair<bool, void*> getPointer(std::string_view key);

  // Set the pointer associated with the specified key.
  // Returns 0 on success,
  // 1 if the key does not exist in the hash table.
  bool setPointer(std::string_view key, void *pv);

  // Delete the item with the specified key.
  // Returns true on success,
  // false if the specified key is not in the hash table.
  bool remove(std::string_view);

 private:

  // Each item in the hash table contains:
  // key - a string used as a key.
  // isOccupied - if false, this entry is empty,
  //              and the other fields are meaningless.
  // isDeleted - if true, this item has been lazily deleted.
  // pv - a pointer related to the key;
  //      nullptr if no pointer was provided to insert.
  class hashItem {
  public:
    std::string key {""};
    bool isOccupied {false};
    bool isDeleted {false};
    void *pv {nullptr};

    hashItem() = default;
  };

  unsigned capacity;   // The current capacity of the hash table.
  unsigned filled {0}; // Number of occupied items in the table.

  std::vector<hashItem> data; // The actual entries are here.

  // The hash function.
  Hash hash(std::string_view key);

  // Search for an item with the specified key.
  // Return a bool indicating whether the key was
  // found to exist in the table already,
  // and the hash (with linear probing if necessary)
  std::pair<bool, Hash> findPos(std::string_view key);

  // The rehash function; makes the hash table bigger.
  // Returns true on success, false if memory allocation fails.
  bool rehash();

  // Return a prime number at least as large as size.
  // Uses a precomputed sequence of selected prime numbers.
  static unsigned int getPrime(int size);
};

#endif // HASH_H
