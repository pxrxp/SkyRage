@echo off
setlocal

echo ==================================
echo     SKY RAGE - WINDOWS BUILDER      
echo ==================================

:: Check if CMake is in PATH
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake is not installed or not in your PATH.
    echo Please install CMake from https://cmake.org/download/
    exit /b 1
)

echo [1/3] Creating build directory...
if not exist "build" mkdir build
cd build

echo [2/3] Configuring CMake...
:: Check if MSVC is available
cl /? >nul 2>&1
if %errorlevel% equ 0 (
    echo Detected MSVC environment.
    cmake -Wno-dev .. -A x64
) else (
    :: Check if MinGW is available
    gcc --version >nul 2>&1
    if %errorlevel% equ 0 (
        echo Detected MinGW environment.
        cmake -Wno-dev .. -G "MinGW Makefiles"
    ) else (
        echo [ERROR] No recognized C++ compiler found (MSVC or MinGW).
        echo Please run this from a Developer Command Prompt or ensure GCC/MinGW is in your PATH.
        exit /b 1
    )
)

echo [3/3] Compiling Project...
cmake --build . --config Release --parallel

echo ==================================
echo  BUILD SUCCESSFUL!                
echo  You can launch the game via:     
echo  .\build\bin\Release\SkyRage.exe (MSVC)
echo    OR                              
echo  .\build\bin\SkyRage.exe        (MinGW)
echo ==================================
endlocal
pause
