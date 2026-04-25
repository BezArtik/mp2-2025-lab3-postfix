#include "arithmetic_expression/token_utils.hpp"
#include "arithmetic_expression/token.hpp"
#include "containers/vector.hpp"
#include <cstdint>
#include <string>
#include <algorithm>

namespace arithmetic::token_utils {

bool is_constant(const std::string& name) noexcept {
    return name == "pi" || name == "e";
}

bool is_function_token(TokenType type) noexcept {
    return get_priority(type) == 5;
}

bool is_operator_token(TokenType type) noexcept {
    auto p = get_priority(type);
    return p >= 1 && p <= 4;
}

bool is_unary_operator_token(TokenType type) noexcept {
    return get_priority(type) == 2;
}

bool is_operand_token(TokenType type) noexcept {
    return type == TokenType::NUMBER  ||
           type == TokenType::CONST   ||
           type == TokenType::VARIABLE;
}

int32_t get_priority(TokenType op) noexcept {
    switch (op) {
    case TokenType::SIN: case TokenType::COS: case TokenType::TAN:
    case TokenType::SQRT: case TokenType::LOG:
    case TokenType::ACOS: case TokenType::ASIN: case TokenType::ATAN:
    case TokenType::SINH: case TokenType::COSH: case TokenType::TANH:
    case TokenType::ACOSH: case TokenType::ASINH: case TokenType::ATANH:
        return 5;
    case TokenType::POW:
        return 4;
    case TokenType::MUL: case TokenType::DIV:
        return 3;
    case TokenType::UNARY_MINUS: case TokenType::UNARY_PLUS:
        return 2;
    case TokenType::BINARY_PLUS: case TokenType::BINARY_MINUS:
        return 1;
    case TokenType::LEFT_PAREN:
        return 0;
    default:
        return -1;
    }
}

TokenType string_to_token(const std::string& str) noexcept {
    if (str == "+") return TokenType::BINARY_PLUS;
    if (str == "-") return TokenType::BINARY_MINUS;
    if (str == "*") return TokenType::MUL;
    if (str == "/") return TokenType::DIV;
    if (str == "^") return TokenType::POW;
    if (str == "(") return TokenType::LEFT_PAREN;
    if (str == ")") return TokenType::RIGHT_PAREN;
    if (str == "~") return TokenType::UNARY_MINUS;
    if (str == "$") return TokenType::UNARY_PLUS;

    if (str == "sin")   return TokenType::SIN;
    if (str == "cos")   return TokenType::COS;
    if (str == "tan")   return TokenType::TAN;
    if (str == "sqrt")  return TokenType::SQRT;
    if (str == "log")   return TokenType::LOG;
    if (str == "asin")  return TokenType::ASIN;
    if (str == "acos")  return TokenType::ACOS;
    if (str == "atan")  return TokenType::ATAN;
    if (str == "sinh")  return TokenType::SINH;
    if (str == "cosh")  return TokenType::COSH;
    if (str == "tanh")  return TokenType::TANH;
    if (str == "asinh") return TokenType::ASINH;
    if (str == "acosh") return TokenType::ACOSH;
    if (str == "atanh") return TokenType::ATANH;

    return TokenType::NUMBER; 
}

TokenType char_to_token(int8_t c) noexcept {
    switch (c) {
    case '+': return TokenType::BINARY_PLUS;
    case '-': return TokenType::BINARY_MINUS;
    case '*': return TokenType::MUL;
    case '/': return TokenType::DIV;
    case '^': return TokenType::POW;
    case '(': return TokenType::LEFT_PAREN;
    case ')': return TokenType::RIGHT_PAREN;
    default:  return TokenType::NUMBER;
    }
}

containers::Vector<std::string> get_variable_names(iter_token begin, iter_token end) {
    containers::Vector<std::string> names;
    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::VARIABLE) {
            auto found = std::any_of(names.begin(), names.end(),
                [&](const auto& name) {
                    return name == it->value_;
                });
            if (!found) {
                names.push_back(it->value_);
            }
        }
    }
    return names;
}

} 