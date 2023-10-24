#pragma once

#include <functional>
#include <thread>
#include <condition_variable>

namespace engine {

class Timer {
public:
    Timer() = default;
    Timer(float time, bool repeat, std::function<void()> callback);
    Timer(Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&& other) noexcept;
    Timer& operator=(Timer&& other) noexcept;
    void restart();
    void stop();
    void start();

private:
    void run();
private:
    float _time{};
    bool _isRepeating{};
    bool _isRunning{};
    std::function<void()> _callback;
    std::thread _executionThread;
    std::condition_variable cv;
    std::mutex mtx;

    
};

}