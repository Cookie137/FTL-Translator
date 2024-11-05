#ifndef TRANSLATOR_LEXICAL_ANALYZER_H
#define TRANSLATOR_LEXICAL_ANALYZER_H

#include "token.h"
#include "trie.h"
#include <vector>

class LexicalAnalyzer {
 public:
    LexicalAnalyzer(std::string keywords_file);
    std::vector<Token> Tokenize(std::string program_file);

 private:
    Trie keywords_trie_;
};

#endif //TRANSLATOR_LEXICAL_ANALYZER_H
