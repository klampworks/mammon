cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
set(CMAKE_CXX_COMPILER_ID "Clang")
project(mammon-db)

set(SQLPP11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sqlpp11/include" CACHE FILEPATH "Path to sqlpp11 includes")
set(SQLPP11_SQLITE3_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlpp11-connector-sqlite3/include" CACHE
    FILEPATH "Path to sqlpp11-sqlite3 includes")

include_directories("${SQLPP11_INCLUDE_DIR}")
include_directories("${SQLPP11_SQLITE3_INCLUDE_DIR}")

add_executable(
    db_test
    test.cpp)

target_link_libraries(db_test
    "${CMAKE_SOURCE_DIR}/sqlpp11-connector-sqlite3/build/src/libsqlpp11-connector-sqlite3.a")
target_link_libraries(db_test sqlite3)

