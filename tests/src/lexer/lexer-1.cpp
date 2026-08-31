#include <gtest/gtest.h>

#include <stdexcept>

#include "Lexer.h"

using gparser::Lexer;
using gparser::TokenType;

// --- happy path -------------------------------------------------------------

TEST(LexerTest, SingleSimpleRule) {
    Lexer lexer;
    auto exprs = lexer.tokenize("expr : 'a' ;");

    ASSERT_EQ(exprs.size(), 1u);
    EXPECT_EQ(exprs[0].lhsToken.type, TokenType::IDENTIFIER);
    EXPECT_EQ(exprs[0].lhsToken.text, "expr");

    ASSERT_EQ(exprs[0].rhsTokens.size(), 1u);
    EXPECT_EQ(exprs[0].rhsTokens[0].type, TokenType::TERMINAL);
    EXPECT_EQ(exprs[0].rhsTokens[0].text, "a");
}

TEST(LexerTest, IdentifierReferenceOnRhs) {
    Lexer lexer;
    auto exprs = lexer.tokenize("rule : other ;");

    ASSERT_EQ(exprs.size(), 1u);
    ASSERT_EQ(exprs[0].rhsTokens.size(), 1u);
    EXPECT_EQ(exprs[0].rhsTokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(exprs[0].rhsTokens[0].text, "other");
}

TEST(LexerTest, PipeAndGroupingTokens) {
    Lexer lexer;
    auto exprs = lexer.tokenize("expr : 'a' | ( 'b' ) ;");

    ASSERT_EQ(exprs.size(), 1u);
    const auto& rhs = exprs[0].rhsTokens;
    ASSERT_EQ(rhs.size(), 5u);
    EXPECT_EQ(rhs[0].type, TokenType::TERMINAL);
    EXPECT_EQ(rhs[1].type, TokenType::PIPE);
    EXPECT_EQ(rhs[2].type, TokenType::LPAREN);
    EXPECT_EQ(rhs[3].type, TokenType::TERMINAL);
    EXPECT_EQ(rhs[4].type, TokenType::RPAREN);
}

TEST(LexerTest, BraceAndBracketTokens) {
    Lexer lexer;
    auto exprs = lexer.tokenize("r : { 'a' } [ 'b' ] ;");

    ASSERT_EQ(exprs.size(), 1u);
    const auto& rhs = exprs[0].rhsTokens;
    ASSERT_EQ(rhs.size(), 6u);
    EXPECT_EQ(rhs[0].type, TokenType::LBRACE);
    EXPECT_EQ(rhs[2].type, TokenType::RBRACE);
    EXPECT_EQ(rhs[3].type, TokenType::LBRACKET);
    EXPECT_EQ(rhs[5].type, TokenType::RBRACKET);
}

TEST(LexerTest, MultipleExpressions) {
    Lexer lexer;
    auto exprs = lexer.tokenize("a : 'x' ;\nb : 'y' ;\n");

    ASSERT_EQ(exprs.size(), 2u);
    EXPECT_EQ(exprs[0].lhsToken.text, "a");
    EXPECT_EQ(exprs[1].lhsToken.text, "b");
}

TEST(LexerTest, EscapedQuoteInsideTerminal) {
    Lexer lexer;
    auto exprs = lexer.tokenize("q : 'a\\'b' ;");

    ASSERT_EQ(exprs.size(), 1u);
    ASSERT_EQ(exprs[0].rhsTokens.size(), 1u);
    EXPECT_EQ(exprs[0].rhsTokens[0].text, "a'b");
}

TEST(LexerTest, HandlesLeadingAndTrailingWhitespace) {
    Lexer lexer;
    auto exprs = lexer.tokenize("\n\n   expr   :   'a'   ;   \n");

    ASSERT_EQ(exprs.size(), 1u);
    EXPECT_EQ(exprs[0].lhsToken.text, "expr");
    ASSERT_EQ(exprs[0].rhsTokens.size(), 1u);
    EXPECT_EQ(exprs[0].rhsTokens[0].text, "a");
}

TEST(LexerTest, EmptyInputProducesNoExpressions) {
    Lexer lexer;
    EXPECT_TRUE(lexer.tokenize("").empty());
    EXPECT_TRUE(lexer.tokenize("   \n\t  ").empty());
}

// A fresh instance must not carry state between calls.
TEST(LexerTest, TokenizeCanBeCalledMultipleTimes) {
    Lexer lexer;
    auto first = lexer.tokenize("a : 'x' ;");
    auto second = lexer.tokenize("a : 'x' ;");

    ASSERT_EQ(second.size(), first.size());
    ASSERT_EQ(second.size(), 1u);
    EXPECT_EQ(second[0].lhsToken.text, "a");
    ASSERT_EQ(second[0].rhsTokens.size(), 1u);
    EXPECT_EQ(second[0].rhsTokens[0].text, "x");
}

// --- documented current behaviour -----------------------------------------

// A rule that is not closed with ';' is currently dropped silently.
TEST(LexerTest, RuleWithoutSemicolonIsDropped) {
    Lexer lexer;
    EXPECT_TRUE(lexer.tokenize("expr : 'a'").empty());
}

// --- error handling -------------------------------------------------------

TEST(LexerTest, QuoteOnLhsThrows) {
    Lexer lexer;
    EXPECT_THROW(lexer.tokenize("'a' : 'b' ;"), std::runtime_error);
}

TEST(LexerTest, ColonUsedTwiceThrows) {
    Lexer lexer;
    EXPECT_THROW(lexer.tokenize("a : b : c ;"), std::runtime_error);
}

TEST(LexerTest, BracketOnLhsThrows) {
    Lexer lexer;
    EXPECT_THROW(lexer.tokenize("a ( : 'b' ;"), std::runtime_error);
}

TEST(LexerTest, UnterminatedTerminalThrows) {
    Lexer lexer;
    EXPECT_THROW(lexer.tokenize("a : 'bcd ;"), std::runtime_error);
}

TEST(LexerTest, UnknownCharacterThrows) {
    Lexer lexer;
    EXPECT_THROW(lexer.tokenize("a : @ ;"), std::runtime_error);
}
