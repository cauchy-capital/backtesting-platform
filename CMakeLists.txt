cmake_minimum_required(VERSION 3.22.1)

project(CauchyBacktester)

add_executable(CauchyBacktester
        src/Portfolio.cpp
        src/Portfolio.h
        src/Decision.h)

target_sources(CauchyBacktester
  PRIVATE
    src/main.cpp
    src/csv_reader.cpp
)

target_include_directories(CauchyBacktester
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
