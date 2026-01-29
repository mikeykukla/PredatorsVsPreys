#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/System/Clock.hpp>
#include "Entity.h"
#include "Statistics.h" 

// Glowna klasa aplikacji - zarzadza cala symulacja
// Rendering, aktualizacja stanu, zapis i odczyt statystyk
class Application
{
public:
	// Konstruktor - inicjalizacja okna, encji, pixeli i konfiguracje
	Application();

	// Glowna petla aplikacji
	void run();

	//  Zapis statystyk symulacji do pliku JSON
	void saveState(const std::string& fileName);

	// Odczytuje i wyswietla statystki z pliku JSON po zakonczeniu symulacji
	void readAndDisplayStats(const std::string& fileName);

private:
	int WIDTH = 700; //default WIDTH
	int HEIGHT = 700; //default HEIGHT

	// Konwertuje wsporzedne 2D (x, y) na indeks 1D w wektorze
	int getIndex(int x, int y) const;

	// Obsluguje zdarzenia okna
	void pollEvents();

	// Aktualizuje stan wszystkich encji w kazdej klatce
	void update();

	// Aktualizuje zachowanie drapieznika (polowanie, ruch, smierc)
	void updatePredator(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity);

	// Aktualizuje zachowanie ofiary (rozmnazanie, ruch)
	void updatePrey(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity);

	// Wczytuje konfiguracje (szerokosc i wysokosc okna) 
	void loadConfig(const std::string& fileName);

	std::unique_ptr<sf::RenderWindow> m_window; // Wskaźnik do okna aplikacji SFML
	std::vector<sf::Vertex> m_pixels; // Pixele do renderowania (pozycja, kolor)
	std::vector<std::unique_ptr<Entity>> m_entities; // Dynamiczna tablica wskaźników do encji (typ, zdrowie, kolor, pozycja)

	int m_preyCount = 0; // Aktualna liczba ofiar
	int m_predatorCount = 0; // Aktualna liczba drapieznikow


	std::unique_ptr<sf::Font> m_font;
	sf::Text m_preyCountText; // Tekst wyswietlajacy liczbe ofiar
	sf::Text m_predatorCountText; // Tekst wyswietlajacy liczbe drapieznikow

	Statistics<int> m_reproducedPreyStats; // Statystyki rozmnozonych ofiar
	Statistics<int> m_eatenPreyStats; // Statystyki zjedzonych ofiar

	sf::Clock m_saveClock;
};