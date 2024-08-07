cmake_minimum_required(VERSION 3.5)
project(DetectCMAKE_MAKE_PROGRAM NONE)

include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)

enable_testing()

add_test(NAME DetectCMAKE_MAKE_PROGRAM
         COMMAND ${CMAKE_COMMAND} -P DetectCMAKE_MAKE_PROGRAM.cmake)
