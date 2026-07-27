#pragma once

#include <functional>
#include "types.h"

template<typename T>
struct BspNode {
    T data;
    BspNode* leftChild;
    BspNode* rightChild;

    bool isLeaf = true;

    void Traverse(std::function<int(BspNode<T>*)> visitor) {
        s32 decision = visitor(this);

        if (isLeaf || decision == 0) {
            return;
        }
        if (decision == 1 && leftChild) {
            leftChild->Traverse(visitor);
        }
        if (decision == 2 && rightChild) {
            rightChild->Traverse(visitor);
        }
    }
};
