#include "ui.h"
#include <iostream>

UI::UI() : currentScreen(0), selectedMenuItem(0) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

UI::~UI() {
    SetColor(COLOR_DEFAULT);
    ShowCursor();
}

void UI::InitConsole() {
    // Set console window size
    SMALL_RECT windowSize = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    // Set console buffer size
    COORD bufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set console title
    SetConsoleTitleA("Focus & Productivity App");

    // Hide cursor
    HideCursor();

    // Set default color
    SetColor(COLOR_DEFAULT);
}

void UI::ClearScreen() {
    system("cls");
}

void UI::SetCursorPosition(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, coord);
}

void UI::SetColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void UI::HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void UI::ShowCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void UI::DrawBox(int x, int y, int width, int height, int color) {
    SetColor(color);
    
    // Top border
    SetCursorPosition(x, y);
    std::cout << char(201); // Top-left corner
    for (int i = 0; i < width - 2; i++) std::cout << char(205); // Horizontal line
    std::cout << char(187); // Top-right corner

    // Sides
    for (int i = 1; i < height - 1; i++) {
        SetCursorPosition(x, y + i);
        std::cout << char(186); // Vertical line
        SetCursorPosition(x + width - 1, y + i);
        std::cout << char(186);
    }

    // Bottom border
    SetCursorPosition(x, y + height - 1);
    std::cout << char(200); // Bottom-left corner
    for (int i = 0; i < width - 2; i++) std::cout << char(205);
    std::cout << char(188); // Bottom-right corner
}

void UI::DrawHorizontalLine(int x, int y, int length, int color) {
    SetColor(color);
    SetCursorPosition(x, y);
    for (int i = 0; i < length; i++) {
        std::cout << char(196);
    }
}

void UI::DrawVerticalLine(int x, int y, int length, int color) {
    SetColor(color);
    for (int i = 0; i < length; i++) {
        SetCursorPosition(x, y + i);
        std::cout << char(179);
    }
}

void UI::DrawText(int x, int y, const std::string& text, int color) {
    SetColor(color);
    SetCursorPosition(x, y);
    std::cout << text;
}

void UI::DrawCenteredText(int x, int y, int width, const std::string& text, int color) {
    int textX = x + (width - text.length()) / 2;
    DrawText(textX, y, text, color);
}

void UI::DrawProgressBar(int x, int y, int width, float progress, int color) {
    SetCursorPosition(x, y);
    SetColor(COLOR_BORDER);
    std::cout << "[";
    
    int filled = (int)(progress * (width - 2));
    SetColor(color);
    for (int i = 0; i < filled; i++) {
        std::cout << char(219); // Solid block
    }
    
    SetColor(COLOR_BORDER);
    for (int i = filled; i < width - 2; i++) {
        std::cout << char(176); // Light shade
    }
    std::cout << "]";
}

void UI::DrawHeader(const std::string& title) {
    // Draw header box
    DrawBox(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, COLOR_HEADER);
    
    // Draw title centered
    DrawCenteredText(0, 1, SCREEN_WIDTH, "=== " + title + " ===", COLOR_HEADER);
}

void UI::DrawSidebar(int selectedItem) {
    // Draw sidebar box
    DrawBox(0, HEADER_HEIGHT, SIDEBAR_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT, COLOR_SIDEBAR);
    
    // Menu items
    std::vector<std::string> menuItems = {
        "Home",
        "Focus Timer",
        "Tasks",
        "Statistics",
        "Block Apps",
        "Exit"
    };

    int startY = HEADER_HEIGHT + 2;
    for (size_t i = 0; i < menuItems.size(); i++) {
        int color = (i == selectedItem) ? COLOR_SELECTED : COLOR_TEXT;
        std::string prefix = (i == selectedItem) ? "> " : "  ";
        DrawText(2, startY + i * 2, prefix + menuItems[i], color);
    }
}

void UI::DrawFooter(const std::string& message) {
    // Clear the footer area first
    SetColor(COLOR_DEFAULT);
    SetCursorPosition(0, SCREEN_HEIGHT - 2);
    for (int i = 0; i < SCREEN_WIDTH; i++) std::cout << " ";
    SetCursorPosition(0, SCREEN_HEIGHT - 1);
    for (int i = 0; i < SCREEN_WIDTH; i++) std::cout << " ";
    
    // Draw footer line and text
    SetColor(COLOR_BORDER);
    DrawHorizontalLine(0, SCREEN_HEIGHT - 2, SCREEN_WIDTH, COLOR_BORDER);
    DrawText(2, SCREEN_HEIGHT - 1, message, COLOR_TEXT);
}

void UI::SetCurrentScreen(int screen) {
    currentScreen = screen;
}
