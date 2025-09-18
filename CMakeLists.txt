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
add_subdirectory(source/Cash2QLib)
add_subdirectory(source/CashIdeal)


#================ $$$Cash2Q$$$ =============================================================================

add_executable(cash_2q_test
    source/cash_2q_tester.cpp
)

target_link_libraries(cash_2q_test PRIVATE
    RLogSU
    Cash2QLib
)

add_custom_target(run_2q
    COMMAND ${CMAKE_BINARY_DIR}/cash_2q_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(rebuild_2q
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/.[^.]*
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "cash_2q_test: Full rebuild"
    USES_TERMINAL
)


target_compile_definitions(cash_2q_test PRIVATE MODULE_NAME="cash_2q_test")

if(DEBUG_MODE)
    target_compile_definitions(cash_2q_test PRIVATE DEBUG_MODE)
    message(STATUS "cash_2q_test: DEBUG_MODE ENABLED")
else()
    message(STATUS "cash_2q_test: DEBUG_MODE DISABLED")
endif()

#================ / Cash2Q =============================================================================



#================ $$$CashIdeal$$$ =============================================================================

add_executable(cash_ideal_test
    source/cash_ideal_tester.cpp
)

target_link_libraries(cash_ideal_test PRIVATE
    RLogSU
    Cash2QLib
    CashIdeal
)

add_custom_target(run_ideal
    COMMAND ${CMAKE_BINARY_DIR}/cash_ideal_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(rebuild_ideal
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/.[^.]*
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "cash_ideal_test: Full rebuild"
    USES_TERMINAL
)


target_compile_definitions(cash_ideal_test PRIVATE MODULE_NAME="cash_ideal_test")

if(DEBUG_MODE)
    target_compile_definitions(cash_ideal_test PRIVATE DEBUG_MODE)
    message(STATUS "cash_ideal_test: DEBUG_MODE ENABLED")
else()
    message(STATUS "cash_ideal_test: DEBUG_MODE DISABLED")
endif()

#================ / CashIdeal =============================================================================






#================== TESTS =================================================================================

set(TEST_DIR    ${CMAKE_SOURCE_DIR}/Tests)
set(TEST_RUNNER ${TEST_DIR}/runtest.sh)


#Cash2Q

file(GLOB cash2q_test_files "${TEST_DIR}/Cash2Q/*.dat")

foreach(test_file ${cash2q_test_files})
    get_filename_component(test_name ${test_file} NAME)

    add_test(
        NAME Cash2Q_${test_name}
        COMMAND ${TEST_RUNNER} ${test_file} $<TARGET_FILE:cash_2q_test>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    set_tests_properties(Cash2Q_${test_name} PROPERTIES 
        DEPENDS cash_2q_test
        LABELS "Cash2Q"
    )
endforeach()


# CashIdeal

file(GLOB cash_ideal_test_files "${TEST_DIR}/CashIdeal/*.dat")
foreach(test_file ${cash_ideal_test_files})
    get_filename_component(test_name ${test_file} NAME)
    
    add_test(
        NAME CashIdeal_${test_name}
        COMMAND ${TEST_RUNNER} ${test_file} $<TARGET_FILE:cash_ideal_test>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    set_tests_properties(CashIdeal_${test_name} PROPERTIES 
        DEPENDS cash_ideal_test
        LABELS "CashIdeal"
    )
endforeach()


#================ / TESTS =================================================================================
