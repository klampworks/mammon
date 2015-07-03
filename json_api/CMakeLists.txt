cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
set(CMAKE_CXX_COMPILER_ID "Clang")
project(mammon)

#add_executable(
#    fourchan_proc_ut
#    fourchan_proc.cpp
#    unit_test/fourchan_proc_ut.cpp
#    )

add_executable(
    mkgold
    unit_test/mkgold.cpp
    unit_test/common_ut.cpp
    chan_post.cpp
    fourchan_parser_json.cpp)

add_executable(
    fourchan_test
    fourchan_test.cpp
    chan_proc.cpp
    fourchan_proc.cpp
    eightchan_proc.cpp
    kyukon/kon.cpp
    kyukon/task.cpp
    chan_db.cpp
    ../base_db.cpp
    ../filesystem.cpp
    fourchan_parser_json.cpp
    chan_post.cpp
    fourchan_parser_json.cpp)
target_link_libraries(fourchan_test sqlite3)
target_link_libraries(fourchan_test curl)

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

