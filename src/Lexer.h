#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

enum class TokenType {
    IDENTIFIER,
    TERMINAL,
    COLON,
    PIPE,
    SEMI,
    LPAREN, RPAREN,
    LBRACKET, RBRACKET,
    LBRACE, RBRACE,
    COMMA,
    END  
};

struct Token {
    TokenType type;
    std::string text;
};


std::vector<Token> tokenize(const std::string& input) {

    auto sizeOfInput = input.size();
    size_t pos = 0;
    std::vector<Token> tokens;

    while(pos < sizeOfInput) {

        if(std::isspace(input[pos])) {
            pos++;
            continue;
        }

        if (input[pos] == '\'') {
            pos++; 
            std::string text;
            while(pos < sizeOfInput && input[pos] != '\'') {
                if(input[pos] == '\\') {
                    pos++;
                    text += input[pos];
                    pos++; 
                    continue;   
                }
                text += input[pos];
                pos++;
            }
            pos++;
            tokens.push_back({TokenType::TERMINAL, text});
            continue; 
        }
        

        switch (input[pos])
        {
            case ':':
                tokens.push_back({TokenType::COLON, ":"});
                pos++;
                continue;
            case '|':
                tokens.push_back({TokenType::PIPE, "|"});
                pos++;
                continue;
            case ';':
                tokens.push_back({TokenType::SEMI, ";"});
                pos++;
                continue;
            case '(':
                tokens.push_back({TokenType::LPAREN, "("});
                pos++;
                continue;
            case ')':
                tokens.push_back({TokenType::RPAREN, ")"});
                pos++;
                continue;
            case '[':
                tokens.push_back({TokenType::LBRACKET, "["});
                pos++;
                continue;
            case ']':
                tokens.push_back({TokenType::RBRACKET, "]"});
                pos++;
                continue;
            case '{':
                tokens.push_back({TokenType::LBRACE, "{"});
                pos++;
                continue;
            case '}':
                tokens.push_back({TokenType::RBRACE, "}"});
                pos++;
                continue;
            case ',':
                tokens.push_back({TokenType::COMMA, ","});
                pos++;
                continue;
        
        }

        if(std::isalpha(input[pos]) || input[pos] == '_') {
            std::string text = "";
            while (pos < sizeOfInput && (std::isalnum(input[pos]) || input[pos] == '_')) {
                text += input[pos];
                pos++;
            }
            tokens.push_back({TokenType::IDENTIFIER, text});
            continue;
        }
        throw std::runtime_error("Unknown character: " + std::string(1, input[pos]));
    }
    tokens.push_back({TokenType::END, ""});
    return tokens;
}
