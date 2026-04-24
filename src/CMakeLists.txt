target_sources(arithmetic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_expression/arithmetic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_expression/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_expression/validator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_expression/calculator.cpp
)