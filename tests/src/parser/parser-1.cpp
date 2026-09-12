#include <gtest/gtest.h>
#include <stdexcept>

#include "Lexer.h"
#include "Parser.h"

gparser::Lexer lexer;

TEST(ParserTest, SingleTerminal) {
    auto exprs = lexer.tokenize("a:'x';");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Terminal('x')\n");
}

TEST(ParserTest, SequenceSymbol) {
    auto exprs = lexer.tokenize("a: 'x' 'y';");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Sequence\n  Terminal('x')\n  Terminal('y')\n");
}

TEST(ParserTest, ChoiceOfTwoTerminals) {
    auto exprs = lexer.tokenize("a: 'x' | 'y' ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(),"Choice\n  Terminal('x')\n  Terminal('y')\n");
}

TEST(ParserTest, Repetition) {
    auto exprs = lexer.tokenize("a : { 'x' } ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(),"Repetition\n  Terminal('x')\n");
}

TEST(ParserTest, Option) {
    auto exprs = lexer.tokenize("a : [ 'x' ] ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Option\n  Terminal('x')\n");
}


TEST(ParserTest, Terminal) {
    auto exprs = lexer.tokenize("a : ( 'x' ) ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(),"Terminal('x')\n");
}

TEST(ParserTest, Reference) {
    auto exprs = lexer.tokenize("a : b ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Reference('b')\n");
}

TEST(ParserTest, Priority) {
    auto exprs = lexer.tokenize("a : 'x' | 'y' 'z' ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Choice\n  Terminal('x')\n  Sequence\n    Terminal('y')\n    Terminal('z')\n");
}

TEST(ParserTest, GroupOverridesPrecedence) {
    auto exprs = lexer.tokenize("a : ( 'x' | 'y' ) 'z' ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Sequence\n  Choice\n    Terminal('x')\n    Terminal('y')\n  Terminal('z')\n");
}

TEST(ParserTest, NestedChoiceInsideRepetition) {
    auto exprs = lexer.tokenize("a : 'x' { 'y' | 'z' } ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Sequence\n  Terminal('x')\n  Repetition\n    Choice\n      Terminal('y')\n      Terminal('z')\n");
}

TEST(ParserTest, ThreeAlternatives) {
    auto exprs = lexer.tokenize("a : 'x' | 'y' | 'z' ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Choice\n  Terminal('x')\n  Terminal('y')\n  Terminal('z')\n");
}

TEST(ParserTest, NestedRepetition) {
    auto exprs = lexer.tokenize("a : { { 'x' } } ;");
    size_t i = 0;
    auto tree = gparser::parseChoice(exprs[0].rhsTokens, i);

    EXPECT_EQ(tree->toString(), "Repetition\n  Repetition\n    Terminal('x')\n");
}

TEST(ParserTest, UnclosedRepetition) {
    auto exprs = lexer.tokenize("a : { 'x' ;");
    size_t i = 0;

    EXPECT_THROW(gparser::parseChoice(exprs[0].rhsTokens, i), std::runtime_error);
}

TEST(ParserTest, UnexpectedToken) {
    auto exprs = lexer.tokenize("a : ) ;");
    size_t i = 0;

    EXPECT_THROW(gparser::parseChoice(exprs[0].rhsTokens, i), std::runtime_error);
}

TEST(ParserTest, EmptySequence) {
    auto exprs = lexer.tokenize("a : ;");
    size_t i = 0;

    EXPECT_THROW(gparser::parseChoice(exprs[0].rhsTokens, i), std::runtime_error);
}

TEST(ParserTest, EmptyAfterPipe) {
    auto exprs = lexer.tokenize("a : 'x' | ;");
    size_t i = 0;

    EXPECT_THROW(gparser::parseChoice(exprs[0].rhsTokens, i), std::runtime_error);
}