#pragma once
#include "arithmetic_expression/parser.hpp"

namespace arithmetic {

class Validator {
public:

	Validator() = default;

	void validate(iter_token begin, iter_token end) const;

private:
	void validate_operands(iter_token begin, iter_token end) const;
	void validate_brackets(iter_token begin, iter_token end) const;
	void validate_operators(iter_token begin, iter_token end) const;
	void validate_number_format(iter_token begin, iter_token end) const;
};

}