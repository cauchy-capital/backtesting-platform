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
  src/portfolio/portfolio.cpp
  src/backtest/backtester.cpp
  src/backtest/event.cpp
  src/backtest/market_event.cpp
  src/backtest/signal_event.cpp
  src/backtest/order_event.cpp
  src/io/csv_data_feed.cpp
  src/strategy/buy_everything_strategy.cpp
  src/strategy/sma_cross_strategy.cpp
  src/market/bar_builder.cpp
)

target_include_directories(cauchy_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ----------------------------
# Main executable
# ----------------------------
add_executable(CauchyBacktester
  src/main.cpp
)

target_link_libraries(CauchyBacktester PRIVATE cauchy_core)

# ----------------------------
# pybind11 (Python module)
# ----------------------------
#FetchContent_Declare(
#  pybind11
#  GIT_REPOSITORY https://github.com/pybind/pybind11.git
#  GIT_TAG v2.13.6
#)
#FetchContent_MakeAvailable(pybind11)

# The Python module name will be "traderbacktest" (or change it to what you want)
#pybind11_add_module(cauchybacktest MODULE
#  src/python/bindings.cpp
#)

#target_link_libraries(cauchybacktest PRIVATE cauchy_core)
#target_include_directories(cauchybacktest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Optional: nicer errors if symbols are missing at import-time
# (mostly relevant on Linux/macOS)
# set_target_properties(traderbacktest PROPERTIES LINK_OPTIONS "-Wl,-undefined,dynamic_lookup")

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

