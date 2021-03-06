
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(runTests test.cpp)
# target_link_libraries(runTests gtest_main)
# add_test(NAME fixed_test COMMAND runTests)


################
# Define a test
add_executable(fixed_test fixed_test.cpp)
add_executable(dsp_test dsp_test.cpp)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

#add_dependencies(fixed_test gtest)
target_link_libraries(fixed_test gtest_main)

#add_dependencies(dsp_test gtest)
target_link_libraries(dsp_test gtest_main)

enable_testing()
add_test(NAME    fixed_test 
         COMMAND fixed_test)

add_test(NAME    dsp_test 
         COMMAND dsp_test)



