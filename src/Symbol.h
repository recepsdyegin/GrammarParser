#pragma once

#include <memory>
#include <string>
#include <vector>

namespace gparser {



struct Symbol {
    virtual ~Symbol() = default;
    virtual std::string toString(int depth = 0) const = 0;
};

struct TerminalSymbol : public Symbol {
    std::string text;
    std::string toString(int depth = 0) const override {
        return std::string(depth * 2, ' ') + "Terminal('" + text + "')\n";
    }
};

struct ReferenceSymbol : public Symbol {
    std::string name;
     std::string toString(int depth = 0) const override {
        return std::string(depth * 2, ' ') + "Reference('" + name + "')\n";
    }
};

struct NonTerminalSymbol : public Symbol {
    std::vector<std::shared_ptr<Symbol>> children;
};

struct SequenceSymbol : NonTerminalSymbol {
    std::string toString(int depth = 0) const override {
        std::string out = std::string(depth * 2, ' ') + "Sequence\n";
        for (const auto &c : children) {
            out += c->toString(depth + 1);
        }
        return out;
    }
};

struct ChoiceSymbol : NonTerminalSymbol {
    std::string toString(int depth = 0) const override {
        std::string out = std::string(depth * 2, ' ') + "Choice\n";
        for (const auto &c : children) {
            out += c->toString(depth + 1);
        }   
        return out;
    }
};
struct OptionSymbol : NonTerminalSymbol {
    std::string toString(int depth = 0) const override {
        std::string out = std::string(depth * 2, ' ') + "Option\n";
        for (const auto &c : children) {
            out += c->toString(depth + 1);
        }
        return out;
    }
};

struct RepetitionSymbol : NonTerminalSymbol {
    std::string toString(int depth = 0) const override {
        std::string out = std::string(depth * 2, ' ') + "Repetition\n";
        for (const auto &c : children) {
            out += c->toString(depth + 1);
        }
        return out;
    }
};

} // namespace gparser