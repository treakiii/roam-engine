// Logger.h - The voice of the engine
// This tells us what's happening, or what's going wrong

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

// Log levels - because not all messages are created equal
enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

// The Logger class - our storyteller
class Logger {
public:
    Logger();
    ~Logger();

    // Initialize the logger - set up the storybook
    bool Initialize(const std::string& logFile = "engine.log");

    // Log messages - tell the tale
    void Log(LogLevel level, const std::string& message);
    void Debug(const std::string& message) { Log(LogLevel::Debug, message); }
    void Info(const std::string& message) { Log(LogLevel::Info, message); }
    void Warning(const std::string& message) { Log(LogLevel::Warning, message); }
    void Error(const std::string& message) { Log(LogLevel::Error, message); }
    void Fatal(const std::string& message) { Log(LogLevel::Fatal, message); }

    // Set log level - filter the noise
    void SetLogLevel(LogLevel level) { currentLogLevel = level; }

    // Enable/disable console output - talk or be quiet
    void SetConsoleOutput(bool enabled) { consoleOutput = enabled; }

private:
    // Format log message - make it pretty
    std::string FormatMessage(LogLevel level, const std::string& message);

    // Get current timestamp - when did this happen?
    std::string GetTimestamp();

    // Log file stream - where we write the story
    std::ofstream logFile;

    // Mutex for thread safety - don't let threads fight
    std::mutex logMutex;

    // Configuration
    LogLevel currentLogLevel;
    bool consoleOutput;
    bool initialized;
};

#endif // LOGGER_H
