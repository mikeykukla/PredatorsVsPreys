#include <SFML/Graphics.hpp>

#include "Application.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() 
{
    const std::string reportFile = "report.json";
    Application app; // Tworzenie obiektu aplikacji

    try {
        app.run(); // Glowna petla symulacji
        app.readAndDisplayStats(reportFile); // Po zakonczeniu symulacji odczytaj i wyswietl raport
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}