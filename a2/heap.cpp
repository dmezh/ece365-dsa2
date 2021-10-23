#include "heap.h"

#include <iostream>

heap::heap(int capacity):mapping(capacity*2) {
    data.reserve(capacity + 1);
    currentSize = 0;
}

int heap::insert(std::string_view id, int key, void *pv) {
    if (currentSize+1 > data.capacity()) return 1;
    if (mapping.contains(id)) return 2;
    
    Node n;
    n.id = id;
    n.key = key;
    n.pData = pv;

    data[++currentSize] = n;
    if (mapping.insert(id, &data[currentSize]))
        abort(); // handle better

                    std::cout << "made it" << std::endl;

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

    if (key > old_key) {
        std::cout << "calling percdown" << std::endl;
        percolateDown(getPos(n));
    } else if (key < old_key) {
        std::cout << "calling percup" << std::endl;
        percolateUp(getPos(n));
    } else {
        std::cout << "calling NOTHING" << std::endl;
    }

    return 0;
}

void heap::percolateUp(int posCur) {
    data[0] = std::move(data[posCur]);
    for (; data[0].key < data[posCur / 2].key; posCur /= 2) {
        mapping.setPointer(data[posCur / 2].id, &data[posCur]);
        data[posCur] = std::move(data[posCur / 2]);
    }
    data[posCur] = std::move(data[0]);
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

void heap::percolateDown(int posCur) {
    int child;

    Node tmp = std::move(data[posCur]);

    for (; posCur * 2 < currentSize; posCur = child) {
        child = posCur * 2;
        if (child != currentSize && data[child+1].key < data[child].key)
            child++;
        if (data[child].key < tmp.key) {
            data[posCur] = std::move(data[child]);
            mapping.setPointer(data[posCur].id, &data[posCur]);
        }
        else
            break;
    }

    data[posCur] = std::move(tmp);
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    Node* n = &data[1];

    if (pId) *pId = n->id;
    if (pKey) *pKey = n->key;
    if (ppData) *(static_cast<void **>(ppData)) = n->pData;

    if (currentSize == 0) return 1;

    mapping.remove(data[1].id);
    data[1] = std::move(data[currentSize--]);
    mapping.setPointer(data[1].id, &data[1]);
    percolateDown(1);
    return 0;
}

int heap::remove(std::string_view id, int *pKey, void *ppData) {
    if (setKey(id, data[1].key-1))
        return 1;
    deleteMin(nullptr, pKey, ppData);
    return 0;
}

void heap::dumptable() {
    for (int i=1; i<currentSize+1; i++) {
        Node *n = &data[i];
        std::cout << "ID: \"" << n->id << "\" Key: " << n->key << std::endl;
    }
}

int heap::getPos(heap::Node *pn) {
    return pn - &data[0];
}
