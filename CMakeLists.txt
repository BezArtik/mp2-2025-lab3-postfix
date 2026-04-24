cmake_minimum_required(VERSION 3.20...3.30)
project(arithmetic_expression VERSION 1.0.0 LANGUAGES CXX)

# Опции сборки
option(BUILD_SAMPLES "Build sample applications" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DOCS "Build documentation" OFF)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)


add_library(containers INTERFACE)
target_include_directories(containers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/containers>
    $<INSTALL_INTERFACE:include/containers>
)
target_link_libraries(containers INTERFACE project_options)
add_library(project::containers ALIAS containers)

add_library(arithmetic STATIC)
target_include_directories(arithmetic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_sources(arithmetic PRIVATE
    src/arithmetic_expression/arithmetic.cpp
    # Будущие файлы:
    # src/arithmetic_expression/parser.cpp
    # src/arithmetic_expression/validator.cpp
    # src/arithmetic_expression/calculator.cpp
    # src/arithmetic_expression/tokenizer.cpp
)
target_link_libraries(arithmetic PUBLIC project::containers)
add_library(project::arithmetic ALIAS arithmetic)

add_subdirectory(src)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS arithmetic containers project_options EXPORT arithmetic_targets)
install(EXPORT arithmetic_targets
    FILE arithmetic_targets.cmake
    NAMESPACE project::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arithmetic
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})