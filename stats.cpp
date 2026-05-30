#include "stats.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

FocusSession::FocusSession(const std::string& d, int dur, const std::string& ts)
    : date(d), durationMinutes(dur), timestamp(ts) {}

Statistics::Statistics() : totalFocusMinutes(0), todayFocusMinutes(0), weekFocusMinutes(0) {}

void Statistics::AddSession(int durationMinutes) {
    FocusSession session(GetCurrentDate(), durationMinutes, GetCurrentTimestamp());
    sessions.push_back(session);
    RecalculateStats();
}

void Statistics::RecalculateStats() {
    totalFocusMinutes = 0;
    todayFocusMinutes = 0;
    weekFocusMinutes = 0;

    for (const auto& session : sessions) {
        totalFocusMinutes += session.durationMinutes;
        
        if (IsToday(session.date)) {
            todayFocusMinutes += session.durationMinutes;
        }
        
        if (IsThisWeek(session.date)) {
            weekFocusMinutes += session.durationMinutes;
        }
    }
}

int Statistics::GetTodaySessionCount() const {
    int count = 0;
    for (const auto& session : sessions) {
        if (IsToday(session.date)) {
            count++;
        }
    }
    return count;
}

std::map<std::string, int> Statistics::GetWeeklyData() const {
    std::map<std::string, int> weekData;
    
    for (const auto& session : sessions) {
        if (IsThisWeek(session.date)) {
            weekData[session.date] += session.durationMinutes;
        }
    }
    
    return weekData;
}

std::vector<FocusSession> Statistics::GetRecentSessions(int count) const {
    std::vector<FocusSession> recent;
    int start = std::max(0, (int)sessions.size() - count);
    
    for (int i = start; i < sessions.size(); i++) {
        recent.push_back(sessions[i]);
    }
    
    return recent;
}

void Statistics::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& session : sessions) {
            file << session.date << "|" 
                 << session.durationMinutes << "|"
                 << session.timestamp << "\n";
        }
        file.close();
    }
}

void Statistics::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        sessions.clear();
        
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string date, durStr, timestamp;
            
            std::getline(iss, date, '|');
            std::getline(iss, durStr, '|');
            std::getline(iss, timestamp, '|');

            if (!date.empty() && !durStr.empty()) {
                int duration = std::stoi(durStr);
                sessions.push_back(FocusSession(date, duration, timestamp));
            }
        }
        file.close();
        RecalculateStats();
    }
}

std::string Statistics::GetCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", now_tm);
    return std::string(buffer);
}

std::string Statistics::GetCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
    return std::string(buffer);
}

bool Statistics::IsToday(const std::string& date) const {
    return date == GetCurrentDate();
}

bool Statistics::IsThisWeek(const std::string& date) const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    
    // Get start of current week (assuming week starts on Monday)
    int daysSinceMonday = (now_tm->tm_wday + 6) % 7; // Convert Sunday=0 to Monday=0
    auto weekStart = now - std::chrono::hours(24 * daysSinceMonday);
    std::time_t weekStart_time = std::chrono::system_clock::to_time_t(weekStart);
    std::tm* weekStart_tm = std::localtime(&weekStart_time);
    
    char weekStartBuffer[32];
    strftime(weekStartBuffer, sizeof(weekStartBuffer), "%Y-%m-%d", weekStart_tm);
    
    return date >= std::string(weekStartBuffer);
}
