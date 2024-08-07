cmake_minimum_required(VERSION 3.5)
<<<<<<< HEAD
project(${RunCMake_TEST} NONE)
include(${RunCMake_TEST}.cmake NO_POLICY_SCOPE)
||||||| parent of f8a41a292f (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
project(DetectCMAKE_MAKE_PROGRAM NONE)

enable_testing()

include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)

add_test(NAME DetectCMAKE_MAKE_PROGRAM
         COMMAND ${CMAKE_COMMAND} -P DetectCMAKE_MAKE_PROGRAM.cmake)
=======
<<<<<<< HEAD
project(DetectCMAKE_MAKE_PROGRAM NONE)

enable_testing()

include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)

add_test(NAME DetectCMAKE_MAKE_PROGRAM
         COMMAND ${CMAKE_COMMAND} -P DetectCMAKE_MAKE_PROGRAM.cmake)
||||||| parent of b862ae4b52 (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
project(${RunCMake_TEST} NONE)
include(${RunCMake_TEST}.cmake NO_POLICY_SCOPE)
=======
project(${RunCMake_TEST} NONE)

include(${RunCMake_TEST}.cmake NO_POLICY_SCOPE)
include(DetectCMAKE_MAKE_PROGRAM.cmake NO_POLICY_SCOPE)
>>>>>>> b862ae4b52 (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
>>>>>>> f8a41a292f (Add new test for CMAKE_MAKE_PROGRAM detection with Ninja generator)
