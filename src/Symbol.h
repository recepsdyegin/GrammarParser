#pragma once

#include <string>
#include <vector>
#include <memory>

struct Symbol {
    virtual ~Symbol() = default;
    
};

struct TerminalSymbol : public Symbol {

    std::string text;
};

struct ReferenceSymbol : public Symbol {
    std::string name;
};

struct NonTerminalSymbol : public Symbol {
    std::vector<std::shared_ptr<Symbol>> children;
};

struct SequenceSymbol : NonTerminalSymbol {};

struct ChoiceSymbol : NonTerminalSymbol {};

struct OptionSymbol : NonTerminalSymbol {};

struct RepetitionSymbol : NonTerminalSymbol {};