cmake_minimum_required(VERSION 3.16)
project(DataStructLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Catch2 3 REQUIRED)

# Покрытие
add_compile_options(-g -O0 --coverage)
add_link_options(--coverage)

add_executable(run_google tests/google_tests.cpp)
target_link_libraries(run_google GTest::gtest GTest::gtest_main pthread)

add_executable(run_boost tests/boost_tests.cpp)
target_link_libraries(run_boost pthread)

add_executable(run_catch tests/catch_tests.cpp)
target_link_libraries(run_catch Catch2::Catch2WithMain)

add_executable(run_bench tests/benchmarks.cpp)
target_link_libraries(run_bench benchmark Threads::Threads)

add_custom_target(coverage_report
    COMMAND lcov --directory . --zerocounters

    COMMAND ./run_google
    COMMAND ./run_boost
    COMMAND ./run_catch
    
    COMMAND lcov --capture --directory . --output-file coverage_all.info --ignore-errors mismatch,inconsistent
    COMMAND lcov --extract coverage_all.info "${CMAKE_SOURCE_DIR}/*.h" --output-file coverage.info --ignore-errors mismatch,inconsistent
    COMMAND genhtml coverage.info --output-directory coverage_html --ignore-errors mismatch,inconsistent
    COMMAND echo "Report generated: coverage_html/index.html"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating coverage report..."
)
add_dependencies(coverage_report run_google run_boost run_catch)