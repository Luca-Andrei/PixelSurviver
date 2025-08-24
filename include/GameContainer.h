#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#include <vector>
#include <memory>
#include <algorithm>
template<typename T>
class GameContainer {
private:
    std::vector<std::unique_ptr<T>> items;
    size_t maxSize;

public:
    explicit GameContainer(size_t maxSize = 100) : maxSize(maxSize) {}
    
    GameContainer(const GameContainer& other) : maxSize(other.maxSize) {
        for (const auto& item : other.items) {
            if (item) {
                items.push_back(std::make_unique<T>(*item));
            }
        }
    }
    
    GameContainer& operator=(const GameContainer& other) {
        if (this != &other) {
            items.clear();
            maxSize = other.maxSize;
            for (const auto& item : other.items) {
                if (item) {
                    items.push_back(std::make_unique<T>(*item));
                }
            }
        }
        return *this;
    }
    
    GameContainer(GameContainer&& other) noexcept 
        : items(std::move(other.items)), maxSize(other.maxSize) {
        other.maxSize = 0;
    }
    
    GameContainer& operator=(GameContainer&& other) noexcept {
        if (this != &other) {
            items = std::move(other.items);
            maxSize = other.maxSize;
            other.maxSize = 0;
        }
        return *this;
    }
    
    ~GameContainer() = default;
    
    bool addItem(std::unique_ptr<T> item) {
        if (items.size() < maxSize && item) {
            items.push_back(std::move(item));
            return true;
        }
        return false;
    }
    

    
    [[nodiscard]] size_t size() const { return items.size(); }
    
    [[nodiscard]] bool empty() const { return items.empty(); }
    
    void clear() { items.clear(); }
    

    
    friend std::ostream& operator<<(std::ostream& os, const GameContainer& container) {
        os << "GameContainer with " << container.items.size() << " items (max: " << container.maxSize << ")";
        return os;
    }
};

#endif
