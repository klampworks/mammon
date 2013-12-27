cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
project (test)

add_executable(
	chan_db_test 
	chan_db_test.cpp 
	chan_db.cpp 
	../base_db.cpp)

target_link_libraries(chan_db_test sqlite3)

add_executable(
	chan_parser_test 
	chan_parser_test.cpp 
	chan_parser.cpp 
	../base_parser.cpp)

add_executable(
	chan_parser_unit_test 
	chan_parser.cpp 
	../base_parser.cpp
	unit_test/chan_parser_unit_test.cpp
	)
