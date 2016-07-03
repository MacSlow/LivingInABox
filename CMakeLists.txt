cmake_minimum_required (VERSION 2.8.12)
project (LivingInABox)

set (CC "g++")

add_compile_options(-std=c++14 -Wall -pedantic -O0 -mavx -fopenmp -msse4.2)

add_executable (LivingInABox main.cpp test_calcBox.cpp test_data.cpp)
target_link_libraries (LivingInABox -fopenmp)
