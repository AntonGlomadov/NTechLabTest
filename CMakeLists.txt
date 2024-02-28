cmake_minimum_required(VERSION 3.20.0)

project(NtechLab VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

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
set_target_properties(NtechLab PROPERTIES LINKER_LANGUAGE CXX)
