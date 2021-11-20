#include "heap.h"

#include <iostream>

heap::heap(int capacity) : mapping(capacity*2) {
    data.reserve(capacity + 1);
    currentSize = 0;
}

int heap::insert(std::string_view id, int key, void *pv) {
    if (currentSize+1 >= data.capacity()) return 1;
    if (mapping.contains(id)) return 2;
    
    Node n;
    n.id = id;
    n.key = key;
    n.pData = pv;

    data[++currentSize] = n;
    if (mapping.insert(id, &data[currentSize])) {
        // should not be possible to get return code 1, because we checked
        // contains() earlier.
        std::cerr << "Error allocating memory for rehash! Exiting.\n";
        abort();
    }

    percolateUp(currentSize);

    return 0;
}

int heap::setKey(std::string_view id, int key) {
    bool exists; void* v;

    std::tie(exists, v) = mapping.getPointer(id);
    if (!exists) return 1;

    Node *n = static_cast<Node *>(v);
    int old_key = n->key;
    n->key = key;

    if (key > old_key)
        percolateDown(getPos(n));
    else if (key < old_key)
        percolateUp(getPos(n));

    return 0;
}

void heap::percolateUp(int hole) {
    data[0] = data[hole];

    for (; data[0].key < data[hole / 2].key; hole /= 2) {
        mapping.setPointer(data[hole / 2].id, &data[hole]);
        data[hole] = data[hole / 2];
    }

    data[hole] = data[0];
    mapping.setPointer(data[hole].id, &data[hole]);
}

void heap::percolateDown(int hole) {
    int child;

    Node tmp = data[hole];

    for (; hole * 2 <= currentSize; hole = child) {
        child = hole * 2;
        if (child != currentSize && data[child+1].key < data[child].key) {
            child++;
        }
        if (data[child].key < tmp.key) {
            data[hole] = data[child];
            mapping.setPointer(data[hole].id, &data[hole]);
        }
        else {
            break;
        }
    }

    data[hole] = tmp;
    mapping.setPointer(data[hole].id, &data[hole]);
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    Node* n = &data[1];

    if (pId) *pId = n->id;
    if (pKey) *pKey = n->key;
    if (ppData) *(static_cast<void **>(ppData)) = n->pData;

    if (currentSize == 0) return 1;

    mapping.remove(data[1].id);
    data[1] = data[currentSize--];
    percolateDown(1);

    return 0;
}

int heap::remove(std::string_view id, int *pKey, void *ppData) {
    bool exists; void* v;
    std::tie(exists, v) = mapping.getPointer(id);

    if (!exists) return 1;

    Node *n = static_cast<Node*>(v);

    if (pKey) *pKey = n->key;
    if (ppData) *(static_cast<void **>(ppData)) = n->pData;
    
    setKey(id, data[1].key-1);

    deleteMin();
    return 0;
}

int heap::getPos(const heap::Node *pn) {
    return pn - &data[0];
}

void heap::dumptable() {
    for (int i = 1; i < currentSize + 1; i++) {
        Node *n = &data[i];
        std::cout << "ID: \"" << n->id << "\" Key: " << n->key << std::endl;
    }
}
