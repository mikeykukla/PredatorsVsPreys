#include "Entity.h"
#include <cstdlib>
#include <random>


Entity::Entity()
{
	static std::random_device rd;
	static std::mt19937 rng(rd()); 
	std::uniform_int_distribution<int> dist(0, 1000);

	int n = dist(rng);

	if (n > 100)
	{
		m_type = EntityType::Nothing;
	}
	else if (n > 50)
	{
		m_type = EntityType::Prey;
	}
	else
	{
		m_type = EntityType::Predator;
	}
}

sf::Color Entity::getColour() noexcept
{
	if (m_type == EntityType::Nothing || m_health == 0)
	{
		return sf::Color::Black;
	}
	else
	{
		float normalisedHealth = (float)m_health / (float)MAX_HEALTH;
		uint8_t col = static_cast<uint8_t>(normalisedHealth * 255);

		switch (m_type)
		{
		case EntityType::Predator:
			return { col , 0, 0 };
		case EntityType::Prey:
			return { 0, col, 0 };
		default:
			return sf::Color::Black;
		}
	}
}

EntityType Entity::getType() const noexcept
{
	return m_type;
}

void Entity::setType(EntityType type) noexcept
{
	m_type = type;

}

void Entity::heal(int amount)
{
	m_health += amount;
	m_health = std::min(m_health, MAX_HEALTH);
}

int Entity::getHealth() const noexcept
{
	return m_health;
}

void Entity::update()
{
	switch (m_type)
	{
	case EntityType::Predator:
		heal(-1);
		break;
	case EntityType::Prey:
		heal(1);
		break;
	default:
		break;
	}
}

void Entity::reproduce(Entity& other)
{
	other.m_health = 10;                
	other.m_type = EntityType::Prey;
}

void Entity::move(Entity& other)
{
	other.m_health = m_health; 
	other.m_type = m_type;
	m_type = EntityType::Nothing;
}

void Entity::setHealth(int val) noexcept
{
	m_health = val;
}