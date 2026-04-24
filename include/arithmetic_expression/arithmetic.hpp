#pragma once
#include "arithmetic_expression/parser.hpp"
#include "arithmetic_expression/validator.hpp"
#include "arithmetic_expression/calculator.hpp"

namespace arithmetic {

class ArithmeticExpression {
public:
    ArithmeticExpression(const std::string& expr);

    double calculate();

    bool has_variables() const noexcept;
    containers::Vector<std::string> get_variable_names() const noexcept;
    void set_variable(const std::string& name, double value);
    double get_variable(const std::string& name) const;

private:
    Parser parser_;
    Validator validator_;
    Calculator calculator_;

    TokenVec original_tokens_;
};

} 