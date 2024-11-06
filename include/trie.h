#ifndef TRANSLATOR_TRIE_H
#define TRANSLATOR_TRIE_H

#include <fstream>
#include <string>
#include <set>
#include <unordered_map>

class Trie {
 public:
    Trie();
    ~Trie();
    void AddString(std::string& str);
    bool Contains(std::string& str);

 private:
    struct Node {
        std::unordered_map<char, Node*> to;
        bool is_terminal = false;
    };

    void Delete(Node* v);
    Node* root_;
    std::set<std::string> st_;
};

#endif //TRANSLATOR_TRIE_H
