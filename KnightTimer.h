#pragma once
#include <chrono>

class KnightTimer {
public:
    KnightTimer();
    float Mark();
    float Peek() const;
private:
    std::chrono::steady_clock::time_point last;
};