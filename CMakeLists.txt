cmake_minimum_required(VERSION 3.20)

# set the project name
project(sim)
# set c++ version
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -DNDEBUG)

# Comment or uncomment these lines to change which version is being compiled
add_executable(paos main.cpp commons.hpp commons.cpp aos.hpp aos.cpp)
add_executable(psoa main.cpp commons.hpp commons.cpp soa.hpp soa.cpp)
