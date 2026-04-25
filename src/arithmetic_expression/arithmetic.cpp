#include "arithmetic_expression/arithmetic.hpp"
#include "arithmetic_expression/calculator.hpp"
#include "arithmetic_expression/token_utils.hpp"
#include "containers/vector.hpp"
#include <string>

namespace arithmetic {

ArithmeticExpression::ArithmeticExpression(const std::string& expr) {
    original_tokens_ = parser_.parse(expr);
    validator_.validate(original_tokens_.begin(), original_tokens_.end());
}

double ArithmeticExpression::calculate() {
    return calculator_.calculate(original_tokens_.begin(), original_tokens_.end());
}

bool ArithmeticExpression::has_variables() const noexcept {
	return !token_utils::get_variable_names(original_tokens_.begin(), original_tokens_.end()).empty();
}

containers::Vector<std::string> ArithmeticExpression::get_variable_names() const noexcept {
    return token_utils::get_variable_names(original_tokens_.begin(), original_tokens_.end());
}

void ArithmeticExpression::set_variable(const std::string& name, double value) {
    calculator_.set_variable(name, value);
}

double ArithmeticExpression::get_variable(const std::string& name) const {
    return calculator_.get_variable(name);
}

} 