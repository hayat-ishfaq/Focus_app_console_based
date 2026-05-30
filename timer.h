#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

enum TimerPreset {
    TIMER_25_MIN = 25,
    TIMER_40_MIN = 40,
    TIMER_60_MIN = 60
};

class FocusTimer {
private:
    int totalMinutes;
    int remainingSeconds;
    bool isRunning;
    bool isPaused;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> pauseTime;
    int pausedSeconds;

public:
    FocusTimer();

    // Timer controls
    void Start(int minutes);
    void Pause();
    void Resume();
    void Stop();
    void Update();

    // Getters
    bool IsRunning() const { return isRunning; }
    bool IsPaused() const { return isPaused; }
    int GetRemainingSeconds() const { return remainingSeconds; }
    int GetTotalMinutes() const { return totalMinutes; }
    float GetProgress() const;
    std::string GetFormattedTime() const;
    
    // Check if timer completed
    bool IsCompleted() const { return isRunning && remainingSeconds <= 0; }
};

#endif
