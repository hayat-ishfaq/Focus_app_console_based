# Focus & Productivity App
## Windows Console-Based Productivity Application in C++

A feature-rich, GUI-like console application for Windows that helps you stay focused and productive.

![Console App](https://img.shields.io/badge/Platform-Windows-blue)
![Language](https://img.shields.io/badge/Language-C++-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 🎯 Features

### 1. **Focus Timer (Pomodoro-Style)**
- Three preset durations: 25, 40, or 60 minutes
- Live countdown with visual progress bar
- Pause/Resume functionality
- Automatic session tracking

### 2. **Task Management**
- Add, view, and manage tasks
- Mark tasks as completed
- Delete tasks
- Persistent storage (saves to file)

### 3. **Statistics & Progress Tracking**
- Total focus time tracking
- Daily and weekly statistics
- Session history
- ASCII-style progress charts

### 4. **App-Like UI**
- Fixed layout with header, sidebar, and content area
- Color-coded interface
- Smooth navigation with keyboard controls
- Professional-looking borders and boxes

### 5. **Navigation**
- Intuitive keyboard controls
- Arrow keys for menu navigation
- Context-sensitive commands
- ESC to go back

---

## 🛠️ Technical Stack

- **Language:** C++
- **API:** Windows API (`windows.h`)
- **Features Used:**
  - Console cursor positioning (`SetConsoleCursorPosition`)
  - Console colors (`SetConsoleTextAttribute`)
  - Keyboard input (`GetAsyncKeyState`)
  - Timers (`chrono`, `Sleep`)
  - File I/O for persistence

---

## 📋 Requirements

- **Operating System:** Windows 7 or later
- **Compiler:** 
  - MinGW-w64 (g++)
  - MSYS2 (g++)
  - Or any C++ compiler supporting Windows API

---

## 🚀 Installation & Building

### Option 1: Using PowerShell Build Script (Recommended)
```powershell
.\build.ps1
```

### Option 2: Using Batch File
```cmd
build.bat
```

### Option 3: Using Makefile
```bash
make
```

### Option 4: Manual Compilation
```bash
g++ -o build/FocusApp.exe ui.cpp timer.cpp tasks.cpp stats.cpp main.cpp -static-libgcc -static-libstdc++
```

---

## 🎮 How to Use

### Starting the Application
```bash
.\build\FocusApp.exe
```

### Navigation Controls

#### Main Menu
- **↑/↓ Arrow Keys:** Navigate menu items
- **Enter:** Select menu item
- **ESC:** Exit application

#### Focus Timer
- **↑/↓ Arrow Keys:** Select timer duration
- **Enter:** Start timer
- **P:** Pause/Resume timer
- **S:** Stop timer
- **ESC:** Back to menu

#### Task Manager
- **A:** Add new task
- **M:** Manage tasks mode
- **↑/↓ Arrow Keys:** Navigate tasks (in manage mode)
- **C:** Complete selected task
- **D:** Delete selected task
- **ESC:** Back to menu

#### Statistics
- **ESC:** Back to menu

---

## 📁 Project Structure

```
Focus_app_console_based/
│
├── main.cpp          # Application entry point & screen logic
├── ui.h / ui.cpp     # UI rendering functions
├── timer.h / timer.cpp   # Focus timer logic
├── tasks.h / tasks.cpp   # Task management
├── stats.h / stats.cpp   # Statistics tracking
│
├── build.ps1         # PowerShell build script
├── build.bat         # Batch build script
├── Makefile          # Make build configuration
│
├── tasks.dat         # Task data (created at runtime)
├── stats.dat         # Statistics data (created at runtime)
│
└── build/            # Build output directory
    └── FocusApp.exe  # Compiled executable
```

---

## 🎨 Features Breakdown

### Home Screen
- Welcome message
- Quick statistics overview
- Today's focus time and sessions
- Pending/completed task counts

### Focus Timer Screen
- Duration selection (25/40/60 minutes)
- Live countdown display
- Visual progress bar
- Session status indicator
- Pause/Resume controls

### Task Manager Screen
- List all tasks with status
- Add new tasks with descriptions
- Mark tasks as complete
- Delete tasks
- Task persistence across sessions

### Statistics Screen
- Total focus time
- Session count
- Daily focus summary
- Weekly activity chart
- Historical data

### Block Screen
- Informational screen about focus mode
- List of apps/sites to avoid
- Productivity tips

---

## 💾 Data Persistence

The application saves data to local files:

- **tasks.dat** - Stores all tasks with their status
- **stats.dat** - Stores focus session history

These files are automatically created in the application directory and are loaded on startup.

---

## 🎯 Code Quality Features

✅ **Modular Design:** Separated UI, logic, and data layers  
✅ **Clean Code:** Well-commented and organized  
✅ **Error Handling:** Graceful file operations  
✅ **Memory Management:** Proper resource cleanup  
✅ **Extensible:** Easy to add new features  

---

## 🔧 Customization

You can easily customize:

- **Colors:** Modify color codes in `ui.h`
- **Screen Size:** Adjust `SCREEN_WIDTH` and `SCREEN_HEIGHT`
- **Timer Presets:** Modify `TimerPreset` enum in `timer.h`
- **File Paths:** Change `TASKS_FILE` and `STATS_FILE` in `main.cpp`

---

## 📝 Example Usage

1. **Start a Focus Session:**
   - Navigate to "Focus Timer"
   - Select duration (25/40/60 minutes)
   - Press Enter to start
   - Stay focused until completion!

2. **Manage Tasks:**
   - Navigate to "Tasks"
   - Press 'A' to add a new task
   - Type your task description
   - Press Enter to save
   - Press 'M' to manage tasks
   - Use 'C' to mark complete or 'D' to delete

3. **View Progress:**
   - Navigate to "Statistics"
   - View your focus time and sessions
   - Check weekly activity chart

---

## 🐛 Troubleshooting

### Compiler Not Found
```
Error: g++ compiler not found!
```
**Solution:** Install MinGW-w64 or MSYS2 and ensure g++ is in your PATH.

### Window Size Issues
If the console window doesn't display correctly, try:
- Maximizing the console window
- Adjusting `SCREEN_WIDTH` and `SCREEN_HEIGHT` in `ui.h`

### Data Files Not Saving
Ensure the application has write permissions in its directory.

---

## 🚀 Future Enhancements

Potential features to add:
- [ ] Sound notifications when timer completes
- [ ] Customizable timer durations
- [ ] Task categories/tags
- [ ] Export statistics to CSV
- [ ] Theme customization
- [ ] Multiple user profiles
- [ ] Real app/website blocking (requires admin rights)
- [ ] System tray integration

---

## 📄 License

This project is open-source and available under the MIT License.

---

## 👨‍💻 Author

Created as a demonstration of Windows Console API capabilities and modern C++ practices.

---

## 🙏 Acknowledgments

- Windows API documentation
- Pomodoro Technique methodology
- Console UI design patterns

---

## 📞 Support

For issues, questions, or contributions, please create an issue in the repository.

---

**Happy Focusing! 🎯**
