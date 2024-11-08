#include "../include/token.h"
#include <utility>

std::string to_string(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::LITERAL:
            return "LITERAL";
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::PUNCTUATOR:
            return "PUNCTUATOR";
        case TokenType::COMMENT:
            return "COMMENT";
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::STRING:
            return "STRING";
        default:
            return "UNKNOWN";
    }
}

Token::Token(TokenType type, std::string word, int line, int column): 
    type_(type), value_(word), line_(line), column_(column) {}

TokenType Token::GetType() {
    return type_;
}

std::string Token::GetValue() {
    return value_;
}

int Token::GetLine() {
    return line_;
}

int Token::GetColumn() {
    return column_;
}

void Token::SetType(TokenType type) {
    type_ = type;
}

void Token::SetValue(std::string value) {
    value_ = value;
}

void Token::SetLine(int line) {
    line_ = line;
}

void Token::SetColumn(int column) {
    column_ = column;
}

void Token::PrintInfo() {
    std::cout << value_ << "  - - - - - - - - - - - - - " << to_string(type_) << " " << line_ << ":" << column_ + 1 << std::endl;
}
