# PROJECT ARCHITECTURE
## Focus & Productivity App

### 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        MAIN.CPP                              │
│                   (Application Entry)                        │
│                                                              │
│  - FocusApp Class (Main Application Controller)            │
│  - Screen Management                                        │
│  - Input Handling                                           │
│  - Application Loop                                         │
└────────────┬────────────────────────────────┬───────────────┘
             │                                │
             ▼                                ▼
┌─────────────────────────┐      ┌─────────────────────────┐
│       UI MODULE         │      │    BUSINESS LOGIC       │
│      (ui.h/cpp)         │      │                         │
├─────────────────────────┤      ├─────────────────────────┤
│ - Console Setup         │      │ TIMER (timer.h/cpp)     │
│ - Cursor Positioning    │      │ - Start/Stop/Pause      │
│ - Colors & Styling      │      │ - Time Calculation      │
│ - Draw Boxes/Lines      │      │ - Progress Tracking     │
│ - Draw Progress Bars    │      │                         │
│ - Layout Components     │      ├─────────────────────────┤
│   * Header              │      │ TASKS (tasks.h/cpp)     │
│   * Sidebar             │      │ - Add/Delete Tasks      │
│   * Footer              │      │ - Complete Tasks        │
│ - Text Rendering        │      │ - Task List Management  │
└─────────────────────────┘      │                         │
                                 ├─────────────────────────┤
                                 │ STATS (stats.h/cpp)     │
                                 │ - Session Tracking      │
                                 │ - Time Calculation      │
                                 │ - Weekly Analytics      │
                                 │ - Data Aggregation      │
                                 └───────────┬─────────────┘
                                             │
                                             ▼
                                 ┌─────────────────────────┐
                                 │   DATA PERSISTENCE      │
                                 │                         │
                                 │ tasks.dat (File I/O)    │
                                 │ stats.dat (File I/O)    │
                                 └─────────────────────────┘
```

---

### 📦 Module Breakdown

#### **1. UI Module** (`ui.h` / `ui.cpp`)
**Responsibility:** All visual rendering and console manipulation

**Key Functions:**
- `InitConsole()` - Set up console window
- `DrawBox()` - Render bordered boxes
- `DrawProgressBar()` - Visual progress indicators
- `DrawHeader/Sidebar/Footer()` - Layout components
- `SetColor()` - Text color management
- `SetCursorPosition()` - Console positioning

**Windows API Used:**
- `SetConsoleCursorPosition`
- `SetConsoleTextAttribute`
- `SetConsoleWindowInfo`
- `GetStdHandle`

---

#### **2. Timer Module** (`timer.h` / `timer.cpp`)
**Responsibility:** Focus session timing logic

**Key Functions:**
- `Start(minutes)` - Begin countdown
- `Pause()` / `Resume()` - Pause control
- `Update()` - Calculate remaining time
- `GetProgress()` - Progress percentage
- `GetFormattedTime()` - Display format

**Features:**
- Uses `std::chrono` for precise timing
- Handles pause duration tracking
- Progress calculation
- Completion detection

---

#### **3. Task Manager** (`tasks.h` / `tasks.cpp`)
**Responsibility:** Task management and persistence

**Key Functions:**
- `AddTask(description)` - Create new task
- `CompleteTask(id)` - Mark as done
- `DeleteTask(id)` - Remove task
- `SaveToFile()` / `LoadFromFile()` - Persistence

**Data Structure:**
```cpp
struct Task {
    int id;
    string description;
    bool completed;
    string dateAdded;
}
```

---

#### **4. Statistics Module** (`stats.h` / `stats.cpp`)
**Responsibility:** Session tracking and analytics

**Key Functions:**
- `AddSession(duration)` - Record session
- `RecalculateStats()` - Update totals
- `GetWeeklyData()` - Chart data
- `SaveToFile()` / `LoadFromFile()` - Persistence

**Analytics:**
- Total focus time
- Daily sessions
- Weekly breakdown
- Session history

---

#### **5. Main Application** (`main.cpp`)
**Responsibility:** Application orchestration

**Key Classes:**
- `FocusApp` - Main controller

**Key Functions:**
- `Initialize()` - Setup and load data
- `Run()` - Main application loop
- `DrawCurrentScreen()` - Screen router
- `HandleInput()` - Input dispatcher
- `Shutdown()` - Cleanup and save

**Input Modes:**
- `MODE_NAVIGATION` - Menu navigation
- `MODE_TIMER_SELECT` - Timer selection
- `MODE_TIMER_RUNNING` - Active timer
- `MODE_TASK_ADD` - Adding task
- `MODE_TASK_MANAGE` - Managing tasks

---

### 🔄 Data Flow

#### **Starting a Focus Session:**
```
User Input (Enter) 
    ↓
HandleInput() 
    ↓
Timer.Start(duration)
    ↓
[Loop] Timer.Update() + DrawTimerScreen()
    ↓
Timer.IsCompleted() 
    ↓
Stats.AddSession(duration)
    ↓
Stats.SaveToFile()
```

#### **Adding a Task:**
```
User Input ('A')
    ↓
inputMode = MODE_TASK_ADD
    ↓
HandleTaskAddInput() [collect text]
    ↓
TaskManager.AddTask(description)
    ↓
TaskManager.SaveToFile()
```

#### **Viewing Statistics:**
```
Navigate to Stats Screen
    ↓
DrawStatsScreen()
    ↓
Stats.GetTotalMinutes()
Stats.GetWeeklyData()
    ↓
Draw charts and numbers
```

---

### 🎯 Screen Management

```
┌─────────────────┐
│   Navigation    │ ← Default Mode
└────────┬────────┘
         │
    ┌────┴─────┬──────────┬──────────┬──────────┐
    ▼          ▼          ▼          ▼          ▼
┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐
│  Home  │ │ Timer  │ │ Tasks  │ │ Stats  │ │ Block  │
└────────┘ └────────┘ └────────┘ └────────┘ └────────┘
              │          │
              │          │
         ┌────┴────┐ ┌───┴─────┐
         ▼         ▼ ▼         ▼
    ┌─────────┐ ┌─────────┐ ┌─────────┐
    │ Select  │ │ Running │ │ Add/    │
    │ Duration│ │  Timer  │ │ Manage  │
    └─────────┘ └─────────┘ └─────────┘
```

---

### 🛠️ Windows API Integration

#### **Console Setup:**
- `GetStdHandle(STD_OUTPUT_HANDLE)` - Get console handle
- `SetConsoleWindowInfo()` - Set window size
- `SetConsoleScreenBufferSize()` - Set buffer size
- `SetConsoleTitleA()` - Set window title

#### **Rendering:**
- `SetConsoleCursorPosition()` - Move cursor
- `SetConsoleTextAttribute()` - Change colors
- `FillConsoleOutputCharacter()` - Clear screen

#### **Input:**
- `GetAsyncKeyState()` - Check key press
- Non-blocking input polling

#### **Cursor:**
- `SetConsoleCursorInfo()` - Show/hide cursor

---

### 📊 File Format

#### **tasks.dat:**
```
<nextId>
<id>|<description>|<completed>|<date>
<id>|<description>|<completed>|<date>
...
```

#### **stats.dat:**
```
<date>|<durationMinutes>|<timestamp>
<date>|<durationMinutes>|<timestamp>
...
```

---

### 🔧 Key Design Patterns

1. **Separation of Concerns**
   - UI separate from logic
   - Data persistence isolated
   - Clear module boundaries

2. **State Machine**
   - InputMode enum for states
   - Screen enum for navigation
   - State-based input handling

3. **Data Persistence**
   - Save on modification
   - Load on startup
   - File-based storage

4. **Event Loop**
   - Main application loop
   - Input polling
   - Screen redrawing
   - Timer updates

---

### 🚀 Extension Points

**Easy to Add:**
- New screens (add to Screen enum)
- New timer presets (modify timer selection)
- Additional statistics (extend Stats class)
- Custom colors (modify color constants)
- New task fields (extend Task struct)

**Would Require More Work:**
- Real app blocking (requires admin rights + system hooks)
- Network sync (needs networking layer)
- Multiple users (needs user management system)
- Sound effects (needs audio library)

---

### 📝 Code Quality Features

✅ **Modular Architecture** - Clear separation of concerns  
✅ **Type Safety** - Enums for states and screens  
✅ **Error Handling** - File I/O checks  
✅ **Resource Management** - Proper cleanup in destructors  
✅ **Commented Code** - Clear documentation  
✅ **Consistent Style** - Following C++ conventions  

---

**This architecture provides a solid foundation for a professional console application!**
