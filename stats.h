#ifndef STATS_H
#define STATS_H

#include <string>
#include <vector>
#include <map>

struct FocusSession {
    std::string date;
    int durationMinutes;
    std::string timestamp;

    FocusSession(const std::string& d, int dur, const std::string& ts = "");
};

class Statistics {
private:
    std::vector<FocusSession> sessions;
    int totalFocusMinutes;
    int todayFocusMinutes;
    int weekFocusMinutes;

public:
    Statistics();

    // Session management
    void AddSession(int durationMinutes);
    void RecalculateStats();

    // Getters
    int GetTotalMinutes() const { return totalFocusMinutes; }
    int GetTodayMinutes() const { return todayFocusMinutes; }
    int GetWeekMinutes() const { return weekFocusMinutes; }
    int GetSessionCount() const { return sessions.size(); }
    int GetTodaySessionCount() const;
    
    // Weekly data for charts
    std::map<std::string, int> GetWeeklyData() const;
    std::vector<FocusSession> GetRecentSessions(int count) const;

    // File operations
    void SaveToFile(const std::string& filename);
    void LoadFromFile(const std::string& filename);

private:
    std::string GetCurrentDate() const;
    std::string GetCurrentTimestamp() const;
    bool IsToday(const std::string& date) const;
    bool IsThisWeek(const std::string& date) const;
};

#endif
