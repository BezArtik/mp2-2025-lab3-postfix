#include "arithmetic_expression/arithmetic.hpp"
#include "containers/vector.hpp"
#include "containers/stack.hpp"
#include <cmath>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <limits>
#include <exception>
#include <cstdlib>

namespace arithmetic {

#define FUNCTIONS \
    case TokenType::SIN:   case TokenType::COS:   case TokenType::TAN:   \
    case TokenType::SQRT:  case TokenType::LOG:   case TokenType::ACOS:  \
    case TokenType::ASIN:  case TokenType::ATAN:  case TokenType::SINH:  \
    case TokenType::COSH:  case TokenType::TANH:  case TokenType::ACOSH: \
    case TokenType::ASINH: case TokenType::ATANH:

#define OPERATORS \
    case TokenType::BINARY_PLUS: case TokenType::BINARY_MINUS: \
    case TokenType::MUL: case TokenType::DIV: \
    case TokenType::POW:

#define EXPR_NOT_DEFINED throw std::invalid_argument("Expression is not defined");

#define INVALID_NUMBER_FORMAT throw std::invalid_argument("Invalid number format at the position " \
                                + std::to_string(std::distance(begin, it)));

ArithmeticExpression::ArithmeticExpression(const std::string& expr)
    : original_tokens_(parse_string(expr)), resolved_tokens_(original_tokens_) {
    validate_expression(original_tokens_.begin(), original_tokens_.end());
}

double ArithmeticExpression::calculate() const {
    TokenVec postfix_tokens = to_postfix_tokens();
    containers::Stack<double> numbers;

    for (const auto& token : postfix_tokens) {
        switch (token.type_) {
        case TokenType::NUMBER:
        case TokenType::CONST:
        case TokenType::VARIABLE:
        {
            numbers.push(string_to_double(token.value_));
            break;
        }
        case TokenType::UNARY_MINUS:
        {
            double val = numbers.top(); numbers.pop();
            numbers.push(-val);
            break;
        }
        case TokenType::UNARY_PLUS:
        {
            break;
        }
        OPERATORS
        {
            double right = numbers.top(); numbers.pop();
            double left = numbers.top(); numbers.pop();
            numbers.push(apply_binary_operator(left, right, token.type_));
            break;
        }
        FUNCTIONS
        {
            double arg = numbers.top(); numbers.pop();
            numbers.push(apply_function(token.type_, arg));
            break;
        }
        }
    }

    return numbers.top();
}

bool ArithmeticExpression::has_variables() const noexcept {
    return std::any_of(original_tokens_.begin(), original_tokens_.end(),
        [](const auto& t) { return t.type_ == TokenType::VARIABLE; });
}

containers::Vector<std::string> ArithmeticExpression::get_variable_names() const noexcept {
    containers::Vector<std::string> names;

    for (const auto& token : original_tokens_) {
        if (token.type_ == TokenType::VARIABLE) {
            if (std::find(names.begin(), names.end(), token.value_) == names.end()) {
                names.push_back(token.value_);
            }
        }
    }
    return names;
}

void ArithmeticExpression::set_variable(const std::string& name, double value) {
    bool found = false;
    for (auto& token : resolved_tokens_) {
        if (token.type_ == TokenType::VARIABLE && token.value_ == name) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::invalid_argument("Variable not found: " + name);
    }
    variables_[name] = value;
}

ArithmeticExpression::TokenVec ArithmeticExpression::parse_string(const std::string& expr) const {
    TokenVec result_vector;
    result_vector.reserve(expr.size() / 2);

    auto it = expr.begin();
    size_t current_pos = 0;

    enum class Expect : uint8_t { OPERAND, OPERATOR };
    Expect expect = Expect::OPERAND;

    while (it != expr.end()) {
        char current_char = *it;

        switch (current_char) {
        case '+': case '-': case '*': case '/':
        case '^': case '(': case ')':
        {
            TokenType op_type;
            std::string op_val;
            bool is_unary_op = false;

            if ((current_char == '-' || current_char == '+') && expect == Expect::OPERAND) {
                is_unary_op = true;
            }

            if (current_char == '-' && is_unary_op) {
                op_type = TokenType::UNARY_MINUS;
                op_val = "~";
                expect = Expect::OPERAND;
            }
            else if (current_char == '+' && is_unary_op) {
                op_type = TokenType::UNARY_PLUS;
                op_val = "$";
                expect = Expect::OPERAND;
            }
            else {
                op_type = char_to_token_type(current_char);
                op_val = std::string(1, current_char);

                if (current_char == ')') {
                    expect = Expect::OPERATOR;
                }
                else if (current_char == '(') {
                    expect = Expect::OPERAND;
                }
                else {
                    expect = Expect::OPERAND;
                }
            }

            result_vector.push_back(Token(op_type, op_val));
            ++current_pos;
            ++it;
            break;
        }

        default:
            if (std::isdigit(current_char)) {
                size_t start_pos = current_pos;
                while (it != expr.end() && (std::isdigit(*it) || *it == '.')) {
                    ++current_pos;
                    ++it;
                }
                result_vector.push_back(Token(TokenType::NUMBER,
                    expr.substr(start_pos, current_pos - start_pos)));
                expect = Expect::OPERATOR;
            }
            else if (std::isalpha(current_char)) {
                size_t start_pos = current_pos;
                while (it != expr.end() && (std::isalnum(*it) || *it == '_')) {
                    ++current_pos;
                    ++it;
                }
                std::string identifier = expr.substr(start_pos, current_pos - start_pos);

                if (it != expr.end() && *it == '(') {
                    TokenType func_type = string_to_token(identifier);
                    if (is_function_token(func_type)) {
                        result_vector.push_back(Token(func_type, identifier));
                        result_vector.push_back(Token(TokenType::LEFT_PAREN, "("));
                        ++current_pos;
                        ++it;
                        expect = Expect::OPERAND;
                    }
                    else {
                        throw std::invalid_argument("Unknown function: " + identifier);
                    }
                }
                else {
                    if (is_constant(identifier)) {
                        result_vector.push_back(Token(TokenType::CONST, identifier));
                    }
                    else {
                        result_vector.push_back(Token(TokenType::VARIABLE, identifier));
                    }
                    expect = Expect::OPERATOR;
                }
            }
            else if (std::isspace(current_char)) {
                ++current_pos;
                ++it;
            }
            else {
                throw std::invalid_argument("Unexpected character: " + std::string(1, current_char));
            }
            break;
        }
    }
    return result_vector;
}

TokenType ArithmeticExpression::char_to_token_type(char c) const noexcept {
    switch (c) {
    case '+': return TokenType::BINARY_PLUS;
    case '-': return TokenType::BINARY_MINUS;
    case '*': return TokenType::MUL;
    case '/': return TokenType::DIV;
    case '^': return TokenType::POW;
    case '(': return TokenType::LEFT_PAREN;
    case ')': return TokenType::RIGHT_PAREN;
    default:  return TokenType::NUMBER;
    }
}

ArithmeticExpression::TokenVec ArithmeticExpression::to_postfix_tokens() const {
    containers::Stack<Token> stack_operator;
    TokenVec result_tokens;

    for (const auto& token : resolved_tokens_) {
        switch (token.type_) {
        case TokenType::NUMBER:
        case TokenType::CONST:
        case TokenType::VARIABLE:
        {
            result_tokens.push_back(token);
            break;
        }
        FUNCTIONS
        {
            stack_operator.push(token);
            break;
        }
        case TokenType::LEFT_PAREN:
        {
            stack_operator.push(token);
            break;
        }
        case TokenType::RIGHT_PAREN:
        {
            while (!stack_operator.empty() && stack_operator.top().type_ != TokenType::LEFT_PAREN) {
                result_tokens.push_back(stack_operator.top());
                stack_operator.pop();
            }
            if (!stack_operator.empty()) {
                stack_operator.pop();
            }
            if (!stack_operator.empty() && is_function_token(stack_operator.top().type_)) {
                result_tokens.push_back(stack_operator.top());
                stack_operator.pop();
            }
            break;
        }
        OPERATORS
        {
            while (!stack_operator.empty() &&
                get_priority(stack_operator.top().type_) >= get_priority(token.type_)) {
                result_tokens.push_back(stack_operator.top());
                stack_operator.pop();
            }
            stack_operator.push(token);
            break;
        }
        }
    }

    while (!stack_operator.empty()) {
        result_tokens.push_back(stack_operator.top());
        stack_operator.pop();
    }
    return result_tokens;
}

int32_t ArithmeticExpression::get_priority(TokenType op) const noexcept {
    switch (op) {
    FUNCTIONS return 5;
    case TokenType::POW: return 4;
    case TokenType::MUL: case TokenType::DIV: return 3;
    case TokenType::UNARY_MINUS: case TokenType::UNARY_PLUS: return 2;
    case TokenType::BINARY_PLUS: case TokenType::BINARY_MINUS: return 1;
    case TokenType::LEFT_PAREN: return 0;
    default: return -1;
    }
}

TokenType ArithmeticExpression::string_to_token(const std::string& str) const noexcept {
    if (str == "$")     return TokenType::UNARY_PLUS;
    if (str == "~")     return TokenType::UNARY_MINUS;
    if (str == "+")     return TokenType::BINARY_PLUS;
    if (str == "-")     return TokenType::BINARY_MINUS;
    if (str == "/")     return TokenType::DIV;
    if (str == "*")     return TokenType::MUL;
    if (str == "^")     return TokenType::POW;
    if (str == "(")     return TokenType::LEFT_PAREN;
    if (str == ")")     return TokenType::RIGHT_PAREN;

    if (str == "sin")   return TokenType::SIN;
    if (str == "cos")   return TokenType::COS;
    if (str == "tan")   return TokenType::TAN;
    if (str == "sqrt")  return TokenType::SQRT;
    if (str == "log")   return TokenType::LOG;
    if (str == "asin")  return TokenType::ASIN;
    if (str == "acos")  return TokenType::ACOS;
    if (str == "atan")  return TokenType::ATAN;
    if (str == "sinh")  return TokenType::SINH;
    if (str == "cosh")  return TokenType::COSH;
    if (str == "tanh")  return TokenType::TANH;
    if (str == "asinh") return TokenType::ASINH;
    if (str == "acosh") return TokenType::ACOSH;
    if (str == "atanh") return TokenType::ATANH;

    return TokenType::NUMBER;
}

double ArithmeticExpression::apply_binary_operator(double left, double right, TokenType op) const {
    switch (op) {
    case TokenType::MUL: return left * right;
    case TokenType::DIV:
        if (std::abs(right) < std::numeric_limits<double>::epsilon()) EXPR_NOT_DEFINED
        return left / right;
    case TokenType::BINARY_PLUS: return left + right;
    case TokenType::BINARY_MINUS: return left - right;
    case TokenType::POW:
        if (left <= 0.0) EXPR_NOT_DEFINED
        return std::pow(left, right);
    default: throw std::invalid_argument("Unknown operator");
    }
}

double ArithmeticExpression::apply_function(TokenType func_type, double arg) const {
    switch (func_type) {
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
    default: throw std::invalid_argument("Unknown function type");
    }
}

double ArithmeticExpression::apply_constant(const std::string& cnst) const {
    if (cnst == "pi") return std::acos(-1.0);
    if (cnst == "e")  return std::exp(1.0);
    return 0.0;
}

double ArithmeticExpression::string_to_double(const std::string& expr) const {
    if (is_constant(expr)) {
        return apply_constant(expr);
    }

    auto var_it = variables_.find(expr);
    if (var_it != variables_.end()) {
        return var_it->second;
    }

    try {
        return std::stod(expr);
    } catch (const std::exception&) {
        throw std::invalid_argument("Invalid number format: " + expr);
    }
}

// ==================== TYPE CHECKS =========================

bool ArithmeticExpression::is_constant(const std::string& c) const noexcept {
    return c == "pi" || c == "e";
}

bool ArithmeticExpression::is_function_token(TokenType type) const noexcept {
    return get_priority(type) == 5;
}

bool ArithmeticExpression::is_operator_token(TokenType op) const noexcept {
    return get_priority(op) >= 1 && get_priority(op) <= 4;
}

bool ArithmeticExpression::is_unary_operator_token(TokenType op) const noexcept {
    return get_priority(op) == 2;
}

bool ArithmeticExpression::is_operand_token(TokenType type) const noexcept {
    return type == TokenType::NUMBER || type == TokenType::CONST || type == TokenType::VARIABLE;
}

//===================== VALIDATION =====================

void ArithmeticExpression::validate_expression(TokenIter begin, TokenIter end) const {
    if (begin == end) {
        throw std::invalid_argument("Empty expression");
    }
    validate_operands(begin, end);
    validate_operators(begin, end);
    validate_brackets(begin, end);
    validate_number_format(begin, end);
}

void ArithmeticExpression::validate_operands(TokenIter begin, TokenIter end) const {
    for (auto it = begin; it != end; ++it) {
        if (it != begin && is_operator_token(it->type_) && !is_unary_operator_token(it->type_)) {
            auto prev = std::prev(it);
            if (!is_operand_token(prev->type_) && prev->type_ == TokenType::LEFT_PAREN) {
                throw std::invalid_argument("Missing left operand at the position " +
                    std::to_string(std::distance(begin, prev)));
            }
        }
        auto next = std::next(it);
        if (next != end && is_operator_token(it->type_)) {
            if (!is_operand_token(next->type_) && !is_function_token(next->type_) &&
                next->type_ == TokenType::RIGHT_PAREN) {
                throw std::invalid_argument("Missing right operand at the position " +
                    std::to_string(std::distance(begin, next)));
            }
        }
        if (next != end && is_operand_token(it->type_)) {
            if (is_operand_token(next->type_)) {
                throw std::invalid_argument("Consecutive operands at the position " +
                    std::to_string(std::distance(begin, next)));
            }
        }
    }
}

void ArithmeticExpression::validate_brackets(TokenIter begin, TokenIter end) const {
    auto check_empty_parens = [begin, end](auto it) {
        auto next = std::next(it);
        return next != end && next->type_ == TokenType::RIGHT_PAREN;
        };

    int32_t balance = 0;
    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::LEFT_PAREN) {
            ++balance;
            if (check_empty_parens(it)) {
                throw std::invalid_argument("Empty parentheses at the position " +
                    std::to_string(std::distance(begin, it)));
            }
        }
        else if (it->type_ == TokenType::RIGHT_PAREN) {
            --balance;
            if (balance < 0) {
                throw std::invalid_argument("Unmatched closing parenthesis");
            }
        }
    }

    if (balance > 0) {
        throw std::invalid_argument("Unmatched opening parenthesis");
    }
}

void ArithmeticExpression::validate_operators(TokenIter begin, TokenIter end) const {
    if (is_operator_token(begin->type_) && !is_unary_operator_token(begin->type_)) {
        throw std::invalid_argument("Binary operator at beginning");
    }
    auto last = std::prev(end);
    if (is_operator_token(last->type_) && !is_unary_operator_token(last->type_)) {
        throw std::invalid_argument("Binary operators at end");
    }
    for (auto it = begin; it != end; ++it) {
        auto next = std::next(it);
        if (is_operator_token(it->type_) && next != end) {
            if (is_operator_token(next->type_)) {
                throw std::invalid_argument("Consecutive operators at the position " +
                    std::to_string(std::distance(begin, next)));
            }
        }
        if (it->type_ == TokenType::LEFT_PAREN && it != begin) {
            auto prev = std::prev(it);
            if (is_operand_token(prev->type_) || prev->type_ == TokenType::RIGHT_PAREN) {
                throw std::invalid_argument("Missing operator before '(' at the position " +
                    std::to_string(std::distance(begin, prev)));
            }
        }
        if (it->type_ == TokenType::RIGHT_PAREN && next != end) {
            if (is_operand_token(next->type_) || next->type_ == TokenType::LEFT_PAREN) {
                throw std::invalid_argument("Missing operator after ')' at the position " +
                    std::to_string(std::distance(begin, next)));
            }
        }
    }
}

void ArithmeticExpression::validate_number_format(TokenIter begin, TokenIter end) const {
    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::NUMBER) {
            const std::string& value = it->value_;

            if (value.empty()) {
                INVALID_NUMBER_FORMAT
            }

            std::size_t pos = 0;
            try {
                [[maybe_unused]] double temp = std::stod(value, &pos);
            }
            catch (const std::exception&) {
                INVALID_NUMBER_FORMAT
            }

            if (pos != value.length()) INVALID_NUMBER_FORMAT
            

            if (value == "." || value == "+" || value == "-" ||
                (value.length() > 1 && value[0] == '0' && value[1] != '.')) {
                INVALID_NUMBER_FORMAT
            }
        }
    }
}


#undef FUNCTIONS
#undef OPERATORS
#undef EXPR_NOT_DEFINED
#undef INVALID_NUMBER_FORMAT

}