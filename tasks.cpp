#include "tasks.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

Task::Task(int id, const std::string& desc, bool comp, const std::string& date)
    : id(id), description(desc), completed(comp), dateAdded(date) {}

TaskManager::TaskManager() : nextId(1) {}

void TaskManager::AddTask(const std::string& description) {
    Task task(nextId++, description, false, GetCurrentDate());
    tasks.push_back(task);
}

void TaskManager::CompleteTask(int id) {
    for (auto& task : tasks) {
        if (task.id == id) {
            task.completed = true;
            break;
        }
    }
}

void TaskManager::DeleteTask(int id) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            break;
        }
    }
}

int TaskManager::GetCompletedCount() const {
    int count = 0;
    for (const auto& task : tasks) {
        if (task.completed) count++;
    }
    return count;
}

int TaskManager::GetPendingCount() const {
    return GetTaskCount() - GetCompletedCount();
}

void TaskManager::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << nextId << "\n";
        for (const auto& task : tasks) {
            file << task.id << "|" 
                 << task.description << "|" 
                 << task.completed << "|"
                 << task.dateAdded << "\n";
        }
        file.close();
    }
}

void TaskManager::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        tasks.clear();
        file >> nextId;
        file.ignore(); // Ignore newline

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string idStr, desc, compStr, date;
            
            std::getline(iss, idStr, '|');
            std::getline(iss, desc, '|');
            std::getline(iss, compStr, '|');
            std::getline(iss, date, '|');

            if (!idStr.empty() && !desc.empty()) {
                int id = std::stoi(idStr);
                bool completed = (compStr == "1");
                tasks.push_back(Task(id, desc, completed, date));
            }
        }
        file.close();
    }
}

std::string TaskManager::GetCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", now_tm);
    return std::string(buffer);
}
