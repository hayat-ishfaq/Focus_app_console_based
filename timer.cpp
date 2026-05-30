#include "timer.h"
#include <sstream>
#include <iomanip>

FocusTimer::FocusTimer() : totalMinutes(0), remainingSeconds(0), 
                           isRunning(false), isPaused(false), pausedSeconds(0) {}

void FocusTimer::Start(int minutes) {
    totalMinutes = minutes;
    remainingSeconds = minutes * 60;
    isRunning = true;
    isPaused = false;
    pausedSeconds = 0;
    startTime = std::chrono::steady_clock::now();
}

void FocusTimer::Pause() {
    if (isRunning && !isPaused) {
        isPaused = true;
        pauseTime = std::chrono::steady_clock::now();
    }
}

void FocusTimer::Resume() {
    if (isRunning && isPaused) {
        isPaused = false;
        auto pauseDuration = std::chrono::steady_clock::now() - pauseTime;
        pausedSeconds += std::chrono::duration_cast<std::chrono::seconds>(pauseDuration).count();
    }
}

void FocusTimer::Stop() {
    isRunning = false;
    isPaused = false;
    remainingSeconds = 0;
    pausedSeconds = 0;
}

void FocusTimer::Update() {
    if (!isRunning || isPaused) return;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    elapsed -= pausedSeconds;

    remainingSeconds = (totalMinutes * 60) - elapsed;
    if (remainingSeconds < 0) {
        remainingSeconds = 0;
    }
}

float FocusTimer::GetProgress() const {
    if (totalMinutes == 0) return 0.0f;
    int totalSeconds = totalMinutes * 60;
    int elapsedSeconds = totalSeconds - remainingSeconds;
    return (float)elapsedSeconds / (float)totalSeconds;
}

std::string FocusTimer::GetFormattedTime() const {
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes 
        << ":" << std::setfill('0') << std::setw(2) << seconds;
    return oss.str();
}
