#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <iostream>
#include <iterator>

// Template Function - Generic utility functions for game mechanics

// Calculate average of any numeric container
template<typename Container>
typename Container::value_type calculateAverage(const Container& container) {
    static_assert(std::is_arithmetic_v<typename Container::value_type>, 
                  "Container must contain numeric types");
    
    if (container.empty()) {
        return typename Container::value_type{};
    }
    
    auto sum = std::accumulate(container.begin(), container.end(), 
                               typename Container::value_type{});
    return sum / static_cast<typename Container::value_type>(container.size());
}

// Find maximum value in container with custom comparator
template<typename Container, typename Comparator>
typename Container::value_type findMax(const Container& container, Comparator comp) {
    if (container.empty()) {
        return typename Container::value_type{};
    }
    
    auto it = std::max_element(container.begin(), container.end(), comp);
    return *it;
}

// Apply function to all elements in container
template<typename Container, typename Function>
void forEach(Container& container, Function func) {
    std::for_each(container.begin(), container.end(), func);
}

// Filter container based on predicate
template<typename Container, typename Predicate>
std::vector<typename Container::value_type> filter(const Container& container, Predicate pred) {
    std::vector<typename Container::value_type> result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), pred);
    return result;
}

// Transform container elements
template<typename Container, typename TransformFunc>
auto transform(const Container& container, TransformFunc func) 
    -> std::vector<decltype(func(std::declval<typename Container::value_type>()))> {
    
    std::vector<decltype(func(std::declval<typename Container::value_type>()))> result;
    result.reserve(container.size());
    
    std::transform(container.begin(), container.end(), std::back_inserter(result), func);
    return result;
}

// Check if all elements satisfy predicate
template<typename Container, typename Predicate>
bool allOf(const Container& container, Predicate pred) {
    return std::all_of(container.begin(), container.end(), pred);
}

// Check if any element satisfies predicate
template<typename Container, typename Predicate>
bool anyOf(const Container& container, Predicate pred) {
    return std::any_of(container.begin(), container.end(), pred);
}

// Check if no element satisfies predicate
template<typename Container, typename Predicate>
bool noneOf(const Container& container, Predicate pred) {
    return std::none_of(container.begin(), container.end(), pred);
}

#endif // GAMEUTILS_H
