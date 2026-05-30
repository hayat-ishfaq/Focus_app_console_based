#include "ui.h"
#include "timer.h"
#include "tasks.h"
#include "stats.h"
#include "blocker.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

// Screen IDs
enum Screen {
    SCREEN_HOME = 0,
    SCREEN_TIMER = 1,
    SCREEN_TASKS = 2,
    SCREEN_STATS = 3,
    SCREEN_BLOCK = 4,
    SCREEN_EXIT = 5
};

// Input state
enum InputMode {
    MODE_NAVIGATION,
    MODE_TIMER_SELECT,
    MODE_TIMER_RUNNING,
    MODE_TASK_ADD,
    MODE_TASK_MANAGE,
    MODE_BLOCK_ADD
};

class FocusApp {
private:
    UI ui;
    FocusTimer timer;
    TaskManager taskManager;
    Statistics stats;
    Blocker blocker;
    
    bool running;
    InputMode inputMode;
    int timerSelection;
    int taskSelection;
    std::string inputBuffer;
    bool needsRedraw;
    int lastTimerSeconds;
    
    // File paths
    const std::string TASKS_FILE = "tasks.dat";
    const std::string STATS_FILE = "stats.dat";
    const std::string BLOCKED_SITES_FILE = "blocked_sites.dat";

public:
    FocusApp() : running(true), inputMode(MODE_NAVIGATION), 
                 timerSelection(0), taskSelection(0), needsRedraw(true), lastTimerSeconds(-1) {}

    void Initialize() {
        ui.InitConsole();
        taskManager.LoadFromFile(TASKS_FILE);
        stats.LoadFromFile(STATS_FILE);
        blocker.LoadCustomWebsites(BLOCKED_SITES_FILE);
    }

    void Run() {
        needsRedraw = true;
        while (running) {
            // Only redraw if needed
            if (needsRedraw) {
                DrawCurrentScreen();
                needsRedraw = false;
            }
            
            // Update timer if running
            if (timer.IsRunning()) {
                timer.Update();
                
                // Only redraw timer screen if seconds changed
                if (timer.GetRemainingSeconds() != lastTimerSeconds) {
                    lastTimerSeconds = timer.GetRemainingSeconds();
                    if (inputMode == MODE_TIMER_RUNNING) {
                        needsRedraw = true;
                    }
                }
                
                if (timer.IsCompleted()) {
                    OnTimerCompleted();
                }
            }
            
            HandleInput();
            Sleep(100); // Prevent excessive CPU usage
        }
    }

    void Shutdown() {
        taskManager.SaveToFile(TASKS_FILE);
        stats.SaveToFile(STATS_FILE);
        blocker.SaveCustomWebsites(BLOCKED_SITES_FILE);
    }

private:
    void DrawCurrentScreen() {
        ui.ClearScreen();
        
        // Draw header first
        std::string headerTitle;
        switch (ui.GetCurrentScreen()) {
            case SCREEN_HOME:
                headerTitle = "FOCUS & PRODUCTIVITY APP";
                break;
            case SCREEN_TIMER:
                headerTitle = "FOCUS TIMER";
                break;
            case SCREEN_TASKS:
                headerTitle = "TASK MANAGER";
                break;
            case SCREEN_STATS:
                headerTitle = "STATISTICS & PROGRESS";
                break;
            case SCREEN_BLOCK:
                headerTitle = "APP/WEBSITE BLOCKER";
                break;
            default:
                headerTitle = "FOCUS APP";
        }
        
        ui.DrawHeader(headerTitle);
        
        // Draw sidebar
        ui.DrawSidebar(ui.GetSelectedMenuItem());
        
        // Draw content based on screen
        switch (ui.GetCurrentScreen()) {
            case SCREEN_HOME:
                DrawHomeScreen();
                break;
            case SCREEN_TIMER:
                DrawTimerScreen();
                break;
            case SCREEN_TASKS:
                DrawTasksScreen();
                break;
            case SCREEN_STATS:
                DrawStatsScreen();
                break;
            case SCREEN_BLOCK:
                DrawBlockScreen();
                break;
        }
    }

    void DrawHomeScreen() {
        int x = CONTENT_X;
        int y = CONTENT_Y;
        
        ui.DrawBox(x, y, CONTENT_WIDTH, 20, COLOR_BORDER);
        
        ui.DrawCenteredText(x, y + 2, CONTENT_WIDTH, "Welcome to Focus App!", COLOR_HEADER);
        ui.DrawCenteredText(x, y + 3, CONTENT_WIDTH, "Stay Focused, Stay Productive", COLOR_TEXT);
        
        // Quick stats
        y += 6;
        ui.DrawText(x + 5, y, "Today's Focus Time: " + std::to_string(stats.GetTodayMinutes()) + " minutes", COLOR_SUCCESS);
        ui.DrawText(x + 5, y + 1, "Today's Sessions: " + std::to_string(stats.GetTodaySessionCount()), COLOR_SUCCESS);
        ui.DrawText(x + 5, y + 3, "Pending Tasks: " + std::to_string(taskManager.GetPendingCount()), COLOR_WARNING);
        ui.DrawText(x + 5, y + 4, "Completed Tasks: " + std::to_string(taskManager.GetCompletedCount()), COLOR_SUCCESS);
        
        y += 7;
        ui.DrawCenteredText(x, y, CONTENT_WIDTH, "Use Arrow Keys to navigate", COLOR_BORDER);
        ui.DrawCenteredText(x, y + 1, CONTENT_WIDTH, "Press Enter to select", COLOR_BORDER);
        
        ui.DrawFooter("Navigate: Arrow Keys | Select: Enter | Exit: ESC");
    }

    void DrawTimerScreen() {
        ui.DrawHeader("FOCUS TIMER");
        
        int x = CONTENT_X;
        int y = CONTENT_Y;
        
        if (inputMode == MODE_TIMER_RUNNING) {
            // Timer running view
            ui.DrawBox(x, y, CONTENT_WIDTH, 20, COLOR_BORDER);
            
            ui.DrawCenteredText(x, y + 3, CONTENT_WIDTH, "FOCUS SESSION IN PROGRESS", COLOR_HEADER);
            
            // Display timer
            std::string timeStr = timer.GetFormattedTime();
            ui.DrawCenteredText(x, y + 6, CONTENT_WIDTH, timeStr, COLOR_SUCCESS);
            ui.SetColor(COLOR_SUCCESS);
            for (int i = 0; i < 5; i++) {
                ui.SetCursorPosition(x + CONTENT_WIDTH/2 - 10 + i*4, y + 7);
                std::cout << char(254); // Small circle
            }
            
            // Progress bar
            ui.DrawProgressBar(x + 10, y + 10, CONTENT_WIDTH - 20, timer.GetProgress(), COLOR_PROGRESS);
            
            std::string progressText = std::to_string((int)(timer.GetProgress() * 100)) + "% Complete";
            ui.DrawCenteredText(x, y + 11, CONTENT_WIDTH, progressText, COLOR_TEXT);
            
            // Status
            std::string status = timer.IsPaused() ? "[PAUSED]" : "[RUNNING]";
            ui.DrawCenteredText(x, y + 14, CONTENT_WIDTH, status, timer.IsPaused() ? COLOR_WARNING : COLOR_SUCCESS);
            
            ui.DrawFooter("Pause: P | Stop: S | Back: ESC");
        } else {
            // Timer selection view
            ui.DrawBox(x, y, CONTENT_WIDTH, 20, COLOR_BORDER);
            
            ui.DrawCenteredText(x, y + 2, CONTENT_WIDTH, "SELECT FOCUS DURATION", COLOR_HEADER);
            
            std::vector<std::string> options = {
                "25 Minutes (Pomodoro)",
                "40 Minutes (Deep Work)",
                "60 Minutes (Extended Focus)"
            };
            
            int optY = y + 6;
            for (size_t i = 0; i < options.size(); i++) {
                int color = (i == timerSelection) ? COLOR_SELECTED : COLOR_TEXT;
                std::string prefix = (i == timerSelection) ? ">>> " : "    ";
                ui.DrawText(x + 10, optY + i * 3, prefix + options[i], color);
            }
            
            ui.DrawFooter("Navigate: Up/Down Arrow | Start: Enter | Back: ESC");
        }
    }

    void DrawTasksScreen() {
        ui.DrawHeader("TASK MANAGER");
        
        int x = CONTENT_X;
        int y = CONTENT_Y;
        
        if (inputMode == MODE_TASK_ADD) {
            // Add task mode
            ui.DrawBox(x, y, CONTENT_WIDTH, 20, COLOR_BORDER);
            ui.DrawCenteredText(x, y + 2, CONTENT_WIDTH, "ADD NEW TASK", COLOR_HEADER);
            
            ui.DrawText(x + 5, y + 5, "Enter task description:", COLOR_TEXT);
            ui.DrawBox(x + 5, y + 6, CONTENT_WIDTH - 10, 3, COLOR_BORDER);
            ui.DrawText(x + 7, y + 7, inputBuffer + "_", COLOR_TEXT);
            
            ui.DrawFooter("Type your task | Enter: Save | ESC: Cancel");
        } else {
            // View tasks mode
            ui.DrawBox(x, y, CONTENT_WIDTH, 22, COLOR_BORDER);
            
            ui.DrawText(x + 5, y + 1, "TASKS OVERVIEW", COLOR_HEADER);
            ui.DrawText(x + CONTENT_WIDTH - 35, y + 1, 
                       "Pending: " + std::to_string(taskManager.GetPendingCount()) + 
                       " | Completed: " + std::to_string(taskManager.GetCompletedCount()), 
                       COLOR_SUCCESS);
            
            ui.DrawHorizontalLine(x + 2, y + 2, CONTENT_WIDTH - 4, COLOR_BORDER);
            
            const auto& tasks = taskManager.GetTasks();
            int startY = y + 4;
            int maxDisplay = 13;
            
            if (tasks.empty()) {
                ui.DrawCenteredText(x, startY + 3, CONTENT_WIDTH, "No tasks yet. Press 'A' to add a task!", COLOR_BORDER);
            } else {
                for (size_t i = 0; i < tasks.size() && i < maxDisplay; i++) {
                    const auto& task = tasks[i];
                    std::string status = task.completed ? "[X]" : "[ ]";
                    int color = task.completed ? COLOR_BORDER : COLOR_TEXT;
                    
                    std::string prefix = (i == taskSelection && inputMode == MODE_TASK_MANAGE) ? "> " : "  ";
                    std::string taskText = prefix + status + " " + task.description;
                    
                    if (taskText.length() > CONTENT_WIDTH - 10) {
                        taskText = taskText.substr(0, CONTENT_WIDTH - 13) + "...";
                    }
                    
                    ui.DrawText(x + 3, startY + i, taskText, color);
                }
            }
            
            if (inputMode == MODE_TASK_MANAGE) {
                ui.DrawFooter("Navigate: Up/Down | Complete: C | Delete: D | Add: A | Back: ESC");
            } else {
                ui.DrawFooter("Add Task: A | Manage Tasks: M | Back: ESC");
            }
        }
    }

    void DrawStatsScreen() {
        ui.DrawHeader("STATISTICS & PROGRESS");
        
        int x = CONTENT_X;
        int y = CONTENT_Y;
        
        ui.DrawBox(x, y, CONTENT_WIDTH, 22, COLOR_BORDER);
        
        // Overall stats
        ui.DrawText(x + 5, y + 2, "OVERALL STATISTICS", COLOR_HEADER);
        ui.DrawHorizontalLine(x + 2, y + 3, CONTENT_WIDTH - 4, COLOR_BORDER);
        
        int statY = y + 5;
        ui.DrawText(x + 5, statY, "Total Focus Time: " + std::to_string(stats.GetTotalMinutes()) + " minutes", COLOR_SUCCESS);
        ui.DrawText(x + 5, statY + 1, "Total Sessions: " + std::to_string(stats.GetSessionCount()), COLOR_SUCCESS);
        ui.DrawText(x + 5, statY + 2, "Today's Focus: " + std::to_string(stats.GetTodayMinutes()) + " minutes", COLOR_SUCCESS);
        ui.DrawText(x + 5, statY + 3, "This Week: " + std::to_string(stats.GetWeekMinutes()) + " minutes", COLOR_SUCCESS);
        
        // Weekly chart
        statY += 6;
        ui.DrawText(x + 5, statY, "WEEKLY ACTIVITY (Last 7 Days)", COLOR_HEADER);
        ui.DrawHorizontalLine(x + 2, statY + 1, CONTENT_WIDTH - 4, COLOR_BORDER);
        
        auto weekData = stats.GetWeeklyData();
        statY += 3;
        
        if (weekData.empty()) {
            ui.DrawCenteredText(x, statY + 2, CONTENT_WIDTH, "No activity this week. Start a focus session!", COLOR_BORDER);
        } else {
            int maxMinutes = 1;
            for (const auto& pair : weekData) {
                if (pair.second > maxMinutes) maxMinutes = pair.second;
            }
            
            int chartY = statY;
            for (const auto& pair : weekData) {
                int barLength = (pair.second * 40) / maxMinutes;
                if (barLength < 1 && pair.second > 0) barLength = 1;
                
                ui.DrawText(x + 5, chartY, pair.first + " ", COLOR_TEXT);
                ui.DrawProgressBar(x + 18, chartY, 50, (float)pair.second / maxMinutes, COLOR_PROGRESS);
                ui.DrawText(x + 70, chartY, std::to_string(pair.second) + " min", COLOR_SUCCESS);
                
                chartY++;
            }
        }
        
        ui.DrawFooter("Back: ESC");
    }

    void DrawBlockScreen() {
        ui.DrawHeader("APP/WEBSITE BLOCKER");
        
        int x = CONTENT_X;
        int y = CONTENT_Y;
        
        if (inputMode == MODE_BLOCK_ADD) {
            // Add website mode
            ui.DrawCenteredText(x, y + 2, CONTENT_WIDTH, "ADD WEBSITE TO BLOCK", COLOR_HEADER);
            ui.DrawHorizontalLine(x + 5, y + 3, CONTENT_WIDTH - 10, COLOR_BORDER);
            
            ui.DrawText(x + 5, y + 5, "Enter website URL (e.g., facebook.com):", COLOR_TEXT);
            ui.DrawText(x + 5, y + 6, "Do NOT include http:// or https://", COLOR_BORDER);
            
            // Input box
            ui.DrawBox(x + 5, y + 8, CONTENT_WIDTH - 10, 3, COLOR_BORDER);
            ui.DrawText(x + 7, y + 9, inputBuffer + "_", COLOR_TEXT);
            
            ui.DrawText(x + 5, y + 12, "Examples:", COLOR_HEADER);
            ui.DrawText(x + 8, y + 13, "- twitter.com", COLOR_BORDER);
            ui.DrawText(x + 8, y + 14, "- www.reddit.com", COLOR_BORDER);
            ui.DrawText(x + 8, y + 15, "- gaming-site.net", COLOR_BORDER);
            
            ui.DrawFooter("Type website URL | Enter: Add | ESC: Cancel");
        } else {
            // View mode
            ui.DrawCenteredText(x, y + 2, CONTENT_WIDTH, "FOCUS MODE BLOCKING", COLOR_HEADER);
            ui.DrawHorizontalLine(x + 5, y + 3, CONTENT_WIDTH - 10, COLOR_BORDER);
            
            // Admin status check
            if (!blocker.HasAdminRights()) {
                ui.DrawText(x + 5, y + 5, "WARNING: No administrator privileges detected!", COLOR_WARNING);
                ui.DrawText(x + 5, y + 6, "Please restart the app as Administrator to enable blocking.", COLOR_WARNING);
                ui.DrawText(x + 5, y + 8, "Right-click FocusApp.exe > Run as Administrator", COLOR_TEXT);
            } else {
                // Status indicator
                std::string status = blocker.IsActive() ? "[ACTIVE]" : "[INACTIVE]";
                int statusColor = blocker.IsActive() ? COLOR_SUCCESS : COLOR_BORDER;
                ui.DrawText(x + 5, y + 5, "Blocking Status: ", COLOR_TEXT);
                ui.DrawText(x + 22, y + 5, status, statusColor);
                
                // Controls
                ui.DrawText(x + 5, y + 7, "Press SPACE to " + std::string(blocker.IsActive() ? "DISABLE" : "ENABLE") + " blocking", COLOR_HEADER);
            }
            
            ui.DrawHorizontalLine(x + 5, y + 9, CONTENT_WIDTH - 10, COLOR_BORDER);
            ui.DrawText(x + 5, y + 10, "BLOCKED WEBSITES (" + std::to_string(blocker.GetBlockedWebsites().size()) + "):", COLOR_HEADER);
            
            const auto& websites = blocker.GetBlockedWebsites();
            int listY = y + 11;
            int maxDisplay = 8;
            for (size_t i = 0; i < websites.size() && i < maxDisplay; i++) {
                ui.DrawText(x + 8, listY + i, "- " + websites[i], COLOR_BORDER);
            }
            if (websites.size() > maxDisplay) {
                ui.DrawText(x + 8, listY + maxDisplay, "... and " + std::to_string(websites.size() - maxDisplay) + " more", COLOR_BORDER);
            }
            
            ui.DrawFooter("Toggle: SPACE | Add Site: A | Back: ESC");
        }
    }

    void HandleInput() {
        // Use _kbhit() to check if key is available
        if (!_kbhit()) {
            return; // No key pressed, return early
        }
        
        int key = _getch();
        
        // Handle special keys (arrow keys, function keys)
        if (key == 0 || key == 224) {
            key = _getch(); // Get the actual key code
            
            if (inputMode == MODE_NAVIGATION) {
                if (key == 72) { // Up arrow
                    int item = ui.GetSelectedMenuItem();
                    if (item > 0) {
                        ui.SetSelectedMenuItem(item - 1);
                        needsRedraw = true;
                    }
                } else if (key == 80) { // Down arrow
                    int item = ui.GetSelectedMenuItem();
                    if (item < 5) {
                        ui.SetSelectedMenuItem(item + 1);
                        needsRedraw = true;
                    }
                }
            } else if (inputMode == MODE_TIMER_SELECT) {
                if (key == 72) { // Up arrow
                    if (timerSelection > 0) {
                        timerSelection--;
                        needsRedraw = true;
                    }
                } else if (key == 80) { // Down arrow
                    if (timerSelection < 2) {
                        timerSelection++;
                        needsRedraw = true;
                    }
                }
            } else if (inputMode == MODE_TASK_MANAGE) {
                const auto& tasks = taskManager.GetTasks();
                if (key == 72) { // Up arrow
                    if (taskSelection > 0) {
                        taskSelection--;
                        needsRedraw = true;
                    }
                } else if (key == 80) { // Down arrow
                    if (taskSelection < tasks.size() - 1) {
                        taskSelection++;
                        needsRedraw = true;
                    }
                }
            }
        } else {
            // Regular key handling
            if (key == 27) { // ESC
                HandleEscapeKey();
            } else if (key == 13) { // Enter
                HandleEnterKey();
            } else if (inputMode == MODE_TIMER_RUNNING) {
                if (key == 'p' || key == 'P') {
                    if (timer.IsPaused()) {
                        timer.Resume();
                    } else {
                        timer.Pause();
                    }
                    needsRedraw = true;
                } else if (key == 's' || key == 'S') {
                    timer.Stop();
                    inputMode = MODE_TIMER_SELECT;
                    needsRedraw = true;
                }
            } else if (inputMode == MODE_TASK_ADD) {
                if (key == 8) { // Backspace
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                        needsRedraw = true;
                    }
                } else if (key >= 32 && key <= 126) {
                    if (inputBuffer.length() < 60) {
                        inputBuffer += (char)key;
                        needsRedraw = true;
                    }
                }
            } else if (inputMode == MODE_BLOCK_ADD) {
                if (key == 8) { // Backspace
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                        needsRedraw = true;
                    }
                } else if (key >= 32 && key <= 126) {
                    if (inputBuffer.length() < 100) {
                        inputBuffer += (char)key;
                        needsRedraw = true;
                    }
                }
            } else if (inputMode == MODE_TASK_MANAGE) {
                if (key == 'c' || key == 'C') {
                    const auto& tasks = taskManager.GetTasks();
                    if (!tasks.empty() && taskSelection < tasks.size()) {
                        taskManager.CompleteTask(tasks[taskSelection].id);
                        taskManager.SaveToFile(TASKS_FILE);
                        needsRedraw = true;
                    }
                } else if (key == 'd' || key == 'D') {
                    const auto& tasks = taskManager.GetTasks();
                    if (!tasks.empty() && taskSelection < tasks.size()) {
                        taskManager.DeleteTask(tasks[taskSelection].id);
                        taskManager.SaveToFile(TASKS_FILE);
                        if (taskSelection > 0) taskSelection--;
                        needsRedraw = true;
                    }
                } else if (key == 'a' || key == 'A') {
                    inputMode = MODE_TASK_ADD;
                    inputBuffer.clear();
                    needsRedraw = true;
                }
            } else if (ui.GetCurrentScreen() == SCREEN_TASKS && inputMode == MODE_NAVIGATION) {
                if (key == 'a' || key == 'A') {
                    inputMode = MODE_TASK_ADD;
                    inputBuffer.clear();
                    needsRedraw = true;
                } else if (key == 'm' || key == 'M') {
                    inputMode = MODE_TASK_MANAGE;
                    taskSelection = 0;
                    needsRedraw = true;
                }
            } else if (ui.GetCurrentScreen() == SCREEN_BLOCK && inputMode == MODE_NAVIGATION) {
                if (key == ' ') { // Space bar
                    if (blocker.HasAdminRights()) {
                        if (blocker.IsActive()) {
                            blocker.Deactivate();
                        } else {
                            blocker.Activate();
                        }
                        needsRedraw = true;
                    }
                } else if (key == 'a' || key == 'A') {
                    inputMode = MODE_BLOCK_ADD;
                    inputBuffer.clear();
                    needsRedraw = true;
                }
            }
        }
    }
    
    void HandleEscapeKey() {
        if (inputMode == MODE_TIMER_RUNNING) {
            timer.Stop();
            inputMode = MODE_NAVIGATION;
        } else if (inputMode == MODE_TASK_ADD || inputMode == MODE_TASK_MANAGE || inputMode == MODE_BLOCK_ADD) {
            inputMode = MODE_NAVIGATION;
            inputBuffer.clear();
            taskSelection = 0;
        } else if (inputMode == MODE_TIMER_SELECT) {
            inputMode = MODE_NAVIGATION;
        } else {
            running = false;
        }
        needsRedraw = true;
    }
    
    void HandleEnterKey() {
        if (inputMode == MODE_NAVIGATION) {
            int selected = ui.GetSelectedMenuItem();
            if (selected == SCREEN_EXIT) {
                running = false;
            } else {
                ui.SetCurrentScreen(selected);
                if (selected == SCREEN_TIMER) {
                    inputMode = MODE_TIMER_SELECT;
                }
                needsRedraw = true;
            }
        } else if (inputMode == MODE_TIMER_SELECT) {
            int duration = 25;
            if (timerSelection == 0) duration = 25;
            else if (timerSelection == 1) duration = 40;
            else if (timerSelection == 2) duration = 60;
            
            timer.Start(duration);
            inputMode = MODE_TIMER_RUNNING;
            lastTimerSeconds = timer.GetRemainingSeconds();
            needsRedraw = true;
        } else if (inputMode == MODE_TASK_ADD) {
            if (!inputBuffer.empty()) {
                taskManager.AddTask(inputBuffer);
                taskManager.SaveToFile(TASKS_FILE);
                inputBuffer.clear();
            }
            inputMode = MODE_NAVIGATION;
            needsRedraw = true;
        } else if (inputMode == MODE_BLOCK_ADD) {
            if (!inputBuffer.empty()) {
                // Add website to blocker
                blocker.AddWebsite(inputBuffer);
                blocker.SaveCustomWebsites(BLOCKED_SITES_FILE);
                // If blocking is active, reapply it with new website
                if (blocker.IsActive()) {
                    blocker.Deactivate();
                    blocker.Activate();
                }
                inputBuffer.clear();
            }
            inputMode = MODE_NAVIGATION;
            needsRedraw = true;
        }
    }

    void OnTimerCompleted() {
        // Save session to statistics
        stats.AddSession(timer.GetTotalMinutes());
        stats.SaveToFile(STATS_FILE);
        
        // Show completion message
        ui.ClearScreen();
        ui.DrawHeader("SESSION COMPLETE!");
        
        int x = CONTENT_X;
        int y = CONTENT_Y + 5;
        
        ui.DrawBox(x, y, CONTENT_WIDTH, 10, COLOR_SUCCESS);
        ui.DrawCenteredText(x, y + 3, CONTENT_WIDTH, "CONGRATULATIONS!", COLOR_SUCCESS);
        ui.DrawCenteredText(x, y + 4, CONTENT_WIDTH, 
                           "You completed a " + std::to_string(timer.GetTotalMinutes()) + "-minute focus session!", 
                           COLOR_TEXT);
        ui.DrawCenteredText(x, y + 6, CONTENT_WIDTH, "Press any key to continue...", COLOR_BORDER);
        
        // Wait for key press
        while (!_kbhit()) {
            Sleep(100);
        }
        _getch(); // Clear the key
        
        timer.Stop();
        inputMode = MODE_NAVIGATION;
        ui.SetCurrentScreen(SCREEN_HOME);
        needsRedraw = true;
    }
};

int main() {
    FocusApp app;
    app.Initialize();
    app.Run();
    app.Shutdown();
    
    return 0;
}
