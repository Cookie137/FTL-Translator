#ifndef ELANG_SYNTAXER_H
#define ELANG_SYNTAXER_H

#include "Token.h"
#include <vector>
#include <stdexcept>
#include <iostream>

class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string& message, int line, int column)
            : std::runtime_error("Syntax error at line " + std::to_string(line) + ", column " + std::to_string(column) + ": " + message) {}
};

class SyntaxAnalyzer {
private:
    std::vector<Token> tokens_;
    size_t currentIndex_ = 0;

    Token& current();
    void advance();
    void expect(TokenType type);
    std::string tokenTypeToString(TokenType type);

    void parseProgram();
    void parseStatement();
    void parseFunctionDeclaration();
    void parseParameters();
    void parseVariableDeclaration();
    void parseType();
    void parseExpressionStatement();
    void parseExpression();
    void parseLogicalOr();
    void parseLogicalAnd();
    void parseEquality();
    void parseRelational();
    void parseAdditive();
    void parseMultiplicative();
    void parseUnary();
    void parsePrimary();

public:
    SyntaxAnalyzer(const std::vector<Token>& tokens);
    void analyze();
};

#endif //ELANG_SYNTAXER_H
