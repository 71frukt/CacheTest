cmake_minimum_required(VERSION 3.12)
project(CashIdeal LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    RLogSU
    Cash2QLib
)

target_compile_definitions(${PROJECT_NAME} INTERFACE MODULE_NAME="${PROJECT_NAME}")

if(DEBUG_MODE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE DEBUG_MODE)
    message(STATUS "${PROJECT_NAME}: DEBUG_MODE ENABLED")
else()
    message(STATUS "${PROJECT_NAME}: DEBUG_MODE DISABLED")
endif()