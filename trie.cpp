#include "trie.h"

Trie::Trie() : root_(new Node()) {}

Trie::~Trie() {
    Delete(root_);
    root_ = nullptr;
}

void Trie::AddString(std::string& str) {
    Node* v = root_;
    for (char c : str) {
        if (!v->to[c]) {
            v->to[c] = new Node();
        }
        v = v->to[c];
    }
    v->is_terminal = true;
}

bool Trie::Contains(std::string& str) {
    Node* v = root_;
    for (char c : str) {
        if (!v->to[c]) {
            return false;
        }
        v = v->to[c];
    }
    return true;
}

void Trie::Delete(Node* v) {
    for (auto& [c, u] : v->to) {
        Delete(u);
    }
    delete v;
}
