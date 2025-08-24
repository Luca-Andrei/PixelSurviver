#ifndef GAMEOBSERVER_H
#define GAMEOBSERVER_H

#include <string>

class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void onGameEvent(const std::string& eventType, const std::string& eventData) = 0;
};


#endif
