#pragma once
#include "arithmetic_expression/parser.hpp"

namespace arithmetic {

class Validator {
public:

	Validator() = default;

	void validate(TokenIter begin, TokenIter end) const;

private:
	void validate_operands(TokenIter begin, TokenIter end) const;
	void validate_brackets(TokenIter begin, TokenIter end) const;
	void validate_operators(TokenIter begin, TokenIter end) const;
	void validate_number_format(TokenIter begin, TokenIter end) const;
};

}