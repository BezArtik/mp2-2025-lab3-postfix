#pragma once

#ifndef __ARITHMETIC_H__
#define __ARITHMETIC_H__

#include "stack.h"
#include <iomanip>
#include <sstream>
#include <cmath>

enum class TokenType {
    NUMBER, BINARY_PLUS, BINARY_MINUS, MUL, DIV, POW, UNARY_MINUS, UNARY_PLUS,
    SIN, COS, TAN, SQRT, LOG, ACOS, ASIN, ATAN,
    SINH, COSH, TANH, ASINH, ACOSH, ATANH, FACT,
    LEFT_PAREN, RIGHT_PAREN,
    VARIABLE, CONST
};

class ArithmeticExpression {
private:
    struct Token {
        TokenType type;
        std::string value;
        Token(TokenType t, const std::string& v) : type(t), value(v) {}
    };

    List<Token> original_tokens;
    List<Token> resolved_tokens;

    // PARSING
    List<Token> parse_string(const std::string& expr) const;
    List<Token> parse_postfix_string(const std::string& expr) const;

    // VALIDATION
    void validate_expression(const List<Token>& tokens) const;
    void validate_brackets(const List<Token>& tokens) const;
    void validate_operators(const List<Token>& tokens) const;
    void validate_operands(const List<Token>& tokens) const;
    void validate_number_format(const List<Token>& tokens) const;

    // TOKEN OPERATIONS
    std::string to_postfix() const;
    std::string token_to_string(const Token& token) const noexcept;
    TokenType string_to_token(const std::string& str) const noexcept;
    int get_priority(TokenType op) const noexcept;

    // TYPE CHECKS
    bool is_constant(const std::string& c) const noexcept;
    bool is_digit(char c) const noexcept;
    bool is_dot(char c) const noexcept;
    bool is_space(char c) const noexcept;
    bool is_operator(char c) const noexcept;
    bool is_letter(char c) const noexcept;
    bool is_variable(char c) const noexcept;
    bool is_unary(const List<Token>& tokens) const noexcept;
    bool is_function_token(TokenType type) const noexcept;
    bool is_operand_token(TokenType type) const noexcept;
    bool is_binary_operator_token(TokenType op) const noexcept;
    bool is_unary_operator_token(TokenType op) const noexcept;

    // MATH OPERATIONS
    double string_to_double(const std::string& expr) const;
    double fact(double num) const;
    double apply_constant(const std::string& expr) const;
    double apply_binary_operator(double left, double right, TokenType op) const;
    double apply_function(TokenType func_type, double arg) const;

public:
    ArithmeticExpression(const std::string& expr);
    double calculate() const;
    bool has_variables() const noexcept;
    List<std::string> get_variable_names() const noexcept;
    void set_variable(const std::string& expr, double value);
};

void input_and_calculate();


#endif
