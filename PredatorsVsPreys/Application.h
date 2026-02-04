#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/System/Clock.hpp>
#include "Entity.h"
#include "Statistics.h" 

class Application
{
public:

	Application();

	void run();

	void saveState(const std::string& fileName);

	void readAndDisplayStats(const std::string& fileName);

private:
	int WIDTH = 700;
	int HEIGHT = 700;

	int getIndex(int x, int y) const;

	void pollEvents();

	void update();

	void updatePredator(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity);

	void updatePrey(std::unique_ptr<Entity>& thisEntity, std::unique_ptr<Entity>& otherEntity);

	void loadConfig(const std::string& fileName);

	std::unique_ptr<sf::RenderWindow> m_window; 
	std::vector<sf::Vertex> m_pixels; 
	std::vector<std::unique_ptr<Entity>> m_entities; 

	int m_preyCount = 0; 
	int m_predatorCount = 0; 


	std::unique_ptr<sf::Font> m_font;
	sf::Text m_preyCountText; 
	sf::Text m_predatorCountText; 

	Statistics<int> m_reproducedPreyStats; 
	Statistics<int> m_eatenPreyStats; 

	sf::Clock m_saveClock;
};