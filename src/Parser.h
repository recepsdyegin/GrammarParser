#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Symbol.h"
#include "lexer/Lexer.h"

namespace gparser {

std::shared_ptr<Symbol> parseChoice(const std::vector<Token> &tokens,
                                    size_t &i);

std::shared_ptr<Symbol> parseTerm(const std::vector<Token> &tokens, size_t &i) {

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

    // TODO: Other cases ...
    return nullptr;
}

} // namespace gparser