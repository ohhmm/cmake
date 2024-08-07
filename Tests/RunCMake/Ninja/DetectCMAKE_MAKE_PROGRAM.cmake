cmake_minimum_required(VERSION 3.0)

# Test for CMAKE_MAKE_PROGRAM detection with Ninja generator

# Set up a scenario where CMAKE_MAKE_PROGRAM is required
project(DetectCMAKE_MAKE_PROGRAM C)

# Check if CMAKE_MAKE_PROGRAM is set and points to the Ninja executable
find_program(NINJA_PROGRAM "ninja")
if(NOT NINJA_PROGRAM)
  message(FATAL_ERROR "Ninja program not found.")
endif()

# Set CMAKE_MAKE_PROGRAM to the path of the Ninja executable
set(CMAKE_MAKE_PROGRAM ${NINJA_PROGRAM})

# Attempt to configure a minimal project using the Ninja generator
try_compile(PROJECT_COMPILED
  "${CMAKE_BINARY_DIR}/TestProject_build"
  "${CMAKE_CURRENT_SOURCE_DIR}/TestProject"
  CMAKE_FLAGS "-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}" "-G" "Ninja"
  OUTPUT_VARIABLE COMPILE_OUTPUT
)

# Check if the project was configured and built correctly
if(NOT PROJECT_COMPILED)
  message(FATAL_ERROR "CMAKE_MAKE_PROGRAM detection test failed: Project could not be compiled with Ninja generator. Output: ${COMPILE_OUTPUT}")
else()
  message(STATUS "CMAKE_MAKE_PROGRAM detection test passed: Project compiled successfully with Ninja generator.")
endif()
