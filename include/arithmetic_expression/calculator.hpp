#pragma once
#include "arithmetic_expression/token.hpp"
#include "containers/map.hpp"
#include <string>

namespace arithmetic {

class Calculator {
	using MapVars = containers::Map<std::string, double>;
	using VecStr = containers::Vector<std::string>;
public:
    Calculator() = default;

    void set_variable(const std::string& name, double value);
    double get_variable(const std::string& name) const;
    bool has_variable(const std::string& name) const;

    double calculate(TokenIter begin, TokenIter end) const;

    static VecStr get_variable_names(TokenIter begin, TokenIter end);

private:
    double evaluate_postfix(TokenIter begin, TokenIter end) const;
    double string_to_double(const std::string& str) const;
    double apply_binary_operator(double left, double right, TokenType op) const;
    double apply_function(TokenType func, double arg) const;
    static double apply_constant(const std::string& name);
    TokenVec to_postfix(TokenIter begin, TokenIter end) const;

    MapVars variables_;
};

} 