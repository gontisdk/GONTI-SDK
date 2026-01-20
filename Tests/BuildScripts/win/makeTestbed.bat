rem Make script for testbed
@echo off
SetLocal EnableDelayedExpansion

rem Przejdź do katalogu źródłowego
rem cd .. rem Wyjście z   GONTI/Source/Utilities/Build/win/   do     GONTI/Source/Utilities/Build/
rem cd .. rem Wyjście z   GONTI/Source/Utilities/Build/       do     GONTI/Source/Utilities/
rem cd .. rem Wyjście z   GONTI/Source/Utilities/             do     GONTI/Source/
rem cd .. rem Wyjście z   GONTI/Source/                       do     GONTI/

rem Wyczyść zmienną
set cFilenames=

rem Dodaj wszystkie pliki .c z Tests/
for /R ..\..\..\Tests %%f in (*.c) do (
    set "cFilenames=!cFilenames! "%%f""
)

echo(
echo ===============================================
echo === START LIST OF: Files passed to compiler ===
echo ===============================================
echo(
echo !cFilenames!
echo(
echo =============================================
echo === END LIST OF: Files passed to compiler ===
echo =============================================
echo(
echo(


if not exist "..\..\Build" (
    mkdir "..\..\Build"
)

cd ..\..\Build


rem Ścieżki do bibliotek
set vcpkgIncludeDir=%VULKAN_SDK%/Include
set vcpkgLibDir=%VULKAN_SDK%/Lib

rem ŚCIEŻKI DO PLIKÓW GONTI

rem GT Include path
set workspaceFoldersInclude=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\GONTI-SDK

rem GT LIBS files
set GONTI-CORE-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.CORE
set GONTI-RENDER-VK-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RENDER.VK
set GONTI-RENDER-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RENDER
set GONTI-RUNTIME-Libs-Dir=D:\Projects\Visual Studio Code Projects\C++\GONTI SDK\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RUNTIME

rem !! ŚCIEŻKI DO PLIKÓW GONTI

rem Kompilacja
set assembly=testbed
set compilerFlags=-g
set includeFlags=-I"%workspaceFoldersInclude%" -I"%vcpkgIncludeDir%"
set linkerFlags=-L"%GONTI-CORE-Libs-Dir%" -L"%GONTI-RENDER-Libs-Dir%" -L"%GONTI-RENDER-VK-Libs-Dir%" -L"%GONTI-RUNTIME-Libs-Dir%" -luser32 -lGONTI.CORE -lGONTI.RENDER -lGONTI.RENDER.VK -lGONTI.RUNTIME -L"%vcpkgLibDir%" -lvulkan-1
set defines=-D_DEBUG -DKIMPORT

echo Building %assembly%...

echo(
echo(

echo Copying GONTI.CORE.dll to testbed directory...
copy "..\..\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.CORE\GONTI.CORE.dll" ".\GONTI.CORE.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.CORE.dll!
) else (
    echo [SUCCESS] GONTI.CORE.dll copied successfully.
)

echo Copying GONTI.RENDER.VK.dll to testbed directory...
copy "..\..\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RENDER.VK\GONTI.RENDER.VK.dll" ".\GONTI.RENDER.VK.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.RENDER.VK.dll!
) else (
    echo [SUCCESS] GONTI.RENDER.VK.dll copied successfully.
)

echo Copying GONTI.RENDER.dll to testbed directory...
copy "..\..\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RENDER\GONTI.RENDER.dll" ".\GONTI.RENDER.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.RENDER.dll!
) else (
    echo [SUCCESS] GONTI.RENDER.dll copied successfully.
)

echo Copying GONTI.RUNTIME.dll to testbed directory...
copy "..\..\GONTI-SDK\_Build\bin\win32\GONTI\GONTI-ENGINE\GONTI.RUNTIME\GONTI.RUNTIME.dll" ".\GONTI.RUNTIME.dll" /Y > nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to copy GONTI.RUNTIME.dll!
) else (
    echo [SUCCESS] GONTI.RUNTIME.dll copied successfully.
)

echo(
echo(

echo(
echo(
echo ======================================================================
echo ===     START OF: MakeTestbed.bat Errors/Warnings/Informations     ===
echo ======================================================================
echo(
echo(
echo(

rem  "../../../Tests/main.c" = path to your main file (is in Tests/ folder so it is no needed)
clang !cFilenames! %compilerFlags% -o %assembly%.exe %defines% %includeFlags% %linkerFlags%

echo(
echo(
echo(
echo ====================================================================
echo ===     END OF: MakeTestbed.bat Errors/Warnings/Informations     ===
echo ====================================================================

rem call run.bat
call "%~dp0run.bat" %assembly%

goto end
:end