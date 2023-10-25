cmake_minimum_required(VERSION 3.26)
project(rain_maker_darwin)

set(CMAKE_CXX_STANDARD 20)

add_executable(rain_maker_darwin src/main.cpp
        src/process_utils.cpp
        src/process_utils.h
        src/memory_utils.cpp
        src/memory_utils.h
        src/bundle_reader.cpp
        src/bundle_reader.h
        src/bundle.cpp
        src/bundle.h
        src/swap_utils.cpp
        src/swap_utils.h)
