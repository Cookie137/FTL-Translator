#include "../include/lexical_analyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::string keywords_file) {
    std::ifstream file("../config/" + keywords_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open keywords file: " + keywords_file);
    }
    std::string word;
    keywords_trie_ = new Trie();
    while (file >> word) {
        keywords_trie_->AddString(word);
    }
    file.close();
}

LexicalAnalyzer::~LexicalAnalyzer() {
    delete keywords_trie_;
    delete program_;
}

std::vector<Token> LexicalAnalyzer::Tokenize(std::string program_file) {
    try {
        std::ifstream file("../config/" + program_file, std::ifstream::binary);
        file.seekg(0, file.end);
        program_size_ = (int)file.tellg();
        file.seekg(0, file.beg);
        program_ = new char[program_size_];
        file.read(program_, program_size_);
        file.close();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to open program file: " + program_file);
    }
    line_ = column_ = 1;
    cursor_ = program_;
    token_.clear();
    parsed_tokens_.clear();
    GetChar();
    H();
    return parsed_tokens_;
}

bool LexicalAnalyzer::IsAlpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_');
}

bool LexicalAnalyzer::IsDigit(char c) {
    return ('0' <= c && c <= '9');
}

void LexicalAnalyzer::GetChar() {
    symbol_ = *cursor_;
    ++cursor_;
}

void LexicalAnalyzer::H() {
    if (IsAlpha(symbol_)) {
        token_.push_back(symbol_);
        GetChar();
        ID();
    } else if (IsDigit(symbol_)) {
        token_.push_back(symbol_);
        GetChar();
        NUM();
    } else if (symbol_ == '"') {
        GetChar();
        STR();
    }
}

void LexicalAnalyzer::ID() {

}

void LexicalAnalyzer::NUM() {

}

void LexicalAnalyzer::STR() {

}

void LexicalAnalyzer::OPR() {

}

void LexicalAnalyzer::PUN() {

}

void LexicalAnalyzer::COM() {

}
