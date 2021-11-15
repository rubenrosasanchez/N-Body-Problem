cmake_minimum_required(VERSION 3.20)

# set the project name
project(Parallel-n-body)
# set c++ version
set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_compile_options(-Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors)

# Comment or uncomment these lines to change which version is being compiled
add_executable(paos main.cpp commons.hpp commons.cpp aos.hpp aos.cpp)
target_link_libraries(paos OpenMP::OpenMP_CXX)
add_executable(psoa main.cpp commons.hpp commons.cpp soa.hpp soa.cpp)
target_link_libraries(psoa OpenMP::OpenMP_CXX)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -DNDEBUG -fopenmp")
