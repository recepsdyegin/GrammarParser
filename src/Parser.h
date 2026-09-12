#pragma once

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include "Symbol.h"
#include "lexer/Lexer.h"

namespace gparser {

bool startsTerm(TokenType type) {
    return type == TokenType::IDENTIFIER || type == TokenType::TERMINAL ||
           type == TokenType::LBRACE || type == TokenType::LBRACKET ||
           type == TokenType::LPAREN;
}


std::shared_ptr<Symbol> parseChoice(const std::vector<Token> &tokens,
                                    size_t &i);

std::shared_ptr<Symbol> parseTerm(const std::vector<Token> &tokens, size_t &i) {

    if(i >= tokens.size()) {
        throw std::runtime_error("Unexpected end of tokens.");
    }

    if (tokens[i].type == TokenType::TERMINAL) {
        auto symbol = std::make_shared<TerminalSymbol>();
        symbol->text = tokens[i].text;
        i++;
        return symbol;
    }

    if (tokens[i].type == TokenType::IDENTIFIER) {
        auto symbol = std::make_shared<ReferenceSymbol>();
        symbol->name = tokens[i].text;
        i++;
        return symbol;
    }
    
    if (tokens[i].type == TokenType::LBRACE) {
        i++;
        auto inner = parseChoice(tokens, i);
        if (i >= tokens.size() || tokens[i].type != TokenType::RBRACE) {
            throw std::runtime_error("Expected '}'");
        }
        i++;
        auto symbol = std::make_shared<RepetitionSymbol>();
        symbol->children.push_back(inner);
        return symbol;
    } 

    if (tokens[i].type == TokenType::LBRACKET) {
        i++;
        auto inner = parseChoice(tokens, i);
        if (i >= tokens.size() || tokens[i].type != TokenType::RBRACKET) {
            throw std::runtime_error("Expected ']'");
        }
        i++;
        auto symbol = std::make_shared<OptionSymbol>();
        symbol->children.push_back(inner);
        return symbol;
    } 
    
    if (tokens[i].type == TokenType::LPAREN) {
        i++;
        auto inner = parseChoice(tokens, i);
        if (i >= tokens.size() || tokens[i].type != TokenType::RPAREN) {
            throw std::runtime_error("Expected ')'");
        }
        i++;
        return inner;       
    }   
    
    throw std::runtime_error("Unexpected token in term");
}

std::shared_ptr<Symbol> parseSequence(const std::vector<Token> &tokens, size_t &i) {

    std::vector<std::shared_ptr<Symbol>> items;

    while(i < tokens.size() && startsTerm(tokens[i].type)) {
        items.push_back(parseTerm(tokens,i));
    }
    
    if(items.empty()) {
        throw std::runtime_error("Empty sequence");
    }
    if(items.size() == 1) {
        return items[0];
    }

    auto symbol = std::make_shared<SequenceSymbol>();
    symbol->children = items;
    return symbol;
}

std::shared_ptr<Symbol> parseChoice(const std::vector<Token> &tokens, size_t &i) {

    std::vector<std::shared_ptr<Symbol>> items;
    items.push_back(parseSequence(tokens, i));

    while(i < tokens.size() && tokens[i].type == TokenType::PIPE) {
        i++; // skip PIPE
        items.push_back(parseSequence(tokens, i));
    }
    
    if(items.size() == 1) {
        return items[0];
    }

    auto symbol = std::make_shared<ChoiceSymbol>();
    symbol->children = items;
    return symbol;
}



} // namespace gparser