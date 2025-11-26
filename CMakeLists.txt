cmake_minimum_required(VERSION 4.1.2)

# set the project name
project(NF06Restaurant)

set(SOURCES
    src/ChainedLists.c
    src/csvRestaurant.c
    src/hash.c
    src/main.c
)

set(CMAKE_C_COMPILER "gcc-15")

# add the executable
add_executable(NF06Restaurant ${SOURCES})
