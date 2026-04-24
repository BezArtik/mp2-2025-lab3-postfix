#include "arithmetic_expression/parser.hpp"
#include "arithmetic_expression/token.hpp"
#include "arithmetic_expression/token_utils.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <cstdint>

namespace arithmetic {

TokenVec Parser::parse(const std::string& expr) {
    TokenVec result_vector;
    result_vector.reserve(expr.size() / 2);

    auto it = expr.begin();
    const auto end = expr.end();
    size_t current_pos = 0;
    Expect expect = Expect::OPERAND;

    while (it != end) {
        int8_t current_char = *it;
        int8_t char_type = classify_char(current_char);

        switch (char_type) {
        case 'O': 
        {  
            TokenType op_type = handle_operator(current_char, expect);
            std::string op_val;

            if (op_type == TokenType::UNARY_MINUS) {
                op_val = "~";
            } else if (op_type == TokenType::UNARY_PLUS) {
                op_val = "$";
            } else {
                op_val = std::string(1, current_char);
            }

            result_vector.push_back(Token(op_type, std::move(op_val)));
            ++current_pos;
            ++it;
            break;
        }

        case 'D': 
        {  
            size_t start_pos = current_pos;
            while (it != end && (std::isdigit(*it) || *it == '.')) {
                ++current_pos;
                ++it;
            }
            size_t length = current_pos - start_pos;
            result_vector.push_back(Token(TokenType::NUMBER,
                std::string(it - length, it)));
            expect = Expect::OPERATOR;
            break;
        }

        case 'A': 
        {  
            size_t start_pos = current_pos;
            while (it != end && (std::isalnum(*it) || *it == '_')) {
                ++current_pos;
                ++it;
            }
            size_t length = current_pos - start_pos;
            std::string identifier(it - length, it);

            if (it != end && *it == '(') {
                TokenType func_type = token_utils::string_to_token(identifier);
                if (token_utils::is_function_token(func_type)) {
                    result_vector.push_back(Token(func_type, std::move(identifier)));
                    result_vector.push_back(Token(TokenType::LEFT_PAREN, "("));
                    ++current_pos;
                    ++it;
                    expect = Expect::OPERAND;
                } else {
                    throw std::invalid_argument("Unknown function: " + identifier);
                }
            } else {
                TokenType type = token_utils::is_constant(identifier) ?
                    TokenType::CONST : TokenType::VARIABLE;
                result_vector.push_back(Token(type, std::move(identifier)));
                expect = Expect::OPERATOR;
            }
            break;
        }

        case 'S': 
        {  
            ++current_pos;
            ++it;
            break;
        }

        default:
        { 
            throw std::invalid_argument("Unexpected character: " +
                std::string(1, current_char));
        }
        }
    }

    return result_vector;
}

TokenType Parser::handle_operator(char c, Expect& expect) {
    bool is_unary = false;

    if ((c == '-' || c == '+') && expect == Expect::OPERAND) {
        is_unary = true;
    }

    if (is_unary) {
        expect = Expect::OPERAND;
        return (c == '-') ? TokenType::UNARY_MINUS : TokenType::UNARY_PLUS;
    } else {
        if (c == ')') {
            expect = Expect::OPERATOR;
        } else if (c == '(') {
            expect = Expect::OPERAND;
        } else {
            expect = Expect::OPERAND;
        }
        return token_utils::char_to_token(c);
    }
}

int8_t Parser::classify_char(char c) noexcept {
    if (c == '+' || c == '-'  || c == '*' || c == '/' 
        || c == '^' || c == '(' || c == ')') return 'O';       
    if (std::isdigit(c)) return 'D';    
    if (std::isalpha(c)) return 'A';    
    if (std::isspace(c)) return 'S';    
    return 'I';  
}

} 