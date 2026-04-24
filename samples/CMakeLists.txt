add_executable(arithmetic_sample)
target_sources(arithmetic_sample PRIVATE
    main_arithmetic.cpp
)
target_link_libraries(arithmetic_sample PRIVATE
    project::arithmetic
)