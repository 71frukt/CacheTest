cmake_minimum_required(VERSION 3.12)
project(RLogSU LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#--- GET LOG DIR --------------------------------------------------------
if(NOT DEFINED LOG_DIR)
    set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log)
    message(STATUS "Using and creating default LOG_DIR: ${LOG_DIR}")
    file(MAKE_DIRECTORY ${LOG_DIR})
else()
    message(STATUS "Using LOG_DIR from parent: ${LOG_DIR}")
endif()
#------------------------------------------------------------------------


add_library(${PROJECT_NAME} STATIC
    src/logger.cpp
    # main.cpp/
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(DEBUG_MODE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
    message(STATUS "DEBUG_MODE ENABLED")
else()
    message(STATUS "DEBUG_MODE DISABLED")
endif()

target_compile_definitions(RLogSU PRIVATE MODULE_NAME="${PROJECT_NAME}")

# add_custom_target(run
#     COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )