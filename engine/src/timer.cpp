#include <chrono>
#include <iostream>
#include <thread>

#include "engine/timer.h"

namespace engine {
Timer::Timer(float time, bool isRepeating, std::function<void()> callback)
    : _time(time), _isRepeating(isRepeating), _callback(callback) {}

Timer::Timer(Timer&& other) noexcept
        : _time(std::exchange(other._time, 0.0f)),
          _isRepeating(std::exchange(other._isRepeating, false)),
          _isRunning(std::exchange(other._isRunning, false)),
          _callback(std::move(other._callback)),
          _executionThread(std::move(other._executionThread)) {
        // Transfer resources from 'other' to this object
    }

Timer& Timer::operator=(Timer&& other) noexcept {
    if (this != &other) {  // Check for self-assignment
        // Stop the current timer, if it's running
        if (_isRunning) {
            stop();
        }

        // Move the members from 'other'
        _time = std::exchange(other._time, 0.0f);
        _isRepeating = std::exchange(other._isRepeating, false);
        _callback = std::move(other._callback);
        _executionThread = std::move(other._executionThread);

        // Start the timer if it was running in the 'other' Timer
        if (other._isRunning) {
            start();
        }
    }
    return *this;
}

void Timer::start() {
  _executionThread = std::thread(&Timer::run, this);
  _isRunning = true;
}

void Timer::stop() {
  _isRunning = false;
  // _executionThread.join();
  cv.notify_all(); // Notify the thread to stqop
  if (_executionThread.joinable()) {
    _executionThread.join();
  }
}

void Timer::restart() {}

void Timer::run() {
  do {
    std::unique_lock<std::mutex> lock(mtx);

    if (cv.wait_for(lock, std::chrono::milliseconds((int) (_time * 1000)),
                    [this] { return !_isRunning; })) {
      // Stop request received
      return;
    }
    // std::this_thread::sleep_for(
        // std::chrono::mill/iseconds(static_cast<int>(_time * 1000)));
    if (_callback) {
      _callback();
    }
  } while (_isRunning && _isRepeating);
}

} // namespace engine