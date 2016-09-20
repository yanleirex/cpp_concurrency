cmake_minimum_required(VERSION 3.6)
project(cpp_concurrency)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#find_package(Boost REQUIRED)
#set(SOURCE_FILES main.cpp)
#add_executable(cpp_concurrency ${SOURCE_FILES})
add_subdirectory(1_thread_object)

add_subdirectory(2_thread_join_detach)

add_subdirectory(3_thread_parameter)

add_subdirectory(5_accumulate)

add_subdirectory(7_thread_mutex)

add_subdirectory(8_deadlock)

add_subdirectory(9_unique_lock)