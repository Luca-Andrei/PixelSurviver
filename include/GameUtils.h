#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <iostream>
#include <iterator>

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

template<typename Container, typename Comparator>
typename Container::value_type findMax(const Container& container, Comparator comp) {
    if (container.empty()) {
        return typename Container::value_type{};
    }
    
    auto it = std::max_element(container.begin(), container.end(), comp);
    return *it;
}



template<typename Container, typename TransformFunc>
auto transform(const Container& container, TransformFunc func) 
    -> std::vector<decltype(func(std::declval<typename Container::value_type>()))> {
    
    std::vector<decltype(func(std::declval<typename Container::value_type>()))> result;
    result.reserve(container.size());
    
    std::transform(container.begin(), container.end(), std::back_inserter(result), func);
    return result;
}



#endif
