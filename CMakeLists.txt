cmake_minimum_required(VERSION 3.5)
project(ray)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")

#OpenMP
option(BUILD_WITH_OPENMP "Build with OpenMP" ON)
if (BUILD_WITH_OPENMP)
    FIND_PACKAGE( OpenMP )
    message("Building with OpenMP.")
    if(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    else()
        message("OpenMP is not found. Cannot use parallelism.")
    endif()
else()
    message("Building without OpenMP.")
endif()

#Build library
set(LIB_SOURCES )
set(LIB_HEADERS src/timer.hpp)

add_library( lib ${LIB_SOURCES} ${LIB_HEADERS} )
set_target_properties(lib PROPERTIES LINKER_LANGUAGE CXX)

# TESTS
enable_testing()
file(GLOB TEST_FILES "tests/*.cpp")
foreach( TEST_FILE ${TEST_FILES})
		get_filename_component(TEST_FILENAME ${TEST_FILE} NAME)
    string( REPLACE ".cpp" "" TARGET_NAME ${TEST_FILENAME} )
    add_executable( ${TARGET_NAME} "${TEST_FILE}" ${LIB_HEADERS} )
    target_link_libraries( ${TARGET_NAME} lib )
		add_test(${TARGET_NAME} ${TARGET_NAME})
endforeach()

include_directories(src/)
