#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iterator>

// Template Class - Generic container for game objects
template<typename T>
class GameContainer {
private:
    std::vector<std::unique_ptr<T>> items;
    size_t maxSize;

public:
    explicit GameContainer(size_t maxSize = 100) : maxSize(maxSize) {}
    
    // Copy constructor
    GameContainer(const GameContainer& other) : maxSize(other.maxSize) {
        for (const auto& item : other.items) {
            if (item) {
                items.push_back(std::make_unique<T>(*item));
            }
        }
    }
    
    // Assignment operator
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
    
    // Move constructor
    GameContainer(GameContainer&& other) noexcept 
        : items(std::move(other.items)), maxSize(other.maxSize) {
        other.maxSize = 0;
    }
    
    // Move assignment operator
    GameContainer& operator=(GameContainer&& other) noexcept {
        if (this != &other) {
            items = std::move(other.items);
            maxSize = other.maxSize;
            other.maxSize = 0;
        }
        return *this;
    }
    
    // Destructor
    ~GameContainer() = default;
    
    // Add item
    bool addItem(std::unique_ptr<T> item) {
        if (items.size() < maxSize && item) {
            items.push_back(std::move(item));
            return true;
        }
        return false;
    }
    
    // Remove item by index
    bool removeItem(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }
    
    // Get item by index
    T* getItem(size_t index) const {
        if (index < items.size()) {
            return items[index].get();
        }
        return nullptr;
    }
    
    // Get size
    size_t size() const { return items.size(); }
    
    // Check if empty
    bool empty() const { return items.empty(); }
    
    // Clear all items
    void clear() { items.clear(); }
    
    // Find item by predicate
    template<typename Predicate>
    T* findItem(Predicate pred) const {
        auto it = std::find_if(items.begin(), items.end(), 
            [&pred](const std::unique_ptr<T>& item) {
                return pred(item.get());
            });
        return it != items.end() ? it->get() : nullptr;
    }
    
    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const GameContainer& container) {
        os << "GameContainer with " << container.items.size() << " items (max: " << container.maxSize << ")";
        return os;
    }
};

#endif // GAMECONTAINER_H
