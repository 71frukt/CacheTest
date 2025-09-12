#pragma once

#include <iostream>

#include "RLogSU/logger.hpp"
#include "RLogSU/logger_appearance.hpp"

namespace RLSU {

enum class ExitCode
{
    SUCCESS,
    FAIL
};

#define ERROR_HANDLE(CallFunc, ...)                                                                                                                                     \
do                                                                                                                                                                      \
{                                                                                                                                                                       \
    RLSU::ExitCode exit_code = CallFunc;                                                                                                                                \
                                                                                                                                                                        \
    if (exit_code != RLSU::ExitCode::SUCCESS)                                                                                                                           \
    {                                                                                                                                                                   \
        RLSU_ERROR("Call failed: '{}'", #CallFunc);                                                                                                                     \
    }                                                                                                                                                                   \
} while(0)

}