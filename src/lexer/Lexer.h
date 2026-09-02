#pragma once

#define LOG_TAG "[Grammer-Parser-Lexer] "

#include "Token.hpp"

#include <string>

namespace gparser {

class Lexer {

  private:
    size_t currentLine = 0;
    size_t currentColumn = 0;
    size_t expressionCount = 0;
    size_t pos = 0;

    void advance(bool lineEnded = false);

    TokenPosition getCurrentPosition();

    void resetPosition();

  public:
    std::vector<TokensInExpression> tokenize(const std::string &input);
};

} // namespace gparser