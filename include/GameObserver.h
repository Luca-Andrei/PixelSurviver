#ifndef GAMEOBSERVER_H
#define GAMEOBSERVER_H

#include <string>
#include <vector>
#include <memory>

// Observer Pattern - Base Observer interface
class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void onGameEvent(const std::string& eventType, const std::string& eventData) = 0;
};

// Observer Pattern - Subject interface is now in GameSubject.h

#endif // GAMEOBSERVER_H
