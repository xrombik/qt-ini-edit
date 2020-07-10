cmake_minimum_required(VERSION 2.8)
project(qt-ini-edit)
set(CPPSTD "c++11")

find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(.)
aux_source_directory(. SRC_LIST)

message(STATUS "CMAKE_BUILD_TYPE: \"${CMAKE_BUILD_TYPE}\"")
if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -fpic -pedantic -Wall -Wfatal-errors -s -Os --std=${CPPSTD}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all -fstack-check -fstack-protector-strong -fsanitize=bounds -fsanitize=vla-bound -fsanitize=leak -fsanitize=undefined -pedantic -fsanitize=address -Wall -Wfatal-errors -g -O0 --std=${CPPSTD}")
endif()

list(REMOVE_ITEM SRC_LIST "./strs-test.cpp")
list(REMOVE_ITEM SRC_LIST "./inifile-test.cpp")
add_executable(${PROJECT_NAME} ${SRC_LIST})
qt5_use_modules(${PROJECT_NAME} Widgets)

add_executable("strs-test" strs-test.cpp strs.cpp)
add_executable("inifile-test" inifile-test.cpp strs.cpp inifile.cpp)
