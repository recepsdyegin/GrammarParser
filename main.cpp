#include "src/lexer/Lexer.h"
#include <iostream>

int main() {
    gparser::Lexer lexer;
    try {
        auto exprs = lexer.tokenize("expr : 'a' | b ;");
        for (auto &e : exprs) {
            std::cout << "LHS: " << e.lhsToken.text << "\n";
            for (auto &t : e.rhsTokens)
                std::cout << "  RHS: " << t.text << "\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
}