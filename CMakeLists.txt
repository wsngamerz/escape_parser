cmake_minimum_required(VERSION 3.27)
project(escape_parser C)

set(CMAKE_C_STANDARD 23)

add_executable(escape_parser main.c
        escape_parser.h
        escape_parser.c)
