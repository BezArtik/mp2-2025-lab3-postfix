#pragma once
#include "arithmetic_expression/token.hpp"
#include <string>

namespace arithmetic {

class Parser {
public:
    Parser() = default;

    TokenVec parse(const std::string& expr);

private:
    enum class Expect { OPERAND, OPERATOR };

    static int8_t classify_char(char c) noexcept;
    TokenType handle_operator(char c, Expect& expect);
};

} 