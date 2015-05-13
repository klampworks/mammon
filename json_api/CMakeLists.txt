set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
set(CMAKE_CXX_COMPILER_ID "Clang")
project(mammon)

add_executable(
    json_test
    main.cpp
    fourchan_parser_json.cpp)


