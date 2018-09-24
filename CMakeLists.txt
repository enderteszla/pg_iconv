cmake_minimum_required(VERSION 3.7)

add_library(iconv.o STATIC iconverter.c)
add_library(text.o STATIC text.c)

project(iconv)
add_executable(pg_iconv main.c)
target_link_libraries(pg_iconv iconv iconv.o)

project(pg_iconv)
# set(CMAKE_C_STANDARD 99)
# add_library(iconv)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -liconv -v -Wall -Werror")
add_library(pg_iconv.so SHARED pg_iconv.c)
target_link_libraries(pg_iconv.so iconv iconv.o text.o)