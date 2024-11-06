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
    if (symbol_ == '\0') {
        return;
    }
    if (IsAlpha(symbol_)) {
        // PushChar();
        GetChar();
        H();
        // ID();
    } else if (IsDigit(symbol_)) {
        PushChar();
        GetChar();
        NUM();
    } else if (symbol_ == '"') {
        GetChar();
        STRING();
    } else if (symbol_ == '=' || symbol_ == '*' || symbol_ == '/' || symbol_ == '%' ||
               symbol_ == '!' || symbol_ == '&' || symbol_ == '^' || symbol_ == '|' ||
               symbol_ == '+' || symbol_ == '-' || symbol_ == '<' || symbol_ == '>') {
        PushChar();
        // PushToken(TokenType::OPERATOR);
        // GetChar();
        OPERATOR();
        // H();
    } else if (symbol_ == ';' || symbol_ == ',' || symbol_ == ':' || symbol_ == '.' ||
               symbol_ == '(' || symbol_ == ')' || symbol_ == '[' || symbol_ == ']' ||
               symbol_ == '{' || symbol_ == '}') {
        PushChar();
        PushToken(TokenType::PUNCTUATOR);
        GetChar();
        H();
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
        tokens_.push_back(token);
        word_.clear();
        H();
    }
}

void LexicalAnalyzer::NUM() {
    if (IsDigit(symbol_)) {
        PushChar();
        GetChar();
        NUM();
    } else if (symbol_ == '.') {
        PushChar();
        GetChar();
        FLOAT_PART();
    } else {
        PushToken(TokenType::NUMBER);
        H();
    }
}

void LexicalAnalyzer::FLOAT_PART() {
    if (IsDigit(symbol_)) {
        PushChar();
        GetChar();
        FLOAT_PART();   
    } else {
        PushToken(TokenType::FLOAT);
        H();
    }
}

void LexicalAnalyzer::STRING() {
    if (symbol_ != '"') {
        PushChar();
        GetChar();
        STRING();
    } else {
        PushToken(TokenType::STRING);
        GetChar();
        H();
    }
}

void LexicalAnalyzer::OPERATOR() {
    GetChar();
    auto condidate = word_ + symbol_;
    if (condidate == "==" || condidate == "++" || condidate == ">=" ||
        condidate == "<=" || condidate == "+=" || condidate == "-=" || 
        condidate == "*=" || condidate == "/="
    ) {
        PushChar();
        GetChar();
    }
    PushToken(TokenType::OPERATOR);
    H();
}

void LexicalAnalyzer::PUN() {

}

void LexicalAnalyzer::COM() {

}
