#ifndef TRANSLATOR_TRIE_H
#define TRANSLATOR_TRIE_H

#include <fstream>
#include <string>
#include <set>
#include <unordered_map>

class Trie {
 public:
    Trie();
    void AddString(std::string& str);
    bool Contains(std::string& str);

 private:
    struct Node {
        std::unordered_map<char, Node*> to;
        bool is_terminal = false;
    };
    Node* root_;
};

#endif //TRANSLATOR_TRIE_H
