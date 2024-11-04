#include "lexical_analyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::string keywords_file) {
    std::ifstream keywords(keywords_file);
    std::string word;
    if (!keywords.is_open()) {
        throw std::runtime_error("Failed to open keywords file: " + keywords_file);
    }
    while (keywords >> word) {
        keywords_trie_.AddString(word);
    }
    keywords.close();
}

std::vector<Token> LexicalAnalyzer::Tokenize(std::string program_file) {
    std::vector<Token> tokens;
    std::ifstream program(program_file);
    if (!program.is_open()) {
        throw std::runtime_error("Failed to open program file: " + program_file);
    }
    // fucking DKA is missing
    program.close();
    return tokens;
}
