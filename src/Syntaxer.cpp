#include "../include/Syntaxer.h"

// Возвращает текущий токен
Token& SyntaxAnalyzer::current() {
    if (currentIndex_ >= tokens_.size()) {
        static Token eof(TokenType::EndOfFile, "", -1, -1);
        return eof;
    }
    return tokens_[currentIndex_];
}

// Переходит к следующему токену
void SyntaxAnalyzer::advance() {
    if (currentIndex_ < tokens_.size()) {
        ++currentIndex_;
    }
}

// Проверяет ожидаемый токен и продвигается вперед
void SyntaxAnalyzer::expect(TokenType type) {
    if (current().type != type) {
        throw SyntaxError("Expected token of type " + tokenTypeToString(type) +
                          ", but got " + tokenTypeToString(current().type),
                          current().line, current().column);
    }
    advance();
}

// Преобразует TokenType в строку
std::string SyntaxAnalyzer::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KW_FUNCTION: return "function";
        case TokenType::KW_VAR: return "var";
        case TokenType::KW_LET: return "let";
        case TokenType::Identifier: return "identifier";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::COLON: return ":";
        case TokenType::COMMA: return ",";
        case TokenType::SEMICOLON: return ";";
        case TokenType::OP_ASSIGN: return "=";
        case TokenType::IntegerLiteral: return "integer literal";
        default: return "unknown";
    }
}

// Анализ программы
void SyntaxAnalyzer::parseProgram() {
    while (current().type != TokenType::EndOfFile) {
        parseStatement();
    }
}

// Анализ операторов
void SyntaxAnalyzer::parseStatement() {
    if (current().type == TokenType::KW_FUNCTION) {
        parseFunctionDeclaration();
    } else if (current().type == TokenType::KW_VAR || current().type == TokenType::KW_LET) {
        parseVariableDeclaration();
    } else if (current().type == TokenType::Identifier) {
        parseExpressionStatement();
    } else {
        throw SyntaxError("Unexpected token: " + current().value, current().line, current().column);
    }
}

// Анализ объявления функции
void SyntaxAnalyzer::parseFunctionDeclaration() {
    expect(TokenType::KW_FUNCTION);
    expect(TokenType::Identifier); // Имя функции
    expect(TokenType::LPAREN);
    if (current().type != TokenType::RPAREN) {
        parseParameters();
    }
    expect(TokenType::RPAREN);
    if (current().type == TokenType::COLON) {
        advance(); // Пропускаем ":"
        parseType();
    }
    expect(TokenType::LBRACE);
    while (current().type != TokenType::RBRACE) {
        parseStatement();
    }
    expect(TokenType::RBRACE);
}

// Анализ параметров функции
void SyntaxAnalyzer::parseParameters() {
    do {
        expect(TokenType::Identifier); // Имя параметра
        expect(TokenType::COLON);
        parseType();
        if (current().type == TokenType::COMMA) {
            advance();
        }
    } while (current().type != TokenType::RPAREN);
}

// Анализ объявления переменной
void SyntaxAnalyzer::parseVariableDeclaration() {
    advance(); // Пропускаем "var" или "let"
    expect(TokenType::Identifier); // Имя переменной
    expect(TokenType::OP_ASSIGN);
    parseExpression();
}

// Анализ типа
void SyntaxAnalyzer::parseType() {
    if (current().type == TokenType::Identifier) {
        advance();
    } else {
        throw SyntaxError("Expected a type, but got: " + current().value, current().line, current().column);
    }
}

// Анализ выражения в строке
void SyntaxAnalyzer::parseExpressionStatement() {
    parseExpression();
    expect(TokenType::SEMICOLON);
}

// Анализ выражения
void SyntaxAnalyzer::parseExpression() {
    parseLogicalOr();
}

// Анализ логического "или"
void SyntaxAnalyzer::parseLogicalOr() {
    parseLogicalAnd();
    while (current().type == TokenType::OP_OR) {
        advance();
        parseLogicalAnd();
    }
}

// Анализ логического "и"
void SyntaxAnalyzer::parseLogicalAnd() {
    parseEquality();
    while (current().type == TokenType::OP_AND) {
        advance();
        parseEquality();
    }
}

// Анализ операций сравнения
void SyntaxAnalyzer::parseEquality() {
    parseRelational();
    while (current().type == TokenType::OP_EQUAL || current().type == TokenType::OP_NOT_EQUAL) {
        advance();
        parseRelational();
    }
}

// Анализ операций отношения
void SyntaxAnalyzer::parseRelational() {
    parseAdditive();
    while (current().type == TokenType::OP_LESS || current().type == TokenType::OP_GREATER ||
           current().type == TokenType::OP_LESS_EQUAL || current().type == TokenType::OP_GREATER_EQUAL) {
        advance();
        parseAdditive();
    }
}

// Анализ сложения/вычитания
void SyntaxAnalyzer::parseAdditive() {
    parseMultiplicative();
    while (current().type == TokenType::OP_PLUS || current().type == TokenType::OP_MINUS) {
        advance();
        parseMultiplicative();
    }
}

// Анализ умножения/деления/остатка
void SyntaxAnalyzer::parseMultiplicative() {
    parseUnary();
    while (current().type == TokenType::OP_MULTIPLY || current().type == TokenType::OP_DIVIDE || current().type == TokenType::OP_MODULO) {
        advance();
        parseUnary();
    }
}

// Анализ унарных операций
void SyntaxAnalyzer::parseUnary() {
    if (current().type == TokenType::OP_NOT || current().type == TokenType::OP_MINUS) {
        advance();
    }
    parsePrimary();
}

// Анализ первичных выражений
void SyntaxAnalyzer::parsePrimary() {
    if (current().type == TokenType::IntegerLiteral || current().type == TokenType::StringLiteral ||
        current().type == TokenType::BooleanLiteral || current().type == TokenType::Identifier) {
        advance();
    } else if (current().type == TokenType::LPAREN) {
        advance();
        parseExpression();
        expect(TokenType::RPAREN);
    } else {
        throw SyntaxError("Unexpected token in expression: " + current().value, current().line, current().column);
    }
}

// Конструктор
SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& tokens) : tokens_(tokens) {}

// Запуск анализа
void SyntaxAnalyzer::analyze() {
    parseProgram();
    if (current().type != TokenType::EndOfFile) {
        throw SyntaxError("Unexpected tokens at the end of the file.", current().line, current().column);
    }
    std::cout << "Parsing completed successfully.\n";
}
