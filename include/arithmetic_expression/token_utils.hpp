#pragma once
#include "arithmetic_expression/token.hpp"

namespace arithmetic::token_utils {

bool is_constant(const std::string& name) noexcept;
bool is_function_token(TokenType type) noexcept;
bool is_operator_token(TokenType type) noexcept;
bool is_unary_operator_token(TokenType type) noexcept;
bool is_operand_token(TokenType type) noexcept;
int32_t get_priority(TokenType op) noexcept;
TokenType string_to_token(const std::string& str) noexcept;
TokenType char_to_token(int8_t c) noexcept;

}