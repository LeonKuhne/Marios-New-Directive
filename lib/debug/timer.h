#pragma once
#include <SDL2/SDL.h>
#include <functional>

class Timer {
public:
    static void measure(char const* label, std::function<void()> func) {
        Timer timer;
        timer.start();
        func();
        double elapsed = timer.stop();
        SDL_Log("%s: Elapsed Time: %f ms", label, elapsed);
    }
private:
    void start() {
        start_time = SDL_GetPerformanceCounter();
    }
    double stop() {
        Uint64 end = SDL_GetPerformanceCounter();
        return (end - start_time) * 1000.0 / SDL_GetPerformanceFrequency();
    }
    Uint64 start_time;
};