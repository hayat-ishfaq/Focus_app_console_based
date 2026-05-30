# Focus & Productivity App - Build Script
# Run this script to compile the application

Write-Host "Building Focus & Productivity App..." -ForegroundColor Cyan

# Check if g++ is available
try {
    $gccVersion = g++ --version 2>&1
    Write-Host "Compiler found: g++" -ForegroundColor Green
} catch {
    Write-Host "Error: g++ compiler not found!" -ForegroundColor Red
    Write-Host "Please install MinGW-w64 or MSYS2 to get g++ on Windows" -ForegroundColor Yellow
    exit 1
}

# Create output directory
if (-Not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
    Write-Host "Created build directory" -ForegroundColor Green
}

# Compile the application
Write-Host "Compiling source files..." -ForegroundColor Yellow

$sourceFiles = @(
    "ui.cpp",
    "timer.cpp",
    "tasks.cpp",
    "stats.cpp",
    "blocker.cpp",
    "main.cpp"
)

$outputFile = "build\FocusApp.exe"
$compileCommand = "g++ -o $outputFile " + ($sourceFiles -join " ") + " -static-libgcc -static-libstdc++"

Write-Host "Running: $compileCommand" -ForegroundColor Gray

try {
    Invoke-Expression $compileCommand
    
    if (Test-Path $outputFile) {
        Write-Host "`nBuild successful!" -ForegroundColor Green
        Write-Host "Executable created at: $outputFile" -ForegroundColor Cyan
        Write-Host "`nTo run the application, execute:" -ForegroundColor Yellow
        Write-Host "  .\build\FocusApp.exe" -ForegroundColor White
    } else {
        Write-Host "`nBuild failed - executable not created" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "`nCompilation error!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    exit 1
}
