#pragma once;

#include <chrono>


//measure the time of the call "func(c)", returns as pair the result of the call and the measured time in milliseconds 
template<class Container, class Func> 
auto measure_milliseconds(const Container& c, Func func) -> std::pair<decltype(func(c)), long long>
{
    using namespace std::chrono;

    auto start  = high_resolution_clock::now();
    auto result = func(c);
    auto end    = high_resolution_clock::now();
    return std::make_pair(result, duration_cast<milliseconds>(end - start).count());
}


//measure the time of the call "func(c)", returns as pair the result of the call and the measured time in nanoseconds 
template<class Container, class Func> 
auto measure_nanoseconds(const Container& c, Func func) -> std::pair<decltype(func(c)), long long>
{
    using namespace std::chrono;

    auto start  = high_resolution_clock::now();
    auto result = func(c);
    auto end    = high_resolution_clock::now();
    return std::make_pair(result, duration_cast<nanoseconds>(end - start).count());
}
