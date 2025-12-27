cmake_minimum_required(VERSION 3.22.1)

project(CauchyBacktester)

add_executable(CauchyBacktester
        src/main.cpp
        src/portfolio.cpp
        src/backtester.cpp
        src/portfolio.cpp
        src/io/csv_data_feed.cpp
        src/strategy/buy_everything_strategy.cpp
)


target_include_directories(CauchyBacktester
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
