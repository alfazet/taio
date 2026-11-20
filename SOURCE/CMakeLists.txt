cmake_minimum_required(VERSION 3.30)

project(taio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES CONFIGURE_DEPENDS tests/bigint.cpp)
set(TARGET_NAME test_bigint)
add_executable(${TARGET_NAME} ${SOURCES})
target_include_directories(${TARGET_NAME}
    PRIVATE
        include
)
file(GLOB SOURCES CONFIGURE_DEPENDS tests/timing.cpp src/karmarkar_karp.cpp src/brute.cpp)
set(TARGET_NAME test_timing)
add_executable(${TARGET_NAME} ${SOURCES})
target_include_directories(${TARGET_NAME}
    PRIVATE
        include
)
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
set(TARGET_NAME taio)
add_executable(${TARGET_NAME} ${SOURCES})
target_include_directories(${TARGET_NAME}
    PRIVATE
        include
)

if (MSVC)
    target_compile_options(${TARGET_NAME}
        PRIVATE
            /W4
            $<$<CONFIG:Release>:/O2>
    )
else ()
    target_compile_options(${TARGET_NAME}
        PRIVATE
            -Wall
            -Wextra
            $<$<CONFIG:Release>:-O3>
    )
endif ()
