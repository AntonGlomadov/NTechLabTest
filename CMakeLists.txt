cmake_minimum_required(VERSION 3.20.0)

project(NtechLab VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")
#вся логика программы в виде статической библиотеки
add_library(MatrixLib STATIC
            inc/ThreadPool.hpp
            inc/WorkerInterface.hpp
            inc/Matrix.hpp
            inc/SafeQueue.hpp)
target_include_directories(MatrixLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set_target_properties(MatrixLib PROPERTIES LINKER_LANGUAGE CXX)

#приложение которое я вляется тестами для программы
add_executable(NtechLab src/main.cpp) 
target_link_libraries(NtechLab PUBLIC MatrixLib)
find_package(GTest REQUIRED)
if (GTest_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GTest_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${GTest_LIBRARIES})
endif()
set_target_properties(NtechLab PROPERTIES LINKER_LANGUAGE CXX)
