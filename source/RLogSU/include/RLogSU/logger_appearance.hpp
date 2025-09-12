#pragma once

#include <iostream>
#include <string>

namespace RLSU::Appearance {
    
namespace Console {

using Color = const std::string;

Color BLACK   = "30";
Color RED     = "31";
Color GREEN   = "32";
Color YELLOW  = "33";
Color BLUE    = "34";
Color MAGENTA = "35";
Color CYAN    = "36";
Color WHITE   = "37";

inline std::string ColoredStr    (const Console::Color color, const std::string& text)  { return "\x1b["  + color + "m" + text + "\x1b[0m"; }
inline std::string ColoredFormatedCodePlace(const std::string& code_place_str)          { return "[" + ColoredStr(CYAN, code_place_str) + "]  "; }

} // namespace Console


namespace Html {

using Color = const std::string;

Color BLUE    = "blue";
Color CYAN    = "cyan";
Color SKYBLUE = "SkyBlue";
Color RED     = "red";
Color GREEN   = "green";
Color YELLOW  = "yellow";
Color WHITE   = "white";
Color MAGENTA = "Fuchsia";


const std::string COLORS_HTML_PREAMBLE =                                                                                           
    "\tbody { background: linear-gradient(to right, #0f2027, #203a43, #2c5364);}                                         \n"
    "\tpre { color: #ffffff; }                                                                                               \n"
    "\t." + Html::RED     + "{ color:" + Html::RED     + ";}                                                                   \n"
    "\t." + Html::YELLOW  + "{ color:" + Html::YELLOW  + ";}                                                                   \n"
    "\t." + Html::BLUE    + "{ color:" + Html::BLUE    + ";}                                                                   \n"
    "\t." + Html::GREEN   + "{ color:" + Html::GREEN   + ";}                                                                   \n"
    "\t." + Html::CYAN    + "{ color:" + Html::CYAN    + ";}                                                                   \n"
    "\t." + Html::MAGENTA + "{ color:" + Html::MAGENTA + ";}                                                                   \n"
    "\t." + Html::SKYBLUE + "{ color:" + Html::SKYBLUE + ";}                                                                   \n"
    "\t.jumbotron {                                                                                                            \n"
    "\t\tbackground: linear-gradient(to bottom, #ffffff,  #ffffff, #001aff, #001aff, #b00000, #b00000);            \n"
    "\t\tcolor: white;                                                                                                         \n"
    "\t\ttext-shadow: 1px 1px 2px rgba(0,0,0,0.5);                                                                           \n"
    "\t\tborder-radius: 0;                                                                                                     \n"
    "\t\tmargin-bottom: 0;                                                                                                     \n"
    "\t}                                                                                                                       \n";                     


const std::string TABLE_HTML_COLOR =
    "\t.tables-row {                                                                                                           \n"
    "\t    margin-left: 100px;                                                                                                 \n"
    "\t    display: flex;                                                                                                      \n"
    "\t    gap: 20px;                                                                                                          \n"
    "\t}                                                                                                                       \n"
    "\t                                                                                                                        \n"
    "\t.table-with-caption {                                                                                                   \n"
    "\t    display: flex;                                                                                                      \n"
    "\t    flex-direction: column;                                                                                             \n"
    "\t    align-items: center;                                                                                                \n"
    "\t}                                                                                                                       \n"
    "\t                                                                                                                        \n"
    "\t.table-caption {                                                                                                        \n"
    "\t    margin-bottom: 10px;                                                                                                \n"
    "\t    font-weight: bold;                                                                                                  \n"
    "\t}                                                                                                                       \n"
    "\t.table {                                                                                                                \n"
    "\t    border-collapse: collapse;                                                                                          \n"
    "\t    width: 100px;                                                                                                       \n"
    "\t}                                                                                                                       \n"
    "\t                                                                                                                        \n"
    "\t.table td {                                                                                                             \n"
    "\t    color: #00ccff;                                                                                                   \n"
    "\t    border: 1px solid #3a4a5a;                                                                                        \n"
    "\t    height: 35px;                                                                                                       \n"
    "\t    text-align: center;                                                                                                 \n"
    "\t}                                                                                                                       \n"
    "\t                                                                                                                        \n"
    "\ttable tr:first-child td {                                                                                               \n"
    "\t    color: #ffffff;                                                                                                   \n"
    "\t    font-weight: bold;                                                                                                  \n"
    "\t    background-color: rgba(0,0,0,0.2);                                                                                \n"
    "\t}                                                                                                                       \n"
    "\t                                                                                                                        \n"
    "\ttable td:first-child {                                                                                                  \n"
    "\t    color: #ffffff;                                                                                                   \n"
    "\t}                                                                                                                       \n";


inline std::string ColoredStr(const Html::Color color, const std::string& text)  { return "<span class=" + color + ">" + text + "</span>"; }
inline std::string ColoredFormatedCodePlace(const std::string& code_place_str)   { return "[" + ColoredStr(CYAN, code_place_str) + "]  "; }

} // namespace Html


const std::string LOGGER_SETTINGS_PREAMBLE = Html::COLORS_HTML_PREAMBLE + Html::TABLE_HTML_COLOR;

} 
