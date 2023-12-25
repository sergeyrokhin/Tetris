cmake_minimum_required(VERSION 3.20)

project(Tetris CXX)


if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
# if(NOT LINUX) should work, too, if you need that
if(LINUX)
    message(STATUS ">>> Linux")
    # linux stuff here
else()
    message(STATUS ">>> Not Linux")
    # stuff that should happen not on Linux
endif()

Include_directories (${project_source_dir}/src)

set(CMAKE_CXX_STANDARD 17)

set(CONSOLE_H_FILES src/console/consoleframe.h)
set(TETRIS_CPP_FILES src/main.cpp src/tetris.cpp src/battlefield.cpp src/common.cpp src/control.cpp src/observer.cpp)
set(TETRIS_H_FILES src/tetris.h src/battlefield.h src/common.h src/control.h src/observer.h) 

set(CONSOLE_CPP_FILES src/console/consoleframe.cpp)

if(LINUX)
  list(APPEND CONSOLE_CPP_FILES src/console/nixconsoleframe.cpp)
  list(APPEND CONSOLE_H_FILES src/console/nixcursor.h)
else()
  list(APPEND CONSOLE_CPP_FILES src/console/winconsoleframe.cpp)
endif()


# add_executable(TestWinConsole src/tests/test_winconsole/main_test_type.cpp ${CONSOLE_CPP_FILES} ${CONSOLE_H_FILES} )
# add_executable(TestInputConsole src/tests/test_winconsole/main_test_input.cpp src/common.cpp src/common.h ${CONSOLE_CPP_FILES} ${CONSOLE_H_FILES} )

add_executable(${PROJECT_NAME} ${TETRIS_CPP_FILES} ${CONSOLE_CPP_FILES} ${TETRIS_H_FILES}   ${CONSOLE_H_FILES} )

list(APPEND TARGETS ${PROJECT_NAME})
# list(APPEND TARGETS TestWinConsole)
# list(APPEND TARGETS TestInputConsole)

foreach(target ${TARGETS})
  target_compile_features(${target} PUBLIC cxx_std_17)
  set_target_properties(${target} PROPERTIES CXX_STANDARD_REQUIRED ON)

  target_link_libraries(${target} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
  target_include_directories(${target} PUBLIC src/)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target}
      PRIVATE -g -Wall -Wextra -Werror -pedantic)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target}
      PRIVATE /W4 /WX)
  endif()
endforeach()
