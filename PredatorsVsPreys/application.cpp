#include "Application.h"
#include "json.hpp"
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// Konstruktor - inicjalizuje okno, wczytuje konfiguracje, tworzy encje i ustawia interfejs uzytkownika
Application::Application()
{
	// Wczytywanie szerokosci i wysokosci okna z pliku 
	loadConfig("config.json");

	// Tworzenie okna symulacji o rozmiarach WIDTH x HEIGHT
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(WIDTH, HEIGHT), "Predators and Preys");

	// Dopasowanie wektorow na podstawie wczytanych wartosci WIDTH i HEIGHT
	// Dwa wektory dla rozdzielenia logiki i grafiki
	m_pixels.resize(WIDTH * HEIGHT); // Wektor pixeli dla renderowania (Rendering. Przechowuje pozycje na ekranie i kolor)
	m_entities.resize(WIDTH * HEIGHT); // Wektor encji dla symulacji (Logika. Przechowuje typ, zdrowie, kolor, ai)

	m_window->setFramerateLimit(60);

	// Kazdy element wektora m_entites otrzymuje nowy obiekt Entity, tworzy pustą plansze,
	std::generate(m_entities.begin(), m_entities.end(),
		[]() { return std::make_unique<Entity>(); });
	// for_each zadzialaloby ale tylko jesli wszytkie pola wektora zostaly juz wczesniej zainicjowane. Resize zainicjowal te pola jednak generate jest bezpieczniejszym wyborem,
	// gdyz sluzy do wypelniania pustych lub czesciowo wypelnionych wektorow 

	// Kazdy piksel otrzymuje wspolrzedne odpowiadajace jego pozycji
	/*auto pixelIt = m_pixels.begin(); 
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (pixelIt == m_pixels.end()) break;
			pixelIt->position = { (float)x, (float)y }; 
			++pixelIt;
		}
	}*/

	// Kazdy piksel otrzymuje wspolrzedne odpowiadajace jego pozycji
	int x = 0, y = 0;
	std::for_each(m_pixels.begin(), m_pixels.end(), [&](auto& pixel) {
		pixel.position = { (float)x, (float)y };
		++x;
		if (x == WIDTH) 
		{ 
			x = 0;
			++y; 
		}
		});

	// Kolor kazdego pixela jest ustawiany na podstawie typu i zdrowia encji (Kopiowanie koloru z encji do pixeli)
	/*auto entityIt = m_entities.begin();
	for (auto& pixel : m_pixels) {
		if (entityIt == m_entities.end()) break;
		pixel.color = (*entityIt)->getColour();
		++entityIt;
	}*/

	// Kolor kazdego pixela jest ustawiany na podstawie typu i zdrowia encji (Kopiowanie koloru z encji do pixeli)
	auto entityIt = m_entities.begin();
	std::for_each(m_pixels.begin(), m_pixels.end(), [&](auto& pixel)
		{
			if (entityIt == m_entities.end()) return;
			pixel.color = (*entityIt)->getColour();
			++entityIt;
		});



	// Wczytanie czcionki
	m_font = std::make_unique<sf::Font>();
	if (!m_font->loadFromFile("font/ARIAL.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	// Ustawienia tekstu wyswietlajacego liczbe drapieznikow i ofiar
	m_predatorCountText.setFont(*m_font);
	m_preyCountText.setFont(*m_font);
	m_predatorCountText.setCharacterSize(15);
	m_preyCountText.setCharacterSize(15);
	m_preyCountText.move(0, 20);
}

// Glowna petla aplikacji, czysci ekran, aktualizuje stan, renderuje i obsluguje zdarzenia
void Application::run()
{
	while (m_window->isOpen())
	{
		m_window->clear();
		update();
		m_window->draw(m_pixels.data(), m_pixels.size(), sf::Points);
		m_window->draw(m_predatorCountText);
		m_window->draw(m_preyCountText);
		m_window->display();
		if (m_saveClock.getElapsedTime().asSeconds() >= 1.0f)
		{
			saveState("report.json");
			m_saveClock.restart();
		}
		pollEvents();
	}
}

// Konwertuje wspolrzedne 2D (x, y) na indeks 1D w wektorze
int Application::getIndex(int x, int y) const
{
	return y * WIDTH + x;
}

// Obsluga zdarzen okna
void Application::pollEvents()
{
	sf::Event e;
	while (m_window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			saveState("report.json");
			std::cout << "Statistics have been saved to the report.json" << std::endl;
			m_window->close();
		}
	}
}

// Aktualizuje stan symulacji w kazdej klatce
// Iteracja przez wszytstkie encje, poruszanie, rozmnazanie i polowanie
void Application::update()
{
	static std::minstd_rand randDevice(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<int> dist(-1, 1); // Losowe przesunięcia: -1, 0, 1

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			auto index = getIndex(x, y);
			auto& thisEntity = m_entities[index];
			auto thisType = thisEntity->getType();

			if (thisType == EntityType::Nothing)
				continue; // Puste pole - nic do zrobienia

			thisEntity->update(); // Aktualizuj stan zdrowia encji

			// Losuj sasiednie pole (x +/- 1, y +/- 1)
			int xChange = dist(randDevice);
			int yChange = dist(randDevice);

			// Oblicz koordynaty sasiada
			int xAdj = x + xChange;
			int yAdj = y + yChange;

			// Sprawdz, czy sasiad jest w granicach planszy
			if (xAdj < 0 || xAdj >= WIDTH || yAdj < 0 || yAdj >= HEIGHT)
				continue;

			auto adjIndex = getIndex(xAdj, yAdj);
			auto& otherEntity = m_entities[adjIndex];

			// Wywolaj odpowiednia funkcje w zależnosci od typu stworzenia
			switch (thisType)
			{
			case EntityType::Predator:
				updatePredator(thisEntity, otherEntity);
				break;

			case EntityType::Prey:
				updatePrey(thisEntity, otherEntity);
				break;

			default:
				break;
			}
		}
	}

	// Aktualizacja kolorow wszystkich pikseli
	// Synchornizacja kolorw pixeli z aktualnym stanem encji
	// mutble umożliwia lambdzie na modyfikacje przechwyconych zmiennych
	std::for_each(m_entities.begin(), m_entities.end(),
		[this, i = 0](const auto& entity) mutable 
		{ 
			m_pixels[i].color = entity->getColour();
			++i;
		});

	// Zlicza zwyklych drapienzikow (zdrowie > 0)
	m_predatorCount = static_cast<int>(std::count_if(m_entities.begin(), m_entities.end(),
		[](const auto& entity) {
			return entity && entity->getType() == EntityType::Predator &&
				entity->getHealth() > 0;
		}));

	// Zlicza wszystkie ofiary 
	m_preyCount = static_cast<int>(std::count_if(m_entities.begin(), m_entities.end(),
		[](const auto& entity) {
			return entity && entity->getType() == EntityType::Prey;
		}));


	// Aktualizuj licznik
	m_predatorCountText.setString("Predator: " + std::to_string(m_predatorCount));
	m_preyCountText.setString("Prey: " + std::to_string(m_preyCount));
}

// Aktualizuje zachowanie drapieznika
// Drapieznik zjada ofiare (zamienia ja w drapieznika) porusza sie po pustych polach
void Application::updatePredator(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity)
{
	if (thisEntity->getHealth() <= 0)
	{
		// Smierc drapieznika
		--m_predatorCount;
		thisEntity->setType(EntityType::Nothing);
		return;
	}

	auto otherType = otherEntity->getType();

	switch (otherType)
	{
	case EntityType::Prey:
		// --m_preyCount;
		// ++m_predatorCount;
		m_eatenPreyStats.add(1); // Dodanie do statystyk zjedzonych ofiar
		otherEntity->setType(EntityType::Predator); // Ofiara zmienia sie w drapieznika
		thisEntity->heal(otherEntity->getHealth()); // Leczenie drapieznika
		break;

	case EntityType::Nothing:
		thisEntity->move(*otherEntity); // Drapieznik przesuwa sie na puste pole
		break;

	default:
		break;
	}
}

// Aktualizuje zachowanie ofiar
// Ofiara moze sie rozmnazac (jesli ma pelne zdrowie) porusza sie po pustych polach
void Application::updatePrey(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity)
{
	auto otherType = otherEntity->getType();
	bool canReproduce = thisEntity->getHealth() >= Entity::MAX_HEALTH; // Warunek rozmnazania


	switch (otherType)
	{
	case EntityType::Nothing:
		if (canReproduce)
		{
			thisEntity->reproduce(*otherEntity); // Ofiara sie rozmnaza na pustym polu
			// ++m_preyCount;
			m_reproducedPreyStats.add(1); // Dodaj do statystyk rozmnozonych ofiar
			thisEntity->setHealth(10); // Zresetuj zdrowie rodzica koszt na rozmnazanie
		}
		else
		{
			thisEntity->move(*otherEntity); // Ofiara przesuwa sie na puste pole
		}
		break;

	default:
		break;
	}
}

// Wczytywanie konfiguracji z pliku JSON
// Pozwala na ustawienie szerokosci i wysokosci okna 
void Application::loadConfig(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Cannot open file " << fileName << std::endl;
		return;
	}

	nlohmann::json config;
	try {
		file >> config;
		file.close();
		WIDTH = config.value("WIDTH", WIDTH);
		HEIGHT = config.value("HEIGHT", HEIGHT);
	}
	catch (const std::exception& e) {
		std::cerr << "Error reading configuration file " << e.what() << std::endl;
	}
}

// Zapisuje statystyki symulacji do pliku JSON
// Zapis liczby rozmnozonych i zjedzonych ofiar
void Application::saveState(const std::string& fileName) {
	std::ofstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Cannot open file: " << fileName << std::endl;
		return;
	}


	file << "{\n";
	file << "    \"Preys reproduced\": " << m_reproducedPreyStats.sum() << ",\n";
	file << "    \"Preys eaten\": " << m_eatenPreyStats.sum() << "\n";
	file << "}\n";

	file.close();
}

void Application::readAndDisplayStats(const std::string& fileName) 
{
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Cannot open file: " << fileName << std::endl;
		return;
	}

	nlohmann::json stats;
	try {
		file >> stats;
		file.close();


		std::cout << "Report:\n";
		std::cout << "Preys reproduced: " << stats["Preys reproduced"] << "\n";
		std::cout << "Preys eaten: " << stats["Preys eaten"] << "\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Operation reading failed: " << e.what() << std::endl;
	}
}

