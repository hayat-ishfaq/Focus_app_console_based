#ifndef TASKS_H
#define TASKS_H

#include <string>
#include <vector>

struct Task {
    int id;
    std::string description;
    bool completed;
    std::string dateAdded;

    Task(int id, const std::string& desc, bool comp = false, const std::string& date = "");
};

class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId;

public:
    TaskManager();

    // Task operations
    void AddTask(const std::string& description);
    void CompleteTask(int id);
    void DeleteTask(int id);
    
    // Getters
    const std::vector<Task>& GetTasks() const { return tasks; }
    int GetTaskCount() const { return tasks.size(); }
    int GetCompletedCount() const;
    int GetPendingCount() const;

    // File operations
    void SaveToFile(const std::string& filename);
    void LoadFromFile(const std::string& filename);

private:
    std::string GetCurrentDate() const;
};

#endif
