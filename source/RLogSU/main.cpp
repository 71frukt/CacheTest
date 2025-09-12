#include <iostream>

#include "RLogSU/logger.hpp"
#include "RLogSU/error_handler.hpp"

RLSU::ExitCode pzdc(int a)
{
    return RLSU::ExitCode::FAIL;
}

int main()
{
    RLSU::Logger logger;

    RLSU_INFO("info {}", 5);
    RLSU_LOG("log");
    RLSU_DUMP("dump");
    RLSU_WARNING("warning");
    RLSU_ERROR("error");

    ERROR_HANDLE(pzdc(2));

    std::cout << "popa" << std::endl;
}