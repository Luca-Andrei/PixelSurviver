#ifndef GAMELOGGER_H
#define GAMELOGGER_H

#include "GameObserver.h"
#include <fstream>
#include <string>

class GameLogger : public GameObserver {
private:
    std::ofstream logFile;
    std::string logFileName;
    bool consoleOutput;

public:
    explicit GameLogger(std::string filename = "game_log.txt", bool console = true);

    GameLogger(const GameLogger &other);

    GameLogger &operator=(const GameLogger &other);

    ~GameLogger() override;

    void onGameEvent(const std::string &eventType, const std::string &eventData) override;



    void logToFile(const std::string &message);

    static void logToConsole(const std::string &message);
};

#endif
