cmake_minimum_required(VERSION 3.15)
project(circular_buffer_final)

set(CMAKE_CXX_STANDARD 17)

# Основная программа
add_executable(circular_buffer_final main.cpp)

# Подключение тестов
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

# Тесты
add_executable(circular_buffer_tests test_circular_buffer.cpp)
target_link_libraries(circular_buffer_tests gtest_main)

enable_testing()
add_test(NAME CircularBufferTests COMMAND circular_buffer_tests)