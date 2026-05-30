@echo off
echo Building Focus ^& Productivity App...
echo.

REM Check if g++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: g++ compiler not found!
    echo Please install MinGW-w64 or MSYS2 to get g++ on Windows
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build

REM Compile the application
echo Compiling source files...
g++ -o build\FocusApp.exe ui.cpp timer.cpp tasks.cpp stats.cpp blocker.cpp main.cpp -static-libgcc -static-libstdc++

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo Executable created at: build\FocusApp.exe
    echo.
    echo To run the application, execute:
    echo   build\FocusApp.exe
) else (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

pause
