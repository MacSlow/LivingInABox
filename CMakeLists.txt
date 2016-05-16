cmake_minimum_required (VERSION 2.8.12)
project (LivingInABox)

add_compile_options(-std=c++14 -Wall -pedantic -O3 -mavx)
#add_compile_options(-std=c++14 -Wall -pedantic -Werror -O0 -ggdb)

add_executable (LivingInABox main.cpp test_calcBox.cpp test_data.cpp)
target_link_libraries (LivingInABox -s -pthread)
