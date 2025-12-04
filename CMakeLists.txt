cmake_minimum_required(VERSION 3.22.1)

project(CauchyBacktester)

add_executable(CauchyBacktester
        src/main.cpp
        src/Portfolio.cpp
        src/Backtester.cpp
        src/Portfolio.cpp
        src/IO/CsvDataFeed.cpp
)


target_include_directories(CauchyBacktester
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
