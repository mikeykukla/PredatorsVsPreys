# PredatorsVsPreys - Symulacja Drapieżnik vs Ofiara

## Opis projektu

Symulacja ekosystemu drapieżników i ofiar na siatce 2D z wizualizacją w czasie rzeczywistym przy użyciu biblioteki SFML. Projekt demonstruje użycie nowoczesnego C++17, wzorców RAII, smart pointerów, algorytmów STL oraz szablonów.

## Mechanika symulacji

- Ofiary (Preys) - rozmnażają się na pustych polach, zyskują zdrowie z czasem (kolor zielony)
- Drapieżniki (Predators) - zjadają ofiary, tracą zdrowie z czasem (kolor czerwony)
- Puste pola (Nothing) - dostępne do kolonizacji (kolor czarny)
- Każda encja ma zdrowie (0-100), wpływające na intensywność koloru

## Wymagania

- System operacyjny: Windows 10/11 (x64)
- Kompilator: Visual Studio 2022 (v143 toolset)
- Standard C++: C++17
- Biblioteki: SFML 2.5+ (graphics, window, system)

## Uruchomienie programu

### Sposób 1: Gotowy program (bez kompilacji)

1. Przejdź do folderu: `Projekt\PredatorsVsPreys`
2. Uruchom: `Launch_Simulation.bat`

### Sposób 2: Gotowy program (bez kompilacji)

1. Przejdź do folderu: `Projekt\PredatorsVsPreys\dist\`
2. Uruchom: `PredatorsVsPreys.exe`

Wszystkie wymagane pliki (DLL-e, czcionki, config) są w folderze dist/.

### Sposób 2: Kompilacja ze źródeł

1. Otwórz: `Projekt\PredatorsVsPreys\PredatorsVsPreys.sln`
2. Ustaw konfigurację: Release | x64
3. Build -> Rebuild Solution
4. Pliki zostaną automatycznie skopiowane do dist/

## Uruchomienie testów

Uruchom: `tests\run_tests.bat`

Automatycznie wykonuje 3 testy:
- Test 1: Mała siatka (300x300)
- Test 2: Normalna siatka (700x700)
- Test 3: Duża siatka (1000x1000)

## Konfiguracja

### config.json (wejście)

```json
{
    "WIDTH": 700,
    "HEIGHT": 700
}