cmake_minimum_required(VERSION 2.8)
# set( CMAKE_VERBOSE_MAKEFILE on  )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(EXECUTABLE_OUTPUT_PATH "../")

find_package(FUSE REQUIRED)
include_directories(${FUSE_INCLUDE_DIR})

add_executable(main
        src/bbfs.c
        src/log.c
        src/main.c
)

target_link_libraries(main ${FUSE_LIBRARIES})
