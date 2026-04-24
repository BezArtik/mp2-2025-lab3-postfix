#pragma once

#include "containers/vector.hpp"
#include "containers/map.hpp"
#include <string>
#include <cstdint>

namespace arithmetic {

enum class TokenType : uint8_t {
    NUMBER, BINARY_PLUS, BINARY_MINUS, MUL, DIV, POW, UNARY_MINUS, UNARY_PLUS,
    SIN, COS, TAN, SQRT, LOG, ACOS, ASIN, ATAN,
    SINH, COSH, TANH, ASINH, ACOSH, ATANH,
    LEFT_PAREN, RIGHT_PAREN,
    VARIABLE, CONST
};

class ArithmeticExpression {
    struct Token {
        TokenType type_;
        std::string value_;
        Token(TokenType t, const std::string& v) : type_(t), value_(v) {}
    };
    using TokenVec = containers::Vector<Token>;
    using TokenIter = typename TokenVec::const_iterator;
    using VariableMap = containers::Map<std::string, double>;

    TokenVec original_tokens_;
    TokenVec resolved_tokens_;
    VariableMap variables_;

    // PARSING
    TokenVec parse_string(const std::string& expr) const;

    // VALIDATION
    void validate_expression(TokenIter begin, TokenIter end) const;
    void validate_operands(TokenIter begin, TokenIter end) const;
    void validate_brackets(TokenIter begin, TokenIter end) const;
    void validate_operators(TokenIter begin, TokenIter end) const;
    void validate_number_format(TokenIter begin, TokenIter end) const;

    // TOKEN OPERATIONS
    TokenType char_to_token_type(char c) const noexcept;
    TokenVec to_postfix_tokens() const;
    TokenType string_to_token(const std::string& str) const noexcept;
    int get_priority(TokenType op) const noexcept;

    // TYPE CHECKS
    bool is_constant(const std::string& c) const noexcept;
    bool is_function_token(TokenType type) const noexcept;
    bool is_operand_token(TokenType type) const noexcept;
    bool is_operator_token(TokenType op) const noexcept;
    bool is_unary_operator_token(TokenType op) const noexcept;

    // MATH OPERATIONS
    double string_to_double(const std::string& expr) const;
    double apply_constant(const std::string& expr) const;
    double apply_binary_operator(double left, double right, TokenType op) const;
    double apply_function(TokenType func_type, double arg) const;

public:
    ArithmeticExpression(const std::string& expr);
    double calculate() const;

    // VARIABLE OPERATIONS
    bool has_variables() const noexcept;
    void set_variable(const std::string& name, double value);
    containers::Vector<std::string> get_variable_names() const noexcept;
};

}