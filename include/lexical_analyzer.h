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

    void H();
    void ID();
    void NUM();
    void STR();
    void OPR();
    void PUN();
    void COM();

    int program_size_, iter_, line_, column_, cur_column_;
    char *cursor_, *program_;
    char symbol_;
    std::string word_;
    std::vector<Token*> tokens_;
    Trie* keywords_trie_;
};

#endif //TRANSLATOR_LEXICAL_ANALYZER_H
