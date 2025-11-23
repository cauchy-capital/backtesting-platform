cmake_minimum_required(VERSION 3.22.1)

project(CauchyBacktester)

add_executable(CauchyBacktester)

target_sources(CauchyBacktester
  PRIVATE
    src/CsvReader.cpp
)
