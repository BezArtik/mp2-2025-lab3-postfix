target_sources(arithmetic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_expression/arithmetic.cpp
)

# В будущем:
# target_sources(arithmetic PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/validator.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/calculator.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.cpp
# )