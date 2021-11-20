cmake_minimum_required(VERSION 3.16.3)

project(ece365_program_2)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -fsanitize=undefined -Wpedantic -Wunused)
set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=undefined")

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

include_directories(.)

add_executable(graph_driver
	graph_driver.cpp
	hash.cpp
	heap.cpp
)
