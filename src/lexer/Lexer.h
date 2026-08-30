#pragma once

#define LOG_TAG "[Grammer-Parser-Lexer] "

#include "Token.hpp"

#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <cstddef>

namespace gpaser {

    class Lexer {

        private:
            size_t currentLine = 0;
            size_t currentColumn = 0;
            size_t expressionCount = 0;
            size_t pos = 0;

            void advance(bool lineEnded = false) {
                pos++;
                currentColumn++;
                if (lineEnded) {
                    currentLine++;
                    currentColumn = 0;
                }
            }

            TokenPosition getCurrentPosition() {
                return {currentLine, currentColumn};
            }

        public:
        std::vector<TokensInExpression> tokenize(const std::string& input) {

            auto sizeOfInput = input.size();
            std::vector<TokensInExpression> tokens;
            
            TokensInExpression tokensInExpression;
            bool onRhs = false;

            while(pos < sizeOfInput) {
                
                // contunie if it is a space
                if(std::isspace(input[pos])) {
                    bool lineEnded = input[pos] == '\n';
                    advance(lineEnded);
                    continue;
                }
                
                // handle escape character
                if (input[pos] == '\'') {
                    if (!onRhs) {
                        // there is a quotation on lhs
                        throw std::runtime_error(std::string(LOG_TAG) + "Quotes are not allowed on the left-hand side. ");
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
                        throw std::runtime_error(
                            std::string(LOG_TAG) + "Unterminated terminal literal."
                        );
                    }

                    advance();
                    tokensInExpression.rhsTokens.push_back({TokenType::TERMINAL, text, getCurrentPosition()});
                    continue; 
                }
                
                // check left-hand side handled
                if (input[pos] == ':') {
                    if (onRhs) {
                        throw std::runtime_error(std::string(LOG_TAG) + "Colon is used more than once in an expression.");
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

                if (tokenMap.find(input[pos]) != tokenMap.end()) {
                    if (!onRhs) {
                        throw std::runtime_error(std::string(LOG_TAG) + "Unallowed brackret on the left-hand side. ");
                    }
                    tokensInExpression.rhsTokens.push_back({tokenMap[input[pos]], std::string(1, input[pos])});
                    advance();
                    continue;
                }
                
                if(std::isalpha(input[pos]) || input[pos] == '_') {
                    std::string text = "";
                    while (pos < sizeOfInput && (std::isalnum(input[pos]) || input[pos] == '_')) {
                        text += input[pos];
                        advance();
                    }
                    if (!onRhs) {
                        tokensInExpression.lhsToken = {TokenType::IDENTIFIER, text, getCurrentPosition()};
                    }
                    else {
                        tokensInExpression.rhsTokens.push_back({TokenType::IDENTIFIER, text, getCurrentPosition()});
                    }
                    continue;
                }
                throw std::runtime_error(std::string(LOG_TAG) + "Unknown character: " + std::string(1, input[pos]));
            }
            return tokens;
        }
    };

}