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
    ERROR
};

class Token {
 public:
    Token();
    TokenType GetType();
    std::string GetValue();
    int GetLine();
    int GetColumn();
    void SetType(TokenType type);
    void SetValue(std::string value);
    void SetLine(int line);
    void SetColumn(int column);
    void PrintInfo();

 private:
    TokenType type_;
    std::string value_;
    int line_;
    int column_;
};

std::string to_string(TokenType type);

#endif //TRANSLATOR_TOKEN_H
