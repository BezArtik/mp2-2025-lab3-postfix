#pragma once
#include "arithmetic_expression/token.hpp"
#include "containers/map.hpp"
#include <string>

namespace arithmetic {

class Calculator {
	using map_vars = containers::Map<std::string, double>;
	using vector_str = containers::Vector<std::string>;
public:
    Calculator() = default;

    void set_variable(const std::string& name, double value);
    double get_variable(const std::string& name) const;
    bool has_variable(const std::string& name) const;

    double calculate(iter_token begin, iter_token end) const;

    static vector_str get_variable_names(iter_token begin, iter_token end);

private:
    double evaluate_postfix(iter_token begin, iter_token end) const;
    double string_to_double(const std::string& str) const;
    double apply_binary_operator(double left, double right, TokenType op) const;
    double apply_function(TokenType func, double arg) const;
    static double apply_constant(const std::string& name);
    vector_tokens to_postfix(iter_token begin, iter_token end) const;

    map_vars variables_;
};

} 