@echo off
setlocal enabledelayedexpansion

echo ======================================
echo    TESTY FUNKCJONALNE - PredatorsVsPreys
echo ======================================
echo.

:: Zdefiniuj œcie¿ki do narzêdzi systemowych
set "PING_CMD=%SystemRoot%\System32\ping.exe"
set "TASKKILL_CMD=%SystemRoot%\System32\taskkill.exe"
set "FINDSTR_CMD=%SystemRoot%\System32\findstr.exe"

:: Zdefiniuj œcie¿ki do programu - DIST folder
set "PROGRAM_DIST=..\dist\PredatorsVsPreys.exe"
set "WORK_DIR_DIST=..\dist"

:: SprawdŸ czy dist istnieje
if exist "%PROGRAM_DIST%" (
    set "PROGRAM=%PROGRAM_DIST%"
    set "WORK_DIR=%WORK_DIR_DIST%"
    echo Uzywam wersji z folderu dist/
) else (
    echo BLAD: Nie znaleziono programu w dist/!
    echo Zbuduj projekt w trybie Release.
    pause
    exit /b 1
)

echo Program: !PROGRAM!
echo Working dir: !WORK_DIR!
echo.

set TESTS_PASSED=0
set TESTS_FAILED=0

:: ===== TEST 1 =====
echo ======================================
echo [TEST 1] Konfiguracja podstawowa (300x300)
echo ======================================

if not exist "test_config.json" (
    echo [FAIL] Brak pliku test_config.json
    set /a TESTS_FAILED+=1
    goto :TEST2
)

copy /y test_config.json "!WORK_DIR!\config.json" > nul

echo Uruchamiam program...
cd /d "!WORK_DIR!"
start "" "PredatorsVsPreys.exe"
cd /d "%~dp0"

echo Czekam 10 sekund...
"%PING_CMD%" 127.0.0.1 -n 11 > nul 2>&1
if !errorlevel! neq 0 (
    waitfor /t 10 PauseTest 2>nul
)

echo Zamykam program...
"%TASKKILL_CMD%" /im PredatorsVsPreys.exe /f > nul 2>&1
"%PING_CMD%" 127.0.0.1 -n 2 > nul 2>&1

if exist "!WORK_DIR!\report.json" (
    echo [OK] Plik report.json utworzony
    
    "%FINDSTR_CMD%" /c:"Preys reproduced" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys reproduced'
        set /a TESTS_PASSED+=1
    ) else (
        echo [FAIL] Brak 'Preys reproduced'
        set /a TESTS_FAILED+=1
    )
    
    "%FINDSTR_CMD%" /c:"Preys eaten" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys eaten'
    ) else (
        echo [FAIL] Brak 'Preys eaten'
    )
    
    echo.
    echo Zawartosc report.json:
    type "!WORK_DIR!\report.json"
    echo.
) else (
    echo [FAIL] Brak pliku report.json
    set /a TESTS_FAILED+=1
)

:: ===== TEST 2 =====
:TEST2
echo.
echo ======================================
echo [TEST 2] Normalna symulacja (700x700)
echo ======================================

if not exist "test_normal.json" (
    echo [FAIL] Brak pliku test_normal.json
    set /a TESTS_FAILED+=1
    goto :TEST3
)

copy /y test_normal.json "!WORK_DIR!\config.json" > nul

echo Uruchamiam program...
cd /d "!WORK_DIR!"
start "" "PredatorsVsPreys.exe"
cd /d "%~dp0"

echo Czekam 10 sekund...
"%PING_CMD%" 127.0.0.1 -n 11 > nul 2>&1
if !errorlevel! neq 0 (
    waitfor /t 10 PauseTest 2>nul
)

echo Zamykam program...
"%TASKKILL_CMD%" /im PredatorsVsPreys.exe /f > nul 2>&1
"%PING_CMD%" 127.0.0.1 -n 2 > nul 2>&1

if exist "!WORK_DIR!\report.json" (
    echo [OK] Plik report.json utworzony
    
    "%FINDSTR_CMD%" /c:"Preys reproduced" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys reproduced'
        set /a TESTS_PASSED+=1
    ) else (
        echo [FAIL] Brak 'Preys reproduced'
        set /a TESTS_FAILED+=1
    )
    
    "%FINDSTR_CMD%" /c:"Preys eaten" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys eaten'
    ) else (
        echo [FAIL] Brak 'Preys eaten'
    )
    
    echo.
    echo Zawartosc report.json:
    type "!WORK_DIR!\report.json"
    echo.
) else (
    echo [FAIL] Brak pliku report.json
    set /a TESTS_FAILED+=1
)

:: ===== TEST 3 =====
:TEST3
echo.
echo ======================================
echo [TEST 3] Duza symulacja (1000x1000)
echo ======================================

if not exist "test_large.json" (
    echo [FAIL] Brak pliku test_large.json
    set /a TESTS_FAILED+=1
    goto :SUMMARY
)

copy /y test_large.json "!WORK_DIR!\config.json" > nul

echo Uruchamiam program...
cd /d "!WORK_DIR!"
start "" "PredatorsVsPreys.exe"
cd /d "%~dp0"

echo Czekam 10 sekund...
"%PING_CMD%" 127.0.0.1 -n 11 > nul 2>&1
if !errorlevel! neq 0 (
    waitfor /t 10 PauseTest 2>nul
)

echo Zamykam program...
"%TASKKILL_CMD%" /im PredatorsVsPreys.exe /f > nul 2>&1
"%PING_CMD%" 127.0.0.1 -n 2 > nul 2>&1

if exist "!WORK_DIR!\report.json" (
    echo [OK] Plik report.json utworzony
    
    "%FINDSTR_CMD%" /c:"Preys reproduced" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys reproduced'
        set /a TESTS_PASSED+=1
    ) else (
        echo [FAIL] Brak 'Preys reproduced'
        set /a TESTS_FAILED+=1
    )
    
    "%FINDSTR_CMD%" /c:"Preys eaten" "!WORK_DIR!\report.json" > nul 2>&1
    if !errorlevel! equ 0 (
        echo [OK] Znaleziono 'Preys eaten'
    ) else (
        echo [FAIL] Brak 'Preys eaten'
    )
    
    echo.
    echo Zawartosc report.json:
    type "!WORK_DIR!\report.json"
    echo.
) else (
    echo [FAIL] Brak pliku report.json
    set /a TESTS_FAILED+=1
)

:: ===== PODSUMOWANIE =====
:SUMMARY
echo.
echo ======================================
echo          PODSUMOWANIE TESTOW
echo ======================================
echo Zaliczone: %TESTS_PASSED%
echo Niezaliczone: %TESTS_FAILED%
echo ======================================
echo.

if %TESTS_FAILED% equ 0 (
    echo WSZYSTKIE TESTY ZALICZONE!
) else (
    echo NIEKTORE TESTY NIE POWIODLY SIE!
)

echo.
pause
exit /b %TESTS_FAILED%