/*********************************************
 * file:	~\code4b\dsets.cpp                *
 * remark: implementation of disjoint sets    *
 **********************************************/

#include <iostream>
#include <format>
#include <cassert>

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) : V(theSize + 1) {  // slot zero is not used
    assert(theSize > 0);
    init();
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
    for (auto i = 1; i < std::ssize(V); ++i) {
        V[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    assert(r != s);
    assert(r >= 1 && r <= std::ssize(V) - 1);
    assert(s >= 1 && s <= std::ssize(V) - 1);
    assert(V[r] < 0);
    assert(V[s] < 0);

    // Do Union by Size
        // V[r] and V[s] store the negative of the size of the set
    int sizeR = -V[r];
    int sizeS = -V[s];

    if (sizeR >= sizeS) {
        // Attach smaller tree (s) to root of larger tree (r)
        V[s] = r;
        V[r] -= sizeS; // Update size of the new root
    }
    else {
        // Attach larger tree (r) to root of smaller tree (s)
        V[r] = s;
        V[s] -= sizeR; // Update size of the new root
    }
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= std::ssize(V) - 1);

    // simple find
    if (V[x] < 0) {
        return x;
    } else {
        // Recursively find the root and compress the path
        V[x] = find(V[x]);
        return V[x];
    }
}

// just in case ...
void DSets::print() const {
    std::cout << "\n";

    for (auto i = 1; i < std::ssize(V); i++) {
        std::cout << std::format("{:4}", i);
    }
    std::cout << "\n";

    for (auto u : V) {
        std::cout << std::format("{:4}", u);
    }
    std::cout << "\n";
}
