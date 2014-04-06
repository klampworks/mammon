cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
project (test)

add_executable(
	reddit_parser_ut
	../reddit/reddit_parser.cpp 
	../base_parser.cpp
	../reddit/unit_test/reddit_parser_ut.cpp
	)

target_link_libraries(reddit_parser_ut boost_regex)

add_executable(
	dump_tags	
	../debug/dump_tags.cpp 
	../tidy.cpp
	)
target_link_libraries(dump_tags tidy)
