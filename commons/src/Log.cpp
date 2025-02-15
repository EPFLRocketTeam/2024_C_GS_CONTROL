#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <qcoreapplication.h>
#include <qdir.h>
#include <string>
#include <filesystem>
#include <QDir>

#include "Log.h"

namespace {
    // ANSI color codes
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";

    std::string get_time()
    {
        std::time_t current_time = std::time(nullptr);
        std::tm* time_info = std::gmtime(&current_time);
        std::stringstream ss;
        ss << "[" << std::put_time(time_info, "%Hh-%Mm-%Ss") << "]";
        return ss.str();
    }

    // Function to map log levels to color codes
    std::string get_color_code(const std::string& level)
    {
        if (level == "INFO")
            return GREEN;
        else if (level == "DEBUG")
            return CYAN;
        else if (level == "WARN")
            return YELLOW;
        else if (level == "ERROR")
            return RED;
        else if (level == "FATAL")
            return MAGENTA;
        else
            return WHITE;
    }

    DebugLevel getLevelFromString(const std::string& level) {
        if (level == "INFO")
            return INFO;
        else if (level == "DEBUG")
            return DEBUG;
        else if (level == "WARN")
            return WARNING;
        else if (level == "ERROR")
            return ERROR;
        else if (level == "FATAL")
            return FATAL;
        else
            return INFO;

    }
}

// STATIC MEMBERS INITIALIZATION -----------------------------------------------
std::string MainLog::filename_ = "/../Log/firehorn.logs"; // if you set it to "default" the naming will automatically follow a date-time-minute format

// PUBLIC METHODS --------------------------------------------------------------

MainLog::MainLog()
{
    filename = filename_;
    create_file();
    std::cout << "MainLog object created\n";
}

MainLog::MainLog(std::string _filename) : filename(_filename)
{
    create_file();
    std::cout << "MainLog object created\n";
}

MainLog::MainLog(std::string _filename, DebugLevel level) : MainLog(_filename) {
    setDebugLevel(level);
}

MainLog::~MainLog()
{
    std::cout << "MainLog object destroyed\n";
}

// PROTECTED METHODS -----------------------------------------------------------

void MainLog::write_log(std::string level, std::string module, std::string event, std::string message)
{
        // Build the log message without colors for the file
    std::string log_message = get_time() 
                    + " ["
                    + level + "] [" 
                    + module + "] [" 
                    + event + "] [" 
                    + message + "],";

    // Build the log message with colors for the console
    std::string color_code = get_color_code(level);
    std::string colored_log_message = get_time()
                    + " ["
                    + color_code + level + RESET + "] ["
                    + module + "] ["
                    + event + "] ["
                    + message + "],";

    if (_level <= getLevelFromString(level)) {
        // Output to console
        std::cout << colored_log_message << std::endl;
    }

    // Write to file
    append_to_file(filename, log_message);

}

void MainLog::setDebugLevel(DebugLevel level) {
    _level = level;
}

// PRIVATE METHODS -------------------------------------------------------------

void MainLog::check_directory(std::string dir)
{
    std::filesystem::path directoryPath("../" + dir);

    if (!std::filesystem::exists(directoryPath)) {
        if (std::filesystem::create_directory(directoryPath)) {
            std::cout << dir << " Directory created successfully.\n";
        } else {
            std::cerr << dir << " Error creating directory.\n";
        }
    } else {
        std::cout << dir << "Directory exists.\n";
    }
}

void MainLog::create_file()
{
    if (filename_ != "default") {
        return;
    }

    // get timestamp 
    // can't use get_time() because we don't want the same format
    std::time_t current_time = std::time(nullptr);
    std::tm* time_info = std::gmtime(&current_time);
    std::stringstream ss;
    ss << std::put_time(time_info, "%Y-%m-%d-%H-%M-%S");
    std::string timestamp = ss.str();

    // manage path
    check_directory("Log");
    filename_ = "../Log/" + timestamp + ".txt";
    

    std::cout << filename_ << " has been created\n";

    return;
}

void MainLog::append_to_file(std::string filename, std::string content)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = QString::fromStdString(filename);
    if (filePath.startsWith("/")) {
        filePath.remove(0, 1);
    }
    QString full_path = appDir + "/" + filePath;
    // Open the file in append mode
    std::ofstream file(full_path.toStdString(), std::ios::app);

    if (file.is_open()) {
        file << content << "\n";
        file.close();

    } else {
        std::cerr << "Error opening file for appending " << full_path.toStdString() << std::endl;
    }
}

// MODULE LOG -------------------------------------------------------------------
// PUBLIC METHODS --------------------------------------------------------------

ModuleLog::ModuleLog(std::string module)
{
    module_ = module;
    filename = filename_;
}

ModuleLog::ModuleLog(std::string module, std::string _filename)
{
    module_ = module;
    filename = _filename;   
}

ModuleLog::~ModuleLog(){}

void ModuleLog::info(std::string event, std::string message)
{
    write_log("INFO", module_, event, message);
}

void ModuleLog::debug(std::string event, std::string message)
{
    write_log("DEBUG", module_, event, message);
}

void ModuleLog::warn(std::string event, std::string message)
{
    write_log("WARN", module_, event, message);
}

void ModuleLog::error(std::string event, std::string message)
{
    write_log("ERROR", module_, event, message);
}

void ModuleLog::fatal(std::string event, std::string message)
{
    write_log("FATAL", module_, event, message);
}
