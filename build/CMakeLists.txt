cmake_minimum_required (VERSION 2.6)
#set(CMAKE_CXX_FLAGS "-std=c++11 -g 
#-I/usr/include/ruby/x86_64-linux/ -I/usr/include/ruby")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra -I/usr/include/ruby/x86_64-linux/ -I/usr/include/x86_64-linux-gnu/ruby-2.1.0/ -I/usr/include/ruby")
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

add_executable(
	driver_test 
	../driver_test.cpp
	../base_driver.cpp
	../chan/chan_driver.cpp
	../chan/desuchan_parser.cpp 
	../chan/wakachan_parser.cpp 
	../chan/wakachan_driver.cpp 
	../chan/chan_parser.cpp 
	../base_parser.cpp
	../chan/chan_db.cpp 
	../base_db.cpp
	../chan/desuchan_driver.cpp
	../chan/shanachan_driver.cpp
	../chan/secchan_driver.cpp
	../tidy.cpp
	../reddit/reddit_driver.cpp
	../reddit/reddit_parser.cpp
	../reddit/reddit_db.cpp
	../chan/fourchan_parser.cpp
	../chan/fourchan_driver.cpp
    ../filesystem.cpp
	)

target_link_libraries(driver_test sqlite3)
target_link_libraries(driver_test kyukon)
target_link_libraries(driver_test magic)
target_link_libraries(driver_test tidy)
target_link_libraries(driver_test boost_regex)

add_executable(
    xml_tool
	../chan/xml_tools/xml_tool.cpp 
    ../tidy.cpp
	)

target_link_libraries(xml_tool xml2)
target_link_libraries(xml_tool tidy)

add_executable(
    eight
	../chan/eightchan_parser.cpp
	../chan/ruby/eightchan_parser_main.cpp
	../chan/rb_chan_post_multi.cpp
)
target_link_libraries(eight ruby20)

