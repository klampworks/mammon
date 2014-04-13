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
	reddit_db_ut
	../reddit/reddit_db.cpp 
	../base_db.cpp
	../reddit/unit_test/reddit_db_ut.cpp
	)
target_link_libraries(reddit_db_ut sqlite3)

add_executable(
	dump_tags	
	../debug/dump_tags.cpp 
	../tidy.cpp
	)
target_link_libraries(dump_tags tidy)

add_executable(
	reddit_driver_test
	../reddit/reddit_driver_test.cpp
	../reddit/reddit_driver.cpp
	../base_driver.cpp
	../reddit/reddit_parser.cpp
	../base_parser.cpp
	../reddit/reddit_db.cpp
	../base_db.cpp
)
target_link_libraries(reddit_driver_test sqlite3)
target_link_libraries(reddit_driver_test boost_regex)
target_link_libraries(reddit_driver_test kyukon)
target_link_libraries(reddit_driver_test magic)
