@echo off
tree /f /a > tree.txt

rem Wyczyść output
cls

echo "What wanna you do:"
echo "1 or b - Build engine"
echo "2 or t - Build testbed"
echo "3 or d - Debug testbed"
set /p userInput=Enter command (Build/Testbed/Debug): 

if /i "%userInput%"=="build" (
    goto build
) else if /i "%userInput%"=="b" (
    goto build
) else if /i "%userInput%"=="1" (
    goto build
) else if /i "%userInput%"=="testbed" (
    goto testbed
) else if /i "%userInput%"=="t" (
    goto testbed
) else if /i "%userInput%"=="2" (
    goto testbed
) else if /i "%userInput%"=="debug" (
    goto debug
) else if /i "%userInput%"=="d" (
    goto debug
) else if /i "%userInput%"=="3" (
    goto debug
) else (
    echo Invalid value exiting...
    goto end
)

:build
cd GONTI-SDK/_Build/_Scripts/win32/
start main.bat
goto end

:debug
cd Tests/BuildScripts/win/
call Debug.bat
goto end

:testbed
cd Tests/BuildScripts/win/
call makeTestbed.bat
goto end

:end