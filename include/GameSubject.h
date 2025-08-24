#ifndef GAMESUBJECT_H
#define GAMESUBJECT_H

#include <vector>
#include <string>
#include "GameObserver.h"

class GameSubject {
public:
    virtual ~GameSubject() = default;
    
    virtual void addObserver(GameObserver* observer);
    virtual void removeObserver(GameObserver* observer);
    virtual void notifyObservers(const std::string& eventType, const std::string& eventData);

protected:
    std::vector<GameObserver*> observers;
};

inline void GameSubject::addObserver(GameObserver* observer) {
    if (observer && std::ranges::find(observers, observer) == observers.end()) {
        observers.push_back(observer);
    }
}

inline void GameSubject::removeObserver(GameObserver* observer) {
    auto it = std::ranges::find(observers, observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

inline void GameSubject::notifyObservers(const std::string& eventType, const std::string& eventData) {
    for (auto observer : observers) {
        if (observer) {
            observer->onGameEvent(eventType, eventData);
        }
    }
}

#endif
