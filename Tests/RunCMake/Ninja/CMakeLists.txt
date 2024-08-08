cmake_minimum_required(VERSION 3.5)
<<<<<<< HEAD
project(${RunCMake_TEST} NONE)
include(${RunCMake_TEST}.cmake NO_POLICY_SCOPE)
||||||| parent of 3d2e424dc7 (Register new test for CMAKE_MAKE_PROGRAM detection)
project(DetectCMAKE_MAKE_PROGRAM NONE)

include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)

enable_testing()

add_test(NAME DetectCMAKE_MAKE_PROGRAM
         COMMAND ${CMAKE_COMMAND} -P DetectCMAKE_MAKE_PROGRAM.cmake)
=======
project(DetectCMAKE_MAKE_PROGRAM NONE)

enable_testing()

include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)

add_test(NAME DetectCMAKE_MAKE_PROGRAM
         COMMAND ${CMAKE_COMMAND} -P DetectCMAKE_MAKE_PROGRAM.cmake)
>>>>>>> 3d2e424dc7 (Register new test for CMAKE_MAKE_PROGRAM detection)
