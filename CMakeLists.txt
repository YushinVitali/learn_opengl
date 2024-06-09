# Change minimum version for CPP modules
cmake_minimum_required(VERSION 3.0.0)

project(learn_opengl VERSION 0.1.0 LANGUAGES CXX)

if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_CXX_FLAGS)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_FLAGS "-std=c++20 -Wall -Werror -pthread -static")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(PROJECT_CXX_FLAGS "/std:c++20 /WX")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROJECT_CXX_FLAGS}")

set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_MODULE_PATH}/cmake")
set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)


# Creating the main target of the project
add_executable(${PROJECT_NAME})


# Sources
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

file(GLOB_RECURSE SOURCE_EXE ${CMAKE_SOURCE_DIR}/*.cpp)
file(GLOB COMMON_SOURCE ${COMMON_DIR}/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC
        ${SOURCE_EXE}
        ${COMMON_SOURCE})


# Includes
set(INCLUDE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${COMMON_DIR}
        ${INCLUDE_HEADERS_DIR})

#find_package(Git QUIET)
find_package(Git)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodule during build" OFF)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update...")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "'git submodule update --init --recursive' failed with ${GIT_SUBMOD_RESULT},
            please checkout submodules")
        else()
            message(STATUS "All submodules have been loaded")
        endif ()
    endif ()
endif ()

message(STATUS "Linking GLFW...")
set(GLFW_LIB_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
add_subdirectory(${GLFW_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

message(STATUS "Linking GLEW...")
set(GLEW_LIB_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/glew-cmake)
add_subdirectory(${GLEW_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC libglew_static)

message(STATUS "Linking OpenGL...")
find_package(OpenGL REQUIRED)
#target_include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES})

message(STATUS "Linking GoogleTests...")
set(GOOGLTEST_LIB_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
add_subdirectory(${GOOGLTEST_LIB_DIR})
#target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest-main)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest)



# message(STATUS "Looking for GLFW")
# FetchContent_Declare(
#        GLFW
#         GIT_REPOSITORY https://github.com/glfw/glfw
# )
# set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
# set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
# set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
# set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
# FetchContent_MakeAvailable(GLFW)