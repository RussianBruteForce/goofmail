set(PROJECT_TEST_NAME "${PROJECT_NAME}_tests")

file(GLOB TESTS_SOURCES ./*.cpp ../*.cpp)
list(REMOVE_ITEM TESTS_SOURCES "${CMAKE_SOURCE_DIR}/tests/../main.cpp")

add_executable(${PROJECT_TEST_NAME} ${TESTS_SOURCES})

include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES} ..)

find_package(Threads)
add_library(gtest STATIC IMPORTED)
add_library(gtest_main STATIC IMPORTED)

target_link_libraries(${PROJECT_TEST_NAME} gtest.a)
target_link_libraries(${PROJECT_TEST_NAME} gtest_main.a)
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_TEST_NAME} curl)
