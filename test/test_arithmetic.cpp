#include <gtest.h>
#include "arithmetic.h"


TEST(ArithmeticExpression, can_create_arithmetic_expression) {
	ASSERT_NO_THROW(ArithmeticExpression expr("1+2"));
}
TEST(ArithmeticExpression, cant_create_empty_arithmetic_expression) {
	ASSERT_ANY_THROW(ArithmeticExpression expr(""));
}

// ======================= OPERATORS ==========================

TEST(ArithmeticExpression, can_calculate_expression_add) {
	ArithmeticExpression expr1("2+6");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 8.0);
}
TEST(ArithmeticExpression, can_calculate_expression_sub) {
	ArithmeticExpression expr1("2-6");
	EXPECT_DOUBLE_EQ(expr1.calculate(), -4.0);
}
TEST(ArithmeticExpression, can_calculate_expression_mul) {
	ArithmeticExpression expr1("2*6");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 12.0);
}
TEST(ArithmeticExpression, can_calculate_expression_div) {
	ArithmeticExpression expr1("2/6");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 2.0/6.0);
}
TEST(ArithmeticExpression, can_calculate_expression_pow) {
	ArithmeticExpression expr1("2^6");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 64.0);
}
TEST(ArithmeticExpression, can_calculate_expression_unary) {
	ArithmeticExpression expr1("-1+2");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 1.0);
}
TEST(ArithmeticExpression, can_calculate_expression_with_paren) {
	ArithmeticExpression expr1("(2+1)*4");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 12.0);
}

// ========================= FUNCTION ===================================

TEST(ArithmeticExpression, can_calculate_expression_with_sin) {
	ArithmeticExpression expr1("sin(1/2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::sin(1.0 / 2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_cos) {
	ArithmeticExpression expr1("cos(1/3)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::cos(1.0 / 3.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_tan) {
	ArithmeticExpression expr1("tan(3/4)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::tan(3.0 / 4.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_sinh) {
	ArithmeticExpression expr1("sinh(1/2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::sinh(1.0 / 2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_cosh) {
	ArithmeticExpression expr1("cosh(1/3)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::cosh(1.0 / 3.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_tanh) {
	ArithmeticExpression expr1("tanh(3/4)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::tanh(3.0 / 4.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_asin) {
	ArithmeticExpression expr1("asin(1/2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::asin(1.0 / 2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_acos) {
	ArithmeticExpression expr1("acos(1/3)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::acos(1.0 / 3.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_atan) {
	ArithmeticExpression expr1("atan(3/4)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::atan(3.0 / 4.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_asinh) {
	ArithmeticExpression expr1("asinh(1/2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::asinh(1.0 / 2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_acosh) {
	ArithmeticExpression expr1("acosh(2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::acosh(2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_atanh) {
	ArithmeticExpression expr1("atanh(3/4)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::atanh(3.0 / 4.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_sqrt) {
	ArithmeticExpression expr1("sqrt(1/2)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::sqrt(1.0 / 2.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_log) {
	ArithmeticExpression expr1("log(1/3)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::log(1.0 / 3.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_fact) {
	ArithmeticExpression expr1("fact(5)");
	EXPECT_DOUBLE_EQ(expr1.calculate(), 120.0);
}

// ====================== CONSTANTS ===============================

TEST(ArithmeticExpression, can_calculate_expression_with_pi) {
	ArithmeticExpression expr1("pi");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::acos(-1.0));
}
TEST(ArithmeticExpression, can_calculate_expression_with_e) {
	ArithmeticExpression expr1("e");
	EXPECT_DOUBLE_EQ(expr1.calculate(), std::exp(1.0));
}


// ====================== VARIABLES ===============================
TEST(ArithmeticExpression, variables_detection) {
	ArithmeticExpression expr("x+y");
	EXPECT_TRUE(expr.has_variables());
}
TEST(ArithmeticExpression, procession_variables_absent) {
	ArithmeticExpression expr("1+2");
	EXPECT_FALSE(expr.has_variables());
}

// ====================== INCORRECT EXPRESSION ============================


TEST(ArithmeticExpression, processiong_incorrect_parens) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("((2+4)*2"));
}
TEST(ArithmeticExpression, processiong_consecutive_operators) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("2--11 * 5"));
}
TEST(ArithmeticExpression, processing_consecutive_operands) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("2 3"));
}
TEST(ArithmeticExpression, processiong_incorrect_dots) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("3..14 * 2"));
}
TEST(ArithmeticExpression, processing_missing_operators) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("2*(*2-3)"));
}
TEST(ArithmeticExpression, processing_missing_operators_before) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("2(1+5)"));
}
TEST(ArithmeticExpression, processing_missing_operators_after) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("(1+5)2"));
}
TEST(ArithmeticExpression, processing_operators_at_first) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("*1"));
}
TEST(ArithmeticExpression, processing_operators_in_the_end) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("1*"));
}
TEST(ArithmeticExpression, processing_missing_argument_in_function) {
	ASSERT_ANY_THROW(ArithmeticExpression expr("sin()"));
}



// ============ EXIT FROM THE RANGE OF ACCEPTABLE =========

TEST(ArithmeticExpression, processiong_division_by_zero) {
	ArithmeticExpression expr1("1/0");
	ASSERT_ANY_THROW(expr1.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_sqrt) {
	ArithmeticExpression expr("sqrt(-1)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_log) {
	ArithmeticExpression expr("log(0)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_asin) {
	ArithmeticExpression expr("asin(2)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_acos) {
	ArithmeticExpression expr("acos(2)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_acosh) {
	ArithmeticExpression expr("acosh(-2)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_atanh) {
	ArithmeticExpression expr("atanh(-2)");
	ASSERT_ANY_THROW(expr.calculate());
}
TEST(ArithmeticExpression, processing_the_exit_from_the_range_of_acceptable_values_fact) {
	ArithmeticExpression expr("fact(-1)");
	ASSERT_ANY_THROW(expr.calculate());
}

// ==================== CALCULATE A COMPLEX EXPRESSION =================

TEST(ArithmeticExpression, calculate_a_very_complex_expression) {
	ArithmeticExpression expr("sin(2 * acos(-1) + pi/2) * (3 ^ (1 + 1)) - fact(3) * log(e ^ 2) + sqrt((5 + 3) * 2) / tan(pi/4)");
	EXPECT_DOUBLE_EQ(expr.calculate(), 1.0);
}
TEST(ArithmeticExpression, calculate_a_very_complex_expression_with_variables) {
	ArithmeticExpression expr("(sin(pi/(2*x)) ^ 2 + cos(pi/(2*x)) ^ 2) * (log(e ^ (y * z)) / (y * z)) - (sqrt(w ^ 2) / w) + (fact(v) / (fact(v-1) * v))");
	expr.set_variable("x", 2.0);
	expr.set_variable("y", 3.0);
	expr.set_variable("z", 4.0);
	expr.set_variable("w", 5.0);
	expr.set_variable("v", 4.0);
	EXPECT_DOUBLE_EQ(expr.calculate(), 1.0);
}