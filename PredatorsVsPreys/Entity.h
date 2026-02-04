#include "SFML/Graphics.hpp"

enum class EntityType
{
	Predator = 0,
	Prey = 1,
	Nothing = 2

};

class Entity
{

public:

	constexpr static int MAX_HEALTH = 100;


	Entity();

	sf::Color getColour() noexcept;

	EntityType getType() const noexcept;

	void setType(EntityType type) noexcept;

	void heal(int amount);

	int getHealth() const noexcept;

	void setHealth(int val) noexcept;

	void update();

	void reproduce(Entity& other);

	void move(Entity& other);

private:
	EntityType m_type; 

	int m_health = MAX_HEALTH / 5;
};