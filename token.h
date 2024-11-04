#ifndef TRANSLATOR_TOKEN_H
#define TRANSLATOR_TOKEN_H

#include <iostream>
#include <string>

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    LITERAL,
    OPERATOR,
    PUNCTUATOR,
    COMMENT,
    ERROR,
    UNKNOWN
};

class Token {
 public:
    Token(TokenType type, std::string value, int line, int column);
    TokenType GetType();
    std::string GetValue();
    int GetLine();
    int GetColumn();
    void PrintInfo();

 private:
    TokenType type_;
    std::string value_;
    int line_;
    int column_;
};

std::string to_string(TokenType type);

#endif //TRANSLATOR_TOKEN_H
