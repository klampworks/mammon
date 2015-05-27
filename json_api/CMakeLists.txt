cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
set(CMAKE_CXX_COMPILER_ID "Clang")
project(mammon)

add_executable(
    json_test
    main.cpp
    fourchan_parser_json.cpp)

add_executable(
    fourchan_parser_json_ut
    unit_test/fourchan_parser_json_ut.cpp
    unit_test/common_ut.cpp
    fourchan_parser_json.cpp)

add_executable(
    mkgold
    unit_test/mkgold.cpp
    unit_test/common_ut.cpp
    fourchan_parser_json.cpp)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

