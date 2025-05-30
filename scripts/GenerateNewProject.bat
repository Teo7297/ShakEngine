@echo off

REM Ask user for project name
set /p projectName="Enter project name: "
echo Creating project: %projectName%

REM Create temp VBScript to show folder selection dialog
echo Creating folder selection dialog...
echo Set objShell = CreateObject("Shell.Application") > "%temp%\folderdialog.vbs"
echo Set objFolder = objShell.BrowseForFolder(0, "Select location to create the project:", 0, 0) >> "%temp%\folderdialog.vbs"
echo If objFolder Is Nothing Then >> "%temp%\folderdialog.vbs"
echo     WScript.Echo "CANCELLED" >> "%temp%\folderdialog.vbs"
echo Else >> "%temp%\folderdialog.vbs"
echo     WScript.Echo objFolder.Self.Path >> "%temp%\folderdialog.vbs"
echo End If >> "%temp%\folderdialog.vbs"

REM Run the script and get the selected folder
for /f "delims=" %%a in ('cscript //nologo "%temp%\folderdialog.vbs"') do set "selectedFolder=%%a"

REM Delete the temporary VBScript
del "%temp%\folderdialog.vbs"

REM Check if the user cancelled the dialog
if "%selectedFolder%"=="CANCELLED" (
    echo Project creation cancelled by user.
    exit /b 1
)

REM Generate project folder
set projectPath=%selectedFolder%\%projectName%
echo Creating project at: %projectPath%

mkdir "%projectPath%"
if %errorlevel% neq 0 (
    echo Failed to create project directory
    exit /b %errorlevel%
)

REM Create required directories
mkdir "%projectPath%\.vscode"
mkdir "%projectPath%\src"

REM Copy settings.json and cpp.code-snippets files from parent project
echo Copying .vscode configuration files...
copy "..\.vscode\settings.json" "%projectPath%\.vscode\settings.json" > nul
if %errorlevel% neq 0 (
    echo Warning: Failed to copy settings.json
)
copy "..\.vscode\cpp.code-snippets" "%projectPath%\.vscode\cpp.code-snippets" > nul
if %errorlevel% neq 0 (
    echo Warning: Failed to copy cpp.code-snippets
)

REM Generate launch.json
echo Generating .vscode/launch.json...
(
echo {
echo     "configurations": [
echo         {
echo             "name": "Debug",
echo             "type": "cppvsdbg",
echo             "request": "launch",
echo             "program": "${workspaceFolder}/build/Debug/%projectName%.exe",
echo             "args": [],
echo             "stopAtEntry": false,
echo             "cwd": "${workspaceFolder}",
echo             "environment": [],
echo             "console": "integratedTerminal",
echo             "preLaunchTask": "Build Debug"
echo         },
echo         {
echo             "name": "RelWithDebInfo",
echo             "type": "cppvsdbg",
echo             "request": "launch",
echo             "program": "${workspaceFolder}/build/RelWithDebInfo/%projectName%.exe",
echo             "args": [],
echo             "stopAtEntry": false,
echo             "cwd": "${workspaceFolder}",
echo             "environment": [],
echo             "console": "integratedTerminal",
echo             "preLaunchTask": "Build RelWithDebInfo"
echo         },
echo         {
echo             "name": "Release",
echo             "type": "cppvsdbg",
echo             "request": "launch",
echo             "program": "${workspaceFolder}/build/Release/%projectName%.exe",
echo             "args": [],
echo             "stopAtEntry": false,
echo             "cwd": "${workspaceFolder}",
echo             "environment": [],
echo             "console": "integratedTerminal",
echo             "preLaunchTask": "Build Release"
echo         }
echo     ]
echo }
) > "%projectPath%\.vscode\launch.json"

REM Generate tasks.json
echo Generating .vscode/tasks.json...
(
echo {
echo     "version": "2.0.0",
echo     "tasks": [
echo         {
echo             "label": "Build Release",
echo             "type": "shell",
echo             "command": "cmd.exe",
echo             "args": [
echo                 "/c",
echo                 "call \\\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat\\\" x64 && cmake --build \"%projectPath%/build\" --config Release --target all -j 14 --"
echo             ],
echo             "group": "build",
echo             "problemMatcher": []
echo         },
echo         {
echo             "label": "Build RelWithDebInfo",
echo             "type": "shell",
echo             "command": "cmd.exe",
echo             "args": [
echo                 "/c",
echo                 "call \\\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat\\\" x64 && cmake --build \"%projectPath%/build\" --config RelWithDebInfo --target all -j 14 --"
echo             ],
echo             "group": "build",
echo             "problemMatcher": []
echo         },
echo         {
echo             "label": "Build Debug",
echo             "type": "shell",
echo             "command": "cmd.exe",
echo             "args": [
echo                 "/c",
echo                 "call \\\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat\\\" x64 && cmake --build \"%projectPath%/build\" --config Debug --target all -j 14 --"
echo             ],
echo             "group": "build",
echo             "problemMatcher": []
echo         },
echo         {
echo             "label": "Clean Rebuild Debug",
echo             "type": "shell",
echo             "command": "cmd.exe",
echo             "args": [
echo                 "/c",
echo                 "cmake --build \"%projectPath%/build\" --config Debug --target clean -- && call \\\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat\\\" x64 && cmake --build \"%projectPath%/build\" --config Debug --target all -j 14 --"
echo             ],
echo             "group": "build",
echo             "problemMatcher": []
echo         }
echo     ]
echo }
) > "%projectPath%\.vscode\tasks.json"

REM Generate CMakeLists.txt
echo Generating CMakeLists.txt...
(
echo cmake_minimum_required^(VERSION 3.27^)
echo project^(%projectName% LANGUAGES CXX^)
echo.
echo set^(CMAKE_CXX_STANDARD 20^)
echo set^(CMAKE_CXX_STANDARD_REQUIRED ON^)
echo set^(CMAKE_CXX_EXTENSIONS OFF^)
echo.
echo include^(FetchContent^)
echo.
echo FetchContent_Declare^(
echo   ShakEngine
echo   GIT_REPOSITORY https://github.com/Teo7297/ShakEngine.git
echo   GIT_TAG master
echo   ^)
echo FetchContent_MakeAvailable^(ShakEngine^)
echo.
echo file^(GLOB_RECURSE SOURCES src/*.cpp^)
echo.
echo add_executable^(${PROJECT_NAME} ${SOURCES}^)
echo.
echo target_include_directories^(${PROJECT_NAME} PUBLIC src^) # better use a classic include dir
echo.
echo target_link_libraries^(${PROJECT_NAME} PUBLIC ShakEngine^)
) > "%projectPath%\CMakeLists.txt"

REM Generate a sample main.cpp file
echo Generating sample source file...
(
echo #include "ShakEngine.h"
echo.
echo // Include your game scenes here
echo.
echo int main^(^)
echo {
echo     // Init engine
echo     shak::ShakEngine* engine = ^&shak::ShakEngine::GetInstance^(^);
echo     engine-^>Initialize^(^);
echo.
echo     // Setup scene
echo     auto sm = engine-^>GetSceneManager^(^);
echo     //! sm-^>AddScene^(std::make_shared^<MyScene^>^("MyScene"^)^);
echo     sm-^>QueueScene^("MyScene"^);
echo     sm-^>TryActivateQueuedScene^(^);
echo.
echo     // Start engine
echo     engine-^>Start^(^);
echo }
) > "%projectPath%\src\main.cpp"

echo Project %projectName% created successfully at %projectPath%

REM Create build directory
mkdir "%projectPath%\build"

REM Open the project in VSCode
cd /d "%projectPath%"
start "" code .

echo.
echo VS Code is opening your new project...
pause
