#include "HCTree.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>

// Helper function to delete the tree recursively
void deleteTree(HCNode* node) {
    if (node != nullptr) {
        deleteTree(node->c0);
        deleteTree(node->c1);
        delete node;
    }
}

HCTree::~HCTree() {
    deleteTree(root);
}

void HCTree::build(const vector<int>& freqs) {

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (size_t i = 0; i < freqs.size(); ++i) {
        if (freqs[i] > 0) {
            HCNode* newNode = new HCNode(static_cast<int>(freqs[i]), static_cast<unsigned char>(i));
            leaves[i] = newNode;
            pq.push(newNode);
        }
    }
    while (pq.size() > 1) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();
        HCNode* newNode = new HCNode(c0->count + c1->count, 0);
        c0->p = newNode;
        c1->p = newNode;
        newNode->c0 = c0;
        newNode->c1 = c1;
        pq.push(newNode);
    }
    if (!pq.empty()) {
        root = pq.top();
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    // Find the leaf node corresponding to the symbol
    HCNode* node = leaves[symbol];

    // Use a stack to store the bits in reverse order
    std::stack<int> bits;

  while (node != nullptr) {
        if (node->p != nullptr && node->p->c0 == node) {
            bits.push(0); 
        } else if (node->p != nullptr && node->p->c1 == node) {
            bits.push(1);
        }
        node = node->p;
    }
    while (!bits.empty()) {
        out.write_bit(bits.top());
        bits.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream& in) const {
    // Start at the root of the tree
    HCNode* node = root;

    // Traverse the tree until a leaf node is reached
    while (node->c0 != nullptr && node->c1 != nullptr) {
        int bit = in.read_bit();
        if (bit == 0) {
            // Move to the left child for '0' bit
            node = node->c0;
        } else {
            // Move to the right child for '1' bit
            node = node->c1;
        }
    }

    // Return the symbol stored in the leaf node
    return node->symbol;
}