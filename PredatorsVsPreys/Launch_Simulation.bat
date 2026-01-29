@echo off
echo ========================================
echo  PredatorsVsPreys - Simulation Launcher
echo ========================================
echo.

REM Sprawdz, czy plik wykonywalny istnieje
if exist "dist\PredatorsVsPreys.exe" (
    echo [OK] Znaleziono plik wykonywalny
    echo [INFO] Uruchamianie symulacji...
    echo.
    
    REM Uruchom program
    cd dist
    PredatorsVsPreys.exe
    cd ..\..
    
    echo.
    echo [INFO] Symulacja zakonczona
    echo.
) else (
    echo [BLAD] Nie znaleziono pliku: dist\PredatorsVsPreys.exe
    echo.
    echo Mozliwe rozwiazania:
    echo 1. Skompiluj projekt w Visual Studio (Release ^| x64)
    echo.
)

echo.
pause