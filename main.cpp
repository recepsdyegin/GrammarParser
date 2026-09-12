    #include "src/lexer/Lexer.h"
    #include "src/Parser.h"
    #include <iostream>

    int main() {

        gparser::Lexer lexer;

        auto exprs = lexer.tokenize("a : 'x' { 'y' | 'z' } ;");
        size_t i = 0;
        auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);
        std::cout << tree->toString();

    }