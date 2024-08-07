<<<<<<< HEAD
cmake_minimum_required(VERSION 3.0)

# Test for CMAKE_MAKE_PROGRAM detection with Ninja generator

# Check if CMAKE_MAKE_PROGRAM is set and points to the Ninja executable
find_program(NINJA_PROGRAM "ninja")
if(NOT NINJA_PROGRAM)
  message(FATAL_ERROR "Ninja program not found.")
endif()

# Set CMAKE_MAKE_PROGRAM to the path of the Ninja executable
set(CMAKE_MAKE_PROGRAM ${NINJA_PROGRAM})

# Check if the CMAKE_MAKE_PROGRAM is executable
execute_process(
  COMMAND "${CMAKE_MAKE_PROGRAM}" --version
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Verify that the command was executed successfully
if(result EQUAL 0)
  message(STATUS "CMAKE_MAKE_PROGRAM detection test passed: Ninja version detected as ${output}.")
else()
  message(FATAL_ERROR "CMAKE_MAKE_PROGRAM detection test failed: Ninja not executable with error ${result}.")
endif()
||||||| parent of b862ae4b52 (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
=======
cmake_minimum_required(VERSION 3.0)

# Test for CMAKE_MAKE_PROGRAM detection with Ninja generator

# Set up a scenario where CMAKE_MAKE_PROGRAM is required
project(DetectCMAKE_MAKE_PROGRAM NONE)

# Check if CMAKE_MAKE_PROGRAM is set and points to the Ninja executable
find_program(NINJA_PROGRAM "ninja")
if(NOT NINJA_PROGRAM)
  message(FATAL_ERROR "Ninja program not found.")
endif()

# Set CMAKE_MAKE_PROGRAM to the path of the Ninja executable
set(CMAKE_MAKE_PROGRAM ${NINJA_PROGRAM})

# Configure a test project using the Ninja generator
try_compile(PROJECT_COMPILED
  ${CMAKE_BINARY_DIR}/test_project
  ${CMAKE_SOURCE_DIR}/TestProject
  test_project
  CMAKE_FLAGS "-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}" "-G Ninja"
)

# Check if the project was configured and built correctly
if(NOT PROJECT_COMPILED)
  message(FATAL_ERROR "CMAKE_MAKE_PROGRAM detection test failed: Project could not be compiled with Ninja generator.")
else()
  message(STATUS "CMAKE_MAKE_PROGRAM detection test passed: Project compiled successfully with Ninja generator.")
endif()
>>>>>>> b862ae4b52 (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
