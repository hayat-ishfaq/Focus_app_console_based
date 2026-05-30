#ifndef UI_H
#define UI_H

#include <windows.h>
#include <string>
#include <vector>

// Color codes
#define COLOR_DEFAULT 7
#define COLOR_HEADER 11      // Bright cyan
#define COLOR_SIDEBAR 10     // Bright green
#define COLOR_SELECTED 14    // Yellow
#define COLOR_TEXT 15        // White
#define COLOR_BORDER 8       // Gray
#define COLOR_SUCCESS 10     // Green
#define COLOR_WARNING 12     // Red
#define COLOR_PROGRESS 13    // Magenta

// Screen dimensions
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

// Layout positions
#define SIDEBAR_WIDTH 20
#define HEADER_HEIGHT 3
#define CONTENT_X (SIDEBAR_WIDTH + 2)
#define CONTENT_Y (HEADER_HEIGHT + 1)
#define CONTENT_WIDTH (SCREEN_WIDTH - SIDEBAR_WIDTH - 4)

class UI {
private:
    HANDLE hConsole;
    int currentScreen;
    int selectedMenuItem;

public:
    UI();
    ~UI();

    // Console setup
    void InitConsole();
    void ClearScreen();
    void SetCursorPosition(int x, int y);
    void SetColor(int color);
    void HideCursor();
    void ShowCursor();

    // Drawing functions
    void DrawBox(int x, int y, int width, int height, int color);
    void DrawHorizontalLine(int x, int y, int length, int color);
    void DrawVerticalLine(int x, int y, int length, int color);
    void DrawText(int x, int y, const std::string& text, int color);
    void DrawCenteredText(int x, int y, int width, const std::string& text, int color);
    void DrawProgressBar(int x, int y, int width, float progress, int color);

    // Layout components
    void DrawHeader(const std::string& title);
    void DrawSidebar(int selectedItem);
    void DrawFooter(const std::string& message);

    // Screen management
    void SetCurrentScreen(int screen);
    int GetCurrentScreen() const { return currentScreen; }
    void SetSelectedMenuItem(int item) { selectedMenuItem = item; }
    int GetSelectedMenuItem() const { return selectedMenuItem; }
};

#endif
