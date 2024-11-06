#ifndef TRANSLATOR_LEXICAL_ANALYZER_H
#define TRANSLATOR_LEXICAL_ANALYZER_H

#include "token.h"
#include "trie.h"
#include <vector>

class LexicalAnalyzer {
 public:
    LexicalAnalyzer(std::string keywords_file);
    ~LexicalAnalyzer();
    std::vector<Token*> Tokenize(std::string program_file);

 private:
    bool IsAlpha(char c);
    bool IsDigit(char c);
    void GetChar();
    void PushChar();

    void PushToken(TokenType tokenType) {
        tokens_.push_back(new Token(tokenType, word_, line_, column_));
        word_.clear();
    }

    void H();
    void ID();
    void NUM();
    void STRING();
    void OPERATOR();
    void PUN();
    void COM();
    void FLOAT_PART();

    int program_size_, iter_, line_, column_;
    char *cursor_, *program_;
    char symbol_;
    std::string word_;
    std::vector<Token*> tokens_;
    Trie* keywords_trie_;
};

#endif //TRANSLATOR_LEXICAL_ANALYZER_H
