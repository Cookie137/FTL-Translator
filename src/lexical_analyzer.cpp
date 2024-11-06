#include "../include/lexical_analyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::string keywords_file) {
    std::ifstream file("../config/" + keywords_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open keywords file: " + keywords_file);
    }
    std::string keyword;
    keywords_trie_ = new Trie();
    while (file >> keyword) {
        keywords_trie_->AddString(keyword);
    }
    file.close();
}

LexicalAnalyzer::~LexicalAnalyzer() {
    delete keywords_trie_;
    delete program_;
}

std::vector<Token*> LexicalAnalyzer::Tokenize(std::string program_file) {
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
    iter_ = 0;
    line_ = column_ = 1;
    cursor_ = program_;
    word_.clear();
    tokens_.clear();
    GetChar();
    H();
    return tokens_;
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
    ++iter_;
}

void LexicalAnalyzer::PushChar() {
    word_.push_back(symbol_);
}

void LexicalAnalyzer::H() {
    //if (iter_ == )
    if (IsAlpha(symbol_)) {
        PushChar();
        GetChar();
        ID();
    } else if (IsDigit(symbol_)) {
        PushChar();
        GetChar();
        NUM();
    } else if (symbol_ == '"') {
        GetChar();
        STR();
    } else if (symbol_ == '=' || symbol_ == '*' || symbol_ == '/' || symbol_ == '%' ||
               symbol_ == '!' || symbol_ == '&' || symbol_ == '^' || symbol_ == '|' ||
               symbol_ == '+' || symbol_ == '-') {
        PushChar();
        GetChar();
        OPR();
    } else if (symbol_ == ';' || symbol_ == ',' || symbol_ == ':' || symbol_ == '.' ||
               symbol_ == '(' || symbol_ == ')' || symbol_ == '[' || symbol_ == ']' ||
               symbol_ == '{' || symbol_ == '}' || symbol_ == '<' || symbol_ == '>') {
        PushChar();
        GetChar();
        PUN();
    } else if (symbol_ == '\n') {

    } else {
        GetChar();
        H();
    }
}

void LexicalAnalyzer::ID() {
    if (IsAlpha(symbol_) || IsDigit(symbol_)) {
        PushChar();
        GetChar();
        ID();
    } else {
        Token* token = new Token(TokenType::IDENTIFIER, word_, line_, column_);
//        if (keywords_trie_->Contains()) {
//
//        } else {
//
//        }
        // if trie ...
        tokens_.push_back(token);
        word_.clear();
        H();
    }
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

void LexicalAnalyzer::BOOL() {
    if () {
        
    }
}
