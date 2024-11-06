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
        default:
            return "UNKNOWN";
    }
}

Token::Token() {}

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
    std::cout << "================================" << '\n';
    std::cout << "Type: " << to_string(type_) << '\n';
    std::cout << "Value: " << value_ << '\n';
    std::cout << "Line: " << line_ << '\n';
    std::cout << "Column: " << column_ << '\n';
    std::cout << "================================" << std::endl;
}
