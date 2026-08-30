#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace gpaser {

    struct TokenPosition {
        size_t line;
        size_t column;
    };

    enum class TokenType {
        IDENTIFIER,
        TERMINAL,
        PIPE,
        LPAREN, RPAREN,
        LBRACKET, RBRACKET,
        LBRACE, RBRACE
    };

    struct Token {
        TokenType type;
        std::string text;
        TokenPosition position;

        std::string getPositionString() {
            return "Line: " + std::to_string(position.line) + "Col: " + std::to_string(position.column);
        };
    };

    struct TokensInExpression {
        Token lhsToken;
        std::pmr::vector<Token> rhsTokens;
    };

    inline std::pmr::unordered_map<char, TokenType> tokenMap = {
        {'|', TokenType::PIPE},
        {'(', TokenType::LPAREN},
        {')', TokenType::RPAREN},
        {'[', TokenType::LBRACKET},
        {']', TokenType::RBRACKET},
        {'{', TokenType::LBRACE},
        {'}', TokenType::RBRACE}
    };
};