#include <cctype>
#include <stdexcept>

#include "Lexer.h"
#define LOG_TAG "[Grammer-Parser-Lexer] "

void gparser::Lexer::advance(bool lineEnded) {
    pos++;
    currentColumn++;
    if (lineEnded) {
        currentLine++;
        currentColumn = 0;
    }
}

gparser::TokenPosition gparser::Lexer::getCurrentPosition() {
    return {currentLine, currentColumn};
}

void gparser::Lexer::resetPosition() {
    currentLine = 0;
    currentColumn = 0;
    expressionCount = 0;
    pos = 0;
}

std::vector<gparser::TokensInExpression>
gparser::Lexer::tokenize(const std::string &input) {

    resetPosition();

    auto sizeOfInput = input.size();
    std::vector<TokensInExpression> tokens;

    TokensInExpression tokensInExpression;
    bool onRhs = false;

    while (pos < sizeOfInput) {

        // contunie if it is a space
        if (std::isspace(input[pos])) {
            bool lineEnded = input[pos] == '\n';
            advance(lineEnded);
            continue;
        }

        // handle escape character
        if (input[pos] == '\'') {
            TokenPosition startPosition = getCurrentPosition();

            if (!onRhs) {
                // there is a quotation on lhs
                throw std::runtime_error(
                    std::string(LOG_TAG) +
                    "Quotes are not allowed on the left-hand side. ");
            }

            advance();
            std::string text;
            while (pos < sizeOfInput && input[pos] != '\'') {
                if (input[pos] == '\\') {
                    advance();
                    text += input[pos];
                    advance();
                    continue;
                }

                text += input[pos];
                advance();
            }

            if (pos >= sizeOfInput) {
                throw std::runtime_error(std::string(LOG_TAG) +
                                         "Unterminated terminal literal.");
            }

            advance();
            tokensInExpression.rhsTokens.push_back(
                {TokenType::TERMINAL, text, startPosition});
            continue;
        }

        // check left-hand side handled
        if (input[pos] == ':') {
            if (onRhs) {
                throw std::runtime_error(
                    std::string(LOG_TAG) +
                    "Colon is used more than once in an expression.");
            }
            advance();
            onRhs = true;
            continue;
        }

        // check expression ended
        if (input[pos] == ';') {
            expressionCount++;
            onRhs = false;
            tokens.push_back(tokensInExpression);
            tokensInExpression = TokensInExpression{};
            advance();
            continue;
        }

        if (bracketMap.find(input[pos]) != bracketMap.end()) {
            if (!onRhs) {
                throw std::runtime_error(
                    std::string(LOG_TAG) +
                    "Unallowed brackret on the left-hand side. ");
            }
            tokensInExpression.rhsTokens.push_back({bracketMap[input[pos]],
                                                    std::string(1, input[pos]),
                                                    getCurrentPosition()});
            advance();
            continue;
        }

        if (std::isalpha(input[pos]) || input[pos] == '_') {
            TokenPosition startPosition = getCurrentPosition();
            std::string text = "";
            while (pos < sizeOfInput &&
                   (std::isalnum(input[pos]) || input[pos] == '_')) {
                text += input[pos];
                advance();
            }
            if (!onRhs) {
                tokensInExpression.lhsToken = {TokenType::IDENTIFIER, text,
                                               startPosition};
            } else {
                tokensInExpression.rhsTokens.push_back(
                    {TokenType::IDENTIFIER, text, startPosition});
            }
            continue;
        }
        throw std::runtime_error(std::string(LOG_TAG) + "Unknown character: " +
                                 std::string(1, input[pos]));
    }

    if (!tokensInExpression.rhsTokens.empty() ||
        !tokensInExpression.lhsToken.text.empty()) {
        throw std::runtime_error(std::string(LOG_TAG) +
                                 "Expression must end with a semicolon.");
    }

    return tokens;
}