#include "GameLogger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

GameLogger::GameLogger(std::string filename, bool console)
    : logFileName(std::move(filename)), consoleOutput(console) {
    logFile.open(logFileName, std::ios::app);
}

GameLogger::~GameLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

GameLogger::GameLogger(const GameLogger &other)
    : logFileName(other.logFileName), consoleOutput(other.consoleOutput) {
    logFile.open(logFileName, std::ios::app);
}

GameLogger &GameLogger::operator=(const GameLogger &other) {
    if (this != &other) {
            if (logFile.is_open()) {
        logFile.close();
    }

        logFileName = other.logFileName;
        consoleOutput = other.consoleOutput;

        logFile.open(logFileName, std::ios::app);
    }
    return *this;
}

void GameLogger::onGameEvent(const std::string &eventType, const std::string &eventData) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " | "
            << eventType << " | " << eventData;

    std::string logMessage = ss.str();

    if (consoleOutput) {
        logToConsole(logMessage);
    }

    logToFile(logMessage);
}

void GameLogger::logToFile(const std::string &message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.flush();
    }
}

void GameLogger::logToConsole(const std::string &message) {
    std::cout << "[LOG] " << message << std::endl;
}
