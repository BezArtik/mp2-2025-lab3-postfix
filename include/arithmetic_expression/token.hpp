#pragma once
#include "containers/vector.hpp"
#include <string>

namespace arithmetic {

enum class TokenType : uint8_t {
	NUMBER, BINARY_PLUS, BINARY_MINUS, MUL, DIV, POW, UNARY_MINUS, UNARY_PLUS,
	SIN, COS, TAN, SQRT, LOG, ACOS, ASIN, ATAN,
	SINH, COSH, TANH, ASINH, ACOSH, ATANH,
	LEFT_PAREN, RIGHT_PAREN,
	VARIABLE, CONST
};

struct Token {
	TokenType type_;
	std::string value_;
	Token(TokenType t, std::string v) : type_(t), value_(std::move(v)) {}
};

using TokenVec = containers::Vector<Token>;
using TokenIter = typename TokenVec::const_iterator;

};