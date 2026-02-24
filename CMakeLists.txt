cmake_minimum_required(VERSION 3.22.1)
project(CauchyBacktester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # important for shared libs / python module

include(FetchContent)

# ----------------------------
# Core library (reused by exe + python module)
# ----------------------------
add_library(cauchy_core STATIC
  cauchy/portfolio/portfolio.cpp

  cauchy/backtest/backtester.cpp
  cauchy/backtest/dispatcher.cpp

  cauchy/execution/simulated_execution_handler.cpp

  cauchy/core/events/event.cpp
  cauchy/core/events/market_event.cpp
  cauchy/core/events/signal_event.cpp
  cauchy/core/events/order_event.cpp
  cauchy/core/events/fill_event.cpp

  cauchy/io/csv_data_feed.cpp

  cauchy/strategy/buy_everything_strategy.cpp
  cauchy/strategy/sma_cross_strategy.cpp

  cauchy/market/bar_builder.cpp

  cauchy/execution/simulated_execution_handler.cpp
)

target_include_directories(cauchy_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------------------
# Main executable
# ----------------------------
add_executable(CauchyBacktester
  cauchy/main.cpp
)

target_link_libraries(CauchyBacktester PRIVATE cauchy_core)

# ----------------------------
# pybind11 (Python module)
# ----------------------------
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(cauchybacktest MODULE
  cauchy/python/bindings.cpp
)

target_link_libraries(cauchybacktest
  PRIVATE
    cauchy_core
)

target_include_directories(cauchybacktest
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------------------
# GoogleTest
# ----------------------------
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(backtest_test
  test/backtest_test.cpp
)

# Link tests against your core library + gtest
target_link_libraries(backtest_test
  PRIVATE
    cauchy_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(backtest_test)

