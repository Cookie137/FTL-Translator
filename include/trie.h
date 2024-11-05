#ifndef TRANSLATOR_TRIE_H
#define TRANSLATOR_TRIE_H

#include <fstream>
#include <string>
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
};

#endif //TRANSLATOR_TRIE_H
