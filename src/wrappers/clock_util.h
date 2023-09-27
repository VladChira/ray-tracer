
#pragma once
#include <chrono>
#include <cmath>

class HiResTimer
{
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, stop_time;
    std::chrono::duration<double, std::milli> duration;

    HiResTimer()
    {
    }

    void start()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        stop_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);
    }

    double elapsed_time_milliseconds()
    {
        return std::round(duration.count() * 100) / 100;
    }
    double elapsed_time_seconds()
    {
        return duration.count() / 1000;
    }
};