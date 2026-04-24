#include "arithmetic_expression/calculator.hpp"
#include "arithmetic_expression/token.hpp"
#include "arithmetic_expression/token_utils.hpp"
#include "containers/stack.hpp"
#include "containers/map.hpp"
#include <cmath>
#include <stdexcept>
#include <string>
#include <limits>
#include <exception>
#include <iterator>

namespace arithmetic {

#define FUNCTIONS \
    case TokenType::SIN:   case TokenType::COS:   case TokenType::TAN: \
    case TokenType::SQRT:  case TokenType::LOG: \
    case TokenType::ACOS:  case TokenType::ASIN:  case TokenType::ATAN: \
    case TokenType::SINH:  case TokenType::COSH:  case TokenType::TANH: \
    case TokenType::ACOSH: case TokenType::ASINH: case TokenType::ATANH: 


void Calculator::set_variable(const std::string& name, double value) {
    variables_.insert({ name, value });
}

double Calculator::get_variable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    throw std::invalid_argument("Variable not found: " + name);
}

bool Calculator::has_variable(const std::string& name) const {
    return variables_.find(name) != variables_.end();
}

Calculator::VecStr Calculator::get_variable_names(TokenIter begin, TokenIter end) {
    VecStr names;

    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::VARIABLE) {
            bool found = false;
            for (const auto& name : names) {
                if (name == it->value_) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                names.push_back(it->value_);
            }
        }
    }

    return names;
}

double Calculator::calculate(TokenIter begin, TokenIter end) const {
    auto postfix = to_postfix(begin, end);
    return evaluate_postfix(postfix.begin(), postfix.end());
}

double Calculator::evaluate_postfix(TokenIter begin, TokenIter end) const {
    containers::Stack<double> stack;

    for (auto it = begin; it != end; ++it) {
        switch (it->type_) {
        case TokenType::NUMBER:
        case TokenType::CONST:
        case TokenType::VARIABLE:
            stack.push(string_to_double(it->value_));
            break;

        case TokenType::UNARY_MINUS: {
            double val = stack.top();
            stack.pop();
            stack.push(-val);
            break;
        }

        case TokenType::UNARY_PLUS: break;
            
        case TokenType::BINARY_PLUS:
        case TokenType::BINARY_MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::POW: 
        {
            double right = stack.top(); stack.pop();
            double left = stack.top();  stack.pop();
            stack.push(apply_binary_operator(left, right, it->type_));
            break;
        }

        FUNCTIONS
        {
            double arg = stack.top();
            stack.pop();
            stack.push(apply_function(it->type_, arg));
            break;
        }

        default:
            throw std::invalid_argument("Unexpected token in postfix expression");
        }
    }

    if (stack.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    return stack.top();
}


TokenVec Calculator::to_postfix(TokenIter begin, TokenIter end) const {
    containers::Stack<Token> operator_stack;
    TokenVec output;
    output.reserve(std::distance(begin, end));

    for (auto it = begin; it != end; ++it) {
        switch (it->type_) {
        case TokenType::NUMBER:
        case TokenType::CONST:
        case TokenType::VARIABLE:
            output.push_back(*it);
            break;

        FUNCTIONS
            operator_stack.push(*it);
            break;

        case TokenType::LEFT_PAREN:
            operator_stack.push(*it);
            break;

        case TokenType::RIGHT_PAREN: {
            while (!operator_stack.empty() &&
                operator_stack.top().type_ != TokenType::LEFT_PAREN) {
                output.push_back(operator_stack.top());
                operator_stack.pop();
            }

            if (!operator_stack.empty()) {
                operator_stack.pop();
            }

            if (!operator_stack.empty() &&
                token_utils::is_function_token(operator_stack.top().type_)) {
                output.push_back(operator_stack.top());
                operator_stack.pop();
            }
            break;
        }

        case TokenType::BINARY_PLUS:
        case TokenType::BINARY_MINUS:
        case TokenType::UNARY_MINUS:
        case TokenType::UNARY_PLUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::POW: {
            while (!operator_stack.empty() &&
                token_utils::get_priority(operator_stack.top().type_) >=
                token_utils::get_priority(it->type_)) {
                output.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(*it);
            break;
        }

        default:
            break;
        }
    }

    while (!operator_stack.empty()) {
        output.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return output;
}

#undef FUNCTIONS

// ==================== CONVERSIONS ====================

double Calculator::string_to_double(const std::string& str) const {
    if (token_utils::is_constant(str)) {
        return apply_constant(str);
    }

    auto var_it = variables_.find(str);
    if (var_it != variables_.end()) {
        return var_it->second;
    }

    try {
        return std::stod(str);
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid number or undefined variable: " + str);
    }
}

#define EXPR_NOT_DEFINED throw std::invalid_argument("Expression not defined for given inputs");

double Calculator::apply_binary_operator(double left, double right, TokenType op) const {
    switch (op) {
    case TokenType::BINARY_PLUS:  return left + right;
    case TokenType::BINARY_MINUS: return left - right;
    case TokenType::MUL:          return left * right;
    case TokenType::DIV:
        if (std::abs(right) < std::numeric_limits<double>::epsilon()) EXPR_NOT_DEFINED
        return left / right;
    case TokenType::POW:
        if (left < 0 && right != std::floor(right)) EXPR_NOT_DEFINED
        return std::pow(left, right);
    default:
        throw std::invalid_argument("Unknown binary operator");
    }
}

double Calculator::apply_function(TokenType func, double arg) const {
    switch (func) {
    case TokenType::SIN:   return std::sin(arg);
    case TokenType::COS:   return std::cos(arg);
    case TokenType::TAN:   return std::tan(arg);
    case TokenType::LOG:
        if (arg <= 0.0) EXPR_NOT_DEFINED
        return std::log(arg);
    case TokenType::SQRT:
        if (arg < 0.0) EXPR_NOT_DEFINED
        return std::sqrt(arg);
    case TokenType::ASIN:
        if (arg < -1.0 || arg > 1.0) EXPR_NOT_DEFINED
        return std::asin(arg);
    case TokenType::ACOS:
        if (arg < -1.0 || arg > 1.0) EXPR_NOT_DEFINED
        return std::acos(arg);
    case TokenType::ATAN:  return std::atan(arg);
    case TokenType::SINH:  return std::sinh(arg);
    case TokenType::COSH:  return std::cosh(arg);
    case TokenType::TANH:  return std::tanh(arg);
    case TokenType::ASINH: return std::asinh(arg);
    case TokenType::ACOSH:
        if (arg < 1.0) EXPR_NOT_DEFINED
        return std::acosh(arg);
    case TokenType::ATANH:
        if (std::abs(arg) >= 1.0) EXPR_NOT_DEFINED
        return std::atanh(arg);
    default:
        throw std::invalid_argument("Unknown function");
    }
}

#undef EXPR_NOT_DEFINED

double Calculator::apply_constant(const std::string& name) {
    if (name == "pi") return 3.14159265358979323846;
    if (name == "e")  return 2.71828182845904523536;
    throw std::invalid_argument("Unknown constant: " + name);
}

} 