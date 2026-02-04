#include <SFML/Graphics.hpp>

#include "Application.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() 
{
    const std::string reportFile = "report.json";
    Application app; 

    try {
        app.run(); 
        app.readAndDisplayStats(reportFile); 
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}