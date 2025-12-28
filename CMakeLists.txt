cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 20)

project(CauchyBacktester)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

add_executable(CauchyBacktester
        src/main.cpp
        src/portfolio/portfolio.cpp
        src/backtest/backtester.cpp
        src/io/csv_data_feed.cpp
        src/strategy/buy_everything_strategy.cpp
)


target_include_directories(CauchyBacktester
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

FetchContent_MakeAvailable(googletest)


#TESTING
enable_testing()

add_executable(
  backtest_test
  test/backtest_test.cpp
)
target_link_libraries(
  backtest_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(backtest_test)
