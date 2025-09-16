cmake_minimum_required(VERSION 3.17)

project(CashTest LANGUAGES CXX)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
     --force-new-ctest-process
     --verbose
      --output-on-failure
)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(DEBUG_MODE "Enable debug mode" OFF)

set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log)

add_subdirectory(source/RLogSU)
add_subdirectory(source/CashLib)


add_executable(${PROJECT_NAME}
    source/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    RLogSU
    CashLib
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(rebuild
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/.[^.]*  # Удаляет скрытые файлы
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "${PROJECT_NAME}: Full rebuild"
    USES_TERMINAL
)


SET(TESTS ${CMAKE_SOURCE_DIR}/Tests)
file(GLOB testfiles "Tests/*.dat")

foreach(file ${testfiles})
    add_test(
        NAME ${file}
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/Tests/runtest.sh ${file} ./${PROJECT_NAME}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    set_tests_properties(${file} PROPERTIES DEPENDS ${PROJECT_NAME})
endforeach()



target_compile_definitions(${PROJECT_NAME} PRIVATE MODULE_NAME="${PROJECT_NAME}")

if(DEBUG_MODE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
    message(STATUS "${PROJECT_NAME}: DEBUG_MODE ENABLED")
else()
    message(STATUS "${PROJECT_NAME}: DEBUG_MODE DISABLED")
endif()