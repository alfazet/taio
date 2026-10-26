cmake_minimum_required(VERSION 3.30)
project(taio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
if (${CMAKE_BUILD_TYPE} STREQUAL "test_bigint")
    file(GLOB SOURCES CONFIGURE_DEPENDS tests/bigint.cpp)
    set(TARGET_NAME test_bigint)
else ()
    file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
    set(TARGET_NAME taio)
endif ()
add_executable(${TARGET_NAME} ${SOURCES})

if (${CMAKE_BUILD_TYPE} STREQUAL "debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()


