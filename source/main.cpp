#include "RLogSU/logger.hpp"

int main()
{
    RLSU_INFO("info {}", 5);
    RLSU_LOG("log");
    RLSU_DUMP("dump");
    RLSU_WARNING("warning");
    RLSU_ERROR("error");

    std::cout << "popa" << std::endl;

}