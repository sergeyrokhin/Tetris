cmake_minimum_required(VERSION 3.20)

project(Tetris CXX)

set(CMAKE_CXX_STANDARD 17)

set(CONSOLE_CPP_FILES src/console/winconsole.cpp)
set(CONSOLE_H_FILES src/console/winconsole.h src/console/cursor.h)
set(TETRIS_CPP_FILES src/main.cpp src/tetris.cpp src/battlefield.cpp src/common.cpp src/control.cpp src/input_device.cpp src/observer.cpp)
set(TETRIS_H_FILES src/tetris.h src/battlefield.h src/common.h src/control.h src/input_device.h src/observer.h) 

add_executable(TestWinConsole src/tests/test_winconsole/main_test_type.cpp ${CONSOLE_CPP_FILES} ${CONSOLE_H_FILES} )
add_executable(TestInputConsole src/tests/test_winconsole/main_test_input.cpp src/common.cpp src/common.h ${CONSOLE_CPP_FILES} ${CONSOLE_H_FILES} )

add_executable(${PROJECT_NAME} ${TETRIS_CPP_FILES} ${CONSOLE_CPP_FILES} ${TETRIS_H_FILES}   ${CONSOLE_H_FILES} )

target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(TestWinConsole PUBLIC src/)
target_include_directories(TestInputConsole PUBLIC src/)