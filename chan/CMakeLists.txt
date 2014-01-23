cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
project (test)

add_executable(
	desuchan_parser_unit_test 
	desuchan_parser.cpp 
	chan_parser.cpp 
	../base_parser.cpp
	unit_test/desuchan_parser_unit_test.cpp
	)

add_executable(
	wakachan_parser_unit_test 
	wakachan_parser.cpp 
	chan_parser.cpp 
	../base_parser.cpp
	unit_test/wakachan_parser_unit_test.cpp
	../tidy.cpp
	)

add_executable(
	shanachan_parser_unit_test 
	wakachan_parser.cpp 
	chan_parser.cpp 
	../base_parser.cpp
	unit_test/shanachan_parser_unit_test.cpp
	../tidy.cpp
	)

target_link_libraries(wakachan_parser_unit_test tidy)
target_link_libraries(shanachan_parser_unit_test tidy)

if(0)
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

endif(0)

add_executable(
	chan_driver_test 
	desuchan_parser.cpp 
	wakachan_parser.cpp 
	wakachan_driver.cpp 
	chan_parser.cpp 
	../base_parser.cpp
	chan_db.cpp 
	../base_db.cpp
	chan_driver_test.cpp
	desuchan_driver.cpp
	chan_driver.cpp
	../base_driver.cpp
	../tidy.cpp
	)

target_link_libraries(chan_driver_test sqlite3)
target_link_libraries(chan_driver_test kyukon)
target_link_libraries(chan_driver_test magic)
target_link_libraries(chan_driver_test tidy)
