#include "../include/lexical_analyzer.h"
#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    try {
        LexicalAnalyzer lexer("keywords.txt");
        std::vector<Token*> tokens = lexer.Tokenize("program.txt");
        for (auto& token : tokens) {
            token->PrintInfo();
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "[!] " << e.what() << std::endl;
        return 1;
    }
}
