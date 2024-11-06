#ifndef TRANSLATOR_LEXICAL_ANALYZER_H
#define TRANSLATOR_LEXICAL_ANALYZER_H

#include "token.h"
#include "trie.h"
#include <vector>

class LexicalAnalyzer {
 public:
    LexicalAnalyzer(std::string keywords_file);
    ~LexicalAnalyzer();
    std::vector<Token> Tokenize(std::string program_file);

 private:
    bool IsAlpha(char c);
    bool IsDigit(char c);

    void GetChar();
    void H();
    void ID();
    void NUM();
    void STR();
    void OPR();
    void PUN();
    void COM();
    void BOOL();

    int program_size_, line_, column_;
    char *cursor_, *program_;
    char symbol_;
    std::string token_;
    std::vector<Token> parsed_tokens_;
    Trie* keywords_trie_;
};

#endif //TRANSLATOR_LEXICAL_ANALYZER_H
