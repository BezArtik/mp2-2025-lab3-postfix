#include "arithmetic.h"

// ==================== CONSTRUCTOR AND BASIC METHODS ====================

ArithmeticExpression::ArithmeticExpression(const std::string& expr): original_tokens(parse_string(expr)), resolved_tokens(original_tokens) {
    validate_expression(original_tokens);
}

double ArithmeticExpression::calculate() const {
    List<Token> postfix_tokens = parse_postfix_string(to_postfix());
    Stack<double> numbers;

    for (size_t i = 0; i < postfix_tokens.size(); ++i) {
        const Token& token = postfix_tokens.get(i);
        switch (token.type) {
        case TokenType::NUMBER:
        case TokenType::CONST: 
        {
            numbers.push(string_to_double(token.value));
            break;
        }
        case TokenType::UNARY_MINUS: 
        {
            double val = numbers.top(); numbers.pop();
            numbers.push(-val);
            break;
        }
        case TokenType::PLUS:
        case TokenType::BINARY_MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::POW: 
        {
            double right = numbers.top(); numbers.pop();
            double left = numbers.top(); numbers.pop();
            double result = apply_binary_operator(left, right, token.type);
            numbers.push(result);
            break;
        }

        case TokenType::SIN:
        case TokenType::COS:
        case TokenType::TAN:
        case TokenType::SQRT:
        case TokenType::LOG:
        case TokenType::ACOS:  
        case TokenType::ASIN:
        case TokenType::ATAN:
        case TokenType::SINH:
        case TokenType::COSH:
        case TokenType::TANH:
        case TokenType::ACOSH:
        case TokenType::ASINH: 
        case TokenType::ATANH:
        case TokenType::FACT:
        {
            double arg = numbers.top(); numbers.pop();
            double result = apply_function(token.type, arg);
            numbers.push(result);
            break;
        }
        }
    }

    return numbers.top();
}

std::string ArithmeticExpression::to_postfix() const {
    Stack<Token> stack_operator;
    std::string res_str;

    for (size_t i = 0; i < resolved_tokens.size(); ++i) {
        Token token = resolved_tokens.get(i);
        switch (token.type) {
        case TokenType::NUMBER:
        case TokenType::CONST: 
        {
            res_str += token.value + " ";
            break;
        }
        case TokenType::SIN:
        case TokenType::COS:
        case TokenType::TAN:
        case TokenType::SQRT:
        case TokenType::LOG: 
        case TokenType::ACOS:
        case TokenType::ASIN:
        case TokenType::ATAN:
        case TokenType::SINH:
        case TokenType::COSH:
        case TokenType::TANH:
        case TokenType::ACOSH:
        case TokenType::ASINH:
        case TokenType::ATANH:
        case TokenType::FACT:
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
            while (!stack_operator.empty() && stack_operator.top().type != TokenType::LEFT_PAREN) {
                res_str += token_to_string(stack_operator.top()) + " ";
                stack_operator.pop();
            }
            if (!stack_operator.empty()) {
                stack_operator.pop();
            }
            if (!stack_operator.empty() && is_function_token(stack_operator.top().type)) {
                res_str += token_to_string(stack_operator.top()) + " ";
                stack_operator.pop();
            }
            break;
        }
        case TokenType::PLUS:
        case TokenType::BINARY_MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::UNARY_MINUS:
        case TokenType::POW: 
        {
            while (!stack_operator.empty() && get_priority(stack_operator.top().type) >= get_priority(token.type)) {
                res_str += token_to_string(stack_operator.top()) + " ";
                stack_operator.pop();
            }
            stack_operator.push(token);
            break;
        }
        }
    }

    while (!stack_operator.empty()) {
        res_str += token_to_string(stack_operator.top()) + " ";
        stack_operator.pop();
    }
    return res_str;
}

// ==================== WORKING WITH VARIABLES ====================

bool ArithmeticExpression::has_variables() const noexcept {
    for (size_t i = 0; i < original_tokens.size(); ++i) {
        const Token& token = original_tokens.get(i);
        if (token.type == TokenType::VARIABLE) {
            return true;
        }
    }
    return false;
}

List<std::string> ArithmeticExpression::get_variable_names() const noexcept {
    List<std::string> names;
    for (size_t i = 0; i < original_tokens.size(); ++i) {
        const Token& token = original_tokens.get(i);
        if (token.type == TokenType::VARIABLE) {
            bool found = false;
            for (size_t j = 0; j < names.size(); ++j) {
                if (names.get(j) == token.value) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                names.push_back(token.value);
            }
        }
    }
    return names;
}

void ArithmeticExpression::set_variable(const std::string& var, double value) {
    for (size_t i = 0; i < resolved_tokens.size(); ++i) {
        Token& token = resolved_tokens.get(i);
        if (token.type == TokenType::VARIABLE && token.value == var) {
            token.type = TokenType::NUMBER;
            token.value = std::to_string(value);
        }
    }
}

// ==================== PARSING ====================

List<ArithmeticExpression::Token> ArithmeticExpression::parse_string(const std::string& expr) const {
    List<Token> result_list;
    size_t i = 0;

    while (i < expr.length()) {
        if (is_operator(expr[i]) || expr[i] == '(' || expr[i] == ')') {
            std::string char_str(1,expr[i]);
            TokenType op_type = string_to_token(char_str);

            if (expr[i] == '-') {
                op_type = is_unary(result_list) ? TokenType::UNARY_MINUS : TokenType::BINARY_MINUS;
            }

            result_list.push_back(Token(op_type, ""));
            ++i;
        }
        else if (is_digit(expr[i])) {
            std::string number;
            while (i < expr.length() && (is_digit(expr[i]) || is_dot(expr[i]))) {
                number += expr[i];
                ++i;
            }
            result_list.push_back(Token(TokenType::NUMBER, number));
        }
        else if (is_letter(expr[i])) {
            std::string identifier;
            while (i < expr.length() && is_variable(expr[i])) {
                identifier += expr[i];
                ++i;
            }

            TokenType identified_type = string_to_token(identifier);

            if (i < expr.length() && expr[i] == '(') {
                if (is_function_token(identified_type)) {
                    result_list.push_back(Token(identified_type, ""));
                    result_list.push_back(Token(TokenType::LEFT_PAREN, "("));
                    ++i;
                }
                else {
                    throw std::invalid_argument("Unknown function: " + identifier);
                }
            }
            else {
                if (is_constant(identifier)) {
                    result_list.push_back(Token(TokenType::CONST, identifier));
                }
                else {
                    result_list.push_back(Token(TokenType::VARIABLE, identifier));
                }
            }
        }
        else if (is_space(expr[i])) {
            ++i;
        }
        else {
            throw std::invalid_argument("Unexpected character: " + std::string(1, expr[i]));
        }
    }
    return result_list;
}

List<ArithmeticExpression::Token> ArithmeticExpression::parse_postfix_string(const std::string& expr) const {
    List<Token> result_list;
    size_t i = 0;

    while (i < expr.length()) {
        while (i < expr.length() && is_space(expr[i])) {
            ++i;
        }
        if (i >= expr.length()) break;

        size_t start = i;
        while (i < expr.length() && !is_space(expr[i])) {
            ++i;
        }

        std::string token_str;
        for (size_t j = start; j < i; ++j) {
            token_str += expr[j];
        }
        TokenType type = string_to_token(token_str);
        result_list.push_back(Token(type, type == TokenType::NUMBER ? token_str : ""));
    }
    return result_list;
}

// ==================== AUXILIARY METHODS ====================

int ArithmeticExpression::get_priority(TokenType op) const noexcept {
    switch (op) {
    case TokenType::SIN: 
    case TokenType::COS: 
    case TokenType::TAN: 
    case TokenType::SQRT: 
    case TokenType::LOG:
    case TokenType::ACOS:
    case TokenType::ASIN:
    case TokenType::ATAN:
    case TokenType::SINH:
    case TokenType::COSH:
    case TokenType::TANH:
    case TokenType::ACOSH:
    case TokenType::ASINH:
    case TokenType::ATANH:
    case TokenType::FACT:
        return 5;
    case TokenType::POW:
        return 4;
    case TokenType::MUL: 
    case TokenType::DIV:
        return 3;
    case TokenType::UNARY_MINUS:
        return 2;
    case TokenType::PLUS: 
    case TokenType::BINARY_MINUS:
        return 1;
    case TokenType::LEFT_PAREN:
        return 0;
    default:
        return -1;
    }
}

std::string ArithmeticExpression::token_to_string(const Token& token) const noexcept {
    switch (token.type) {
    case TokenType::NUMBER:       return token.value;
    case TokenType::PLUS:         return "+";
    case TokenType::BINARY_MINUS: return "-";
    case TokenType::MUL:          return "*";
    case TokenType::DIV:          return "/";
    case TokenType::POW:          return "^";
    case TokenType::UNARY_MINUS:  return "~";
    case TokenType::SIN:          return "sin";
    case TokenType::COS:          return "cos";
    case TokenType::TAN:          return "tan";
    case TokenType::SQRT:         return "sqrt";
    case TokenType::LOG:          return "log";
    case TokenType::ACOS:         return "acos";
    case TokenType::ASIN:         return "asin";
    case TokenType::ATAN:         return "atan";
    case TokenType::SINH:         return "sinh";
    case TokenType::COSH:         return "cosh";
    case TokenType::TANH:         return "tanh";
    case TokenType::ACOSH:        return "acosh";
    case TokenType::ASINH:        return "asinh";
    case TokenType::ATANH:        return "atanh";
    case TokenType::FACT:         return "fact";
    case TokenType::LEFT_PAREN:   return "(";
    case TokenType::RIGHT_PAREN:  return ")";
    default:                      return "";
    }
}

TokenType ArithmeticExpression::string_to_token(const std::string& str) const noexcept {
    if (str == "~")     return TokenType::UNARY_MINUS;
    if (str == "+")     return TokenType::PLUS;
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
    if (str == "fact")  return TokenType::FACT;

    return TokenType::NUMBER;
}



// ==================== MATHEMATICAL OPERATIONS ====================

double ArithmeticExpression::fact(double num) const {
    uint64_t num_int = static_cast<uint64_t>(num);
    uint64_t result = 1;
    for (uint64_t i = 2; i <= num_int; ++i) {
        result *= i;
    }
    return static_cast<double>(result);
}

double ArithmeticExpression::apply_binary_operator(double left, double right, TokenType op) const {
    switch (op) {
    case TokenType::MUL: return left * right;
    case TokenType::DIV:
        if (right == 0.0) throw std::invalid_argument("Division by zero");
        return left / right;
    case TokenType::PLUS: return left + right;
    case TokenType::BINARY_MINUS: return left - right;
    case TokenType::POW:
        if (left <= 0.0) throw std::invalid_argument("Expression is not defined");
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
        if (arg <= 0.0) throw std::invalid_argument("Expression is not defined");
        return std::log(arg);
    case TokenType::SQRT:
        if (arg < 0.0) throw std::invalid_argument("Expression is not defined");
        return std::sqrt(arg);
    case TokenType::ASIN:
        if (arg < -1.0 || arg > 1.0) throw std::invalid_argument("Expression is not defined");
        return std::asin(arg);
    case TokenType::ACOS:
        if (arg < -1.0 || arg > 1.0) throw std::invalid_argument("Expression is not defined");
        return std::acos(arg);
    case TokenType::ATAN:  return std::atan(arg);
    case TokenType::SINH:  return std::sinh(arg);
    case TokenType::COSH:  return std::cosh(arg);
    case TokenType::TANH:  return std::tanh(arg);
    case TokenType::ASINH: return std::asinh(arg);
    case TokenType::ACOSH:
        if (arg < 1.0) throw std::invalid_argument("Expression is not defined");
        return std::acosh(arg);
    case TokenType::ATANH:
        if (std::abs(arg) >= 1.0) throw std::invalid_argument("Expression is not defined");
        return std::atanh(arg);
    case TokenType::FACT: 
        if (std::floor(arg) != arg || arg < 0.0) throw std::invalid_argument("Epression is not defined");
        return fact(arg);
    default: throw std::invalid_argument("Unknown function type");
    }
}

double ArithmeticExpression::apply_constant(const std::string& cnst) const {
    if (cnst == "pi") return std::acos(-1.0);
    if (cnst == "e")  return std::exp(1.0);
    else {
        throw std::runtime_error("Uknown constant: " + cnst);
    }
}

double ArithmeticExpression::string_to_double(const std::string& expr) const {
    if (is_constant(expr)) {
        return apply_constant(expr);
    }
    else {
        return std::stod(expr);
    }
}

// ==================== CHECKS =========================

bool ArithmeticExpression::is_constant(const std::string& c) const noexcept { return c == "pi" || c == "e"; }

bool ArithmeticExpression::is_digit(char c) const noexcept { return c >= '0' && c <= '9'; }

bool ArithmeticExpression::is_dot(char c) const noexcept { return c == '.'; }

bool ArithmeticExpression::is_space(char c) const noexcept { return c == ' ' || c == '\t' || c == '\n'; }

bool ArithmeticExpression::is_operator(char c) const noexcept { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }

bool ArithmeticExpression::is_letter(char c) const noexcept { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

bool ArithmeticExpression::is_variable(char c) const noexcept { return is_letter(c) || is_digit(c) || c == '_'; }

bool ArithmeticExpression::is_function_token(TokenType type) const noexcept { return get_priority(type) == 5; }

bool ArithmeticExpression::is_operator_token(TokenType op) const noexcept { return get_priority(op) <= 4 && get_priority(op) >= 1; }

bool ArithmeticExpression::is_unary(const List<Token>& tokens) const noexcept {
    if (tokens.is_empty()) return true;
    const TokenType& last = tokens.back().type;
    return last == TokenType::LEFT_PAREN ||
           last == TokenType::PLUS ||
           last == TokenType::MUL  ||
           last == TokenType::DIV  ||
           is_function_token(last);
}

//===================== VALIDATE =====================

void ArithmeticExpression::validate_expression(const List<Token>& tokens) const {
    if (tokens.is_empty()) {
        throw std::invalid_argument("Empty expression");
    }

    validate_brackets(tokens);
    validate_operators(tokens);
    validate_number(tokens);
}

void ArithmeticExpression::validate_brackets(const List<Token>& tokens) const {
    int balance = 0;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens.get(i);

        if (token.type == TokenType::LEFT_PAREN) {
            ++balance;
        }
        else if (token.type == TokenType::RIGHT_PAREN) {
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

void ArithmeticExpression::validate_operators(const List<Token>& tokens) const {
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens.get(i);

        if (i > 0 && is_operator_token(token.type) && is_operator_token(tokens.get(i - 1).type)) {
            throw std::invalid_argument("Consecutive operators");
        }

        if (i == tokens.size() - 1 && is_operator_token(token.type)) {
            throw std::invalid_argument("Expression ends with operator");
        }

        if (token.type == TokenType::UNARY_MINUS && i > 0) {
            const Token& prev = tokens.get(i - 1);
            if (prev.type == TokenType::NUMBER || prev.type == TokenType::CONST ||
                prev.type == TokenType::VARIABLE || prev.type == TokenType::RIGHT_PAREN) {
                throw std::invalid_argument("Misplaced unary minus");
            }
        }
    }
}

void ArithmeticExpression::validate_number(const List<Token>& tokens) const {
    for (size_t j = 0; j < tokens.size(); ++j) {
        const Token& token = tokens.get(j);

        if (token.type == TokenType::NUMBER) {
            size_t i = 0, counter_dot = 0;


            if (token.value.length() == 0) {
                throw std::invalid_argument("Empty number");
            }

            if (token.value[i] == '-') {
                ++i;
                if (i >= token.value.length()) {
                    throw std::invalid_argument("Invalid number format: " + token.value);
                }
            }

            if (i < token.value.length() && is_dot(token.value[i])) {
                throw std::invalid_argument("Invalid number format: " + token.value);
            }

            bool has_digit = false;
            while (i < token.value.length()) {
                if (is_dot(token.value[i])) {
                    if (counter_dot >= 1) {
                        throw std::invalid_argument("Invalid number format: " + token.value);
                    }
                    if (i + 1 >= token.value.length() || !is_digit(token.value[i + 1])) {
                        throw std::invalid_argument("Invalid number format: " + token.value);
                    }
                    ++counter_dot;
                    ++i;
                }
                else if (is_digit(token.value[i])) {
                    has_digit = true;
                    ++i;
                }
                else {
                    throw std::invalid_argument("Invalid number format: " + token.value);
                }
            }

            if (!has_digit) {
                throw std::invalid_argument("Invalid number format: " + token.value);
            }
        }
    }
}

// ==================== INTERFACE ====================

void input_and_calculate() {

    std::string input_string;
    std::cout << "Enter an expression: ";
    std::getline(std::cin, input_string);

    ArithmeticExpression calc(input_string);
    if (calc.has_variables()) {
        List<std::string> vars = calc.get_variable_names();
        for (size_t i = 0; i < vars.size(); ++i) {
            std::string value_expr;
            std::cout << "Enter a value for " << vars.get(i) << ": ";
            std::getline(std::cin, value_expr);

            ArithmeticExpression value_calc(value_expr);
            if (value_calc.has_variables()) {
                throw std::invalid_argument("Nested variables not allowed in variable values");
            }

            double value = value_calc.calculate();
            calc.set_variable(vars.get(i), value);
        }
    }

    std::cout << "Result: " << std::fixed << std::setprecision(7) << calc.calculate() << std::endl;

}