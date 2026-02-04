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

Application::Application()
{
	loadConfig("config.json");

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(WIDTH, HEIGHT), "Predators and Preys");
	m_pixels.resize(WIDTH * HEIGHT);
	m_entities.resize(WIDTH * HEIGHT);

	m_window->setFramerateLimit(60);

	std::generate(m_entities.begin(), m_entities.end(),
		[]() { return std::make_unique<Entity>(); });

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

	auto entityIt = m_entities.begin();
	std::for_each(m_pixels.begin(), m_pixels.end(), [&](auto& pixel)
		{
			if (entityIt == m_entities.end()) return;
			pixel.color = (*entityIt)->getColour();
			++entityIt;
		});

	m_font = std::make_unique<sf::Font>();
	if (!m_font->loadFromFile("font/ARIAL.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	m_predatorCountText.setFont(*m_font);
	m_preyCountText.setFont(*m_font);
	m_predatorCountText.setCharacterSize(15);
	m_preyCountText.setCharacterSize(15);
	m_preyCountText.move(0, 20);
}

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

int Application::getIndex(int x, int y) const
{
	return y * WIDTH + x;
}

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
				continue;

			thisEntity->update(); 
			int xChange = dist(randDevice);
			int yChange = dist(randDevice);

			int xAdj = x + xChange;
			int yAdj = y + yChange;

			if (xAdj < 0 || xAdj >= WIDTH || yAdj < 0 || yAdj >= HEIGHT)
				continue;

			auto adjIndex = getIndex(xAdj, yAdj);
			auto& otherEntity = m_entities[adjIndex];

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

	std::for_each(m_entities.begin(), m_entities.end(),
		[this, i = 0](const auto& entity) mutable 
		{ 
			m_pixels[i].color = entity->getColour();
			++i;
		});

	m_predatorCount = static_cast<int>(std::count_if(m_entities.begin(), m_entities.end(),
		[](const auto& entity) {
			return entity && entity->getType() == EntityType::Predator &&
				entity->getHealth() > 0;
		}));

	m_preyCount = static_cast<int>(std::count_if(m_entities.begin(), m_entities.end(),
		[](const auto& entity) {
			return entity && entity->getType() == EntityType::Prey;
		}));

	m_predatorCountText.setString("Predator: " + std::to_string(m_predatorCount));
	m_preyCountText.setString("Prey: " + std::to_string(m_preyCount));
}

void Application::updatePredator(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity)
{
	if (thisEntity->getHealth() <= 0)
	{
		--m_predatorCount;
		thisEntity->setType(EntityType::Nothing);
		return;
	}

	auto otherType = otherEntity->getType();

	switch (otherType)
	{
	case EntityType::Prey:
		m_eatenPreyStats.add(1);
		otherEntity->setType(EntityType::Predator);
		thisEntity->heal(otherEntity->getHealth());
		break;

	case EntityType::Nothing:
		thisEntity->move(*otherEntity);
		break;

	default:
		break;
	}
}

void Application::updatePrey(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity)
{
	auto otherType = otherEntity->getType();
	bool canReproduce = thisEntity->getHealth() >= Entity::MAX_HEALTH;


	switch (otherType)
	{
	case EntityType::Nothing:
		if (canReproduce)
		{
			thisEntity->reproduce(*otherEntity);
			m_reproducedPreyStats.add(1);
			thisEntity->setHealth(10);
		}
		else
		{
			thisEntity->move(*otherEntity);
		}
		break;

	default:
		break;
	}
}

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

