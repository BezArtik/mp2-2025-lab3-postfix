#include "arithmetic_expression/validator.hpp"
#include "arithmetic_expression/token.hpp"
#include "arithmetic_expression/token_utils.hpp"
#include <string>
#include <stdexcept>
#include <iterator>
#include <cstdint>
#include <exception>

namespace arithmetic {

void Validator::validate(iter_token begin, iter_token end) const {
    validate_operands(begin, end);
    validate_brackets(begin, end);
    validate_operators(begin, end);
    validate_number_format(begin, end);
}

#define INVALID_OPERAND_FORMAT(it) \
        throw std::invalid_argument("Invalid operand format at the position " \
        + std::to_string(std::distance(begin, it)));

void Validator::validate_operands(iter_token begin, iter_token end) const {
    for (auto it = begin; it != end; ++it) {
        if (it != begin && token_utils::is_operator_token(it->type_) && 
            !token_utils::is_unary_operator_token(it->type_)) {
            auto prev = std::prev(it);
            if (!token_utils::is_operand_token(prev->type_) && 
                prev->type_ == TokenType::LEFT_PAREN) {
                INVALID_OPERAND_FORMAT(prev)
            }
        }
        auto next = std::next(it);
        if (next != end && token_utils::is_operator_token(it->type_)) {
            if (!token_utils::is_operand_token(next->type_) && 
                !token_utils::is_function_token(next->type_) &&
                next->type_ == TokenType::RIGHT_PAREN) {
				INVALID_OPERAND_FORMAT(next)
            }
        }
        if (next != end && token_utils::is_operand_token(it->type_)) {
            if (token_utils::is_operand_token(next->type_)) INVALID_OPERAND_FORMAT(next)
        }
    }
}

#undef INVALID_OPERAND_FORMAT

#define INVALID_BRACKET_FORMAT throw std::invalid_argument("Invalid bracket format");

void Validator::validate_brackets(iter_token begin, iter_token end) const {
    auto empty_parens = [begin, end](auto it) {
        auto next = std::next(it);
        return next != end && next->type_ == TokenType::RIGHT_PAREN;
        };

    int32_t balance = 0;
    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::LEFT_PAREN) {
            ++balance;
            if (empty_parens(it)) INVALID_BRACKET_FORMAT
        } else if (it->type_ == TokenType::RIGHT_PAREN) {
            --balance;
            if (balance < 0) INVALID_BRACKET_FORMAT
        }
    }

    if (balance > 0) INVALID_BRACKET_FORMAT
}

#define INVALID_OPERATOR_FORMAT(it) \
        throw std::invalid_argument("Invalid operator format at the position " \
        + std::to_string(std::distance(begin, it)));

void Validator::validate_operators(iter_token begin, iter_token end) const {
    if (token_utils::is_operator_token(begin->type_) && 
        !token_utils::is_unary_operator_token(begin->type_)) {
		INVALID_OPERATOR_FORMAT(begin)
    }
    auto last = std::prev(end);
    if (token_utils::is_operator_token(last->type_) && 
        !token_utils::is_unary_operator_token(last->type_)) {
		INVALID_OPERATOR_FORMAT(last)
    }
    for (auto it = begin; it != end; ++it) {
        auto next = std::next(it);
        if (token_utils::is_operator_token(it->type_) && next != end) {
            if (token_utils::is_operator_token(next->type_)) {
				INVALID_OPERATOR_FORMAT(next)
            }
        }
        if (it->type_ == TokenType::LEFT_PAREN && it != begin) {
            auto prev = std::prev(it);
            if (token_utils::is_operand_token(prev->type_) || prev->type_ == TokenType::RIGHT_PAREN) {
				INVALID_OPERATOR_FORMAT(prev)
            }
        }
        if (it->type_ == TokenType::RIGHT_PAREN && next != end) {
            if (token_utils::is_operand_token(next->type_) || next->type_ == TokenType::LEFT_PAREN) {
                INVALID_OPERATOR_FORMAT(next)
            }
        }
    }
}

#undef INVALID_BRACKET_FORMAT

#undef INVALID_OPERATOR_FORMAT

#define INVALID_NUMBER_FORMAT \
        throw std::invalid_argument("Invalid number format at the position " \
        + std::to_string(std::distance(begin, it)));

void Validator::validate_number_format(iter_token begin, iter_token end) const {
    for (auto it = begin; it != end; ++it) {
        if (it->type_ == TokenType::NUMBER) {
            const std::string& value = it->value_;

            if (value.empty()) INVALID_NUMBER_FORMAT
                
            std::size_t pos = 0;
            try {
                [[maybe_unused]] double temp = std::stod(value, &pos);
            } catch (const std::exception&) {
                INVALID_NUMBER_FORMAT
            }

            if (pos != value.length()) INVALID_NUMBER_FORMAT

            if (value == "." || value == "+" || value == "-" ||
                (value.length() > 1 && value[0] == '0' && value[1] != '.')) INVALID_NUMBER_FORMAT
                
            
        }
    }
}

#undef INVALID_NUMBER_FORMAT

}