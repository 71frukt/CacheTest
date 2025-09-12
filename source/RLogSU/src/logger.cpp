#include <cstdio>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ostream>
#include <string>
#include <filesystem>


#include "RLogSU/logger.hpp"
#include "RLogSU/logger_appearance.hpp"

namespace RLSU {

Logger::Logger(const std::string& logfile_name)
{
    logfile_.open(log_folder_ + "/" + logfile_name);

    if (!logfile_)
    {
        std::cout << "Faild opening of logfile named '" << (log_folder_ + "/" + logfile_name) << "'" << std::endl;
        return;
    }

    logfile_ << "<html>                                                                                                     \n"
             << "\t<head>                                                                                                   \n"
             << "\t<title>" << logfile_name << "</title>                                                                    \n"
             << "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
             << "\t</head>                                                                                                  \n"
             << "\t<body>                                                                                                   \n"
             << "\t<div class=\"jumbotron text-center\">                                                                    \n"
             << "\t\t<h1>" << logfile_name << "</h1>                                                                        \n"
             << "\t</div>                                                                                                   \n"
             << "\t<pre>                                                                                                    \n"
             << "\t<style>                                                                                                  \n"
             << RLSU::Appearance::LOGGER_SETTINGS_PREAMBLE << "                                                             \n"
             << "\t</style>                                                                                                 \n";
}


Logger::~Logger()
{
    logfile_ << "\t\t</pre> \n"
                "\t</body>  \n"
                "</html>";

    logfile_.close();
}



void Logger::ColoredLog_(LogLevel log_level, const std::string text, const std::string code_place)
{
    using namespace RLSU::Appearance;

    if (log_level != LogLevel::DUMP)
        std::cout << "[" + module_name_ + "] ";  

    if (log_level != LogLevel::INFO)
        logfile_  << "[" + module_name_ + "] ";


    switch (log_level)
    {
    case LogLevel::INFO:
        std::cout << Console::ColoredStr(Console::CYAN,   "[INFO]    ") << text;
        break;

    case LogLevel::LOG:
        logfile_  << Html   ::ColoredStr(Html   ::GREEN,  "[LOG]     ") << text;
        std::cout << Console::ColoredStr(Console::GREEN,  "[LOG]     ") << text;
        break;

    case LogLevel::DUMP:
        logfile_ << Html::ColoredStr(Html::SKYBLUE,       "[DUMP]    ") << Html::ColoredFormatedCodePlace(code_place) << text;
        break;

    case LogLevel::WARNING:
        logfile_  << Html   ::ColoredStr(Html   ::YELLOW, "[WARNING] ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::YELLOW, text);
        std::cerr << Console::ColoredStr(Console::YELLOW, "[WARNING] ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::YELLOW, text);
        break;

    case LogLevel::ERROR:
        logfile_  << Html   ::ColoredStr(Html   ::RED,     "[ERROR]   ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::RED,   text);
        std::cerr << Console::ColoredStr(Console::RED,     "[ERROR]   ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::RED,   text);
        break;

    case LogLevel::ASSERT:
        logfile_  << Html   ::ColoredStr(Html   ::MAGENTA, "[ASSERT]  ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::RED, "failed condition: ") << "'" << Html   ::ColoredStr(Html   ::MAGENTA, text) << "'";
        std::cerr << Console::ColoredStr(Console::MAGENTA, "[ASSERT]  ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::RED, "failed condition: ") << "'" << Console::ColoredStr(Console::MAGENTA, text) << "'";
        break;
    
    default:
        std::cout << "Unknown Log level!\n";
        break;
    }

    if (log_level != LogLevel::DUMP)
        std::cout << "\n";  

    if (log_level != LogLevel::INFO)
        logfile_  << "\n";
}


std::string GetRelativePath(const std::string& abs_path_str)
{
    std::filesystem::path abs_path(abs_path_str);
    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path relative_path = std::filesystem::relative(abs_path, current_path);

    return relative_path.empty() ? abs_path_str : relative_path.string();
}



}   // namespace RLSU