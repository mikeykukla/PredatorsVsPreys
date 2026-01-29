#include "Entity.h"
#include <cstdlib>
#include <random>

// Konstruktor klasy Entity losowo okresla typ nowo utworzonej encji bazujac na prawdopodobienstwie
Entity::Entity()
{
	// Staly generator losowy
	// Generator jest tworzony tylko raz
	static std::random_device rd; // Tworzy raz ziarno 
	static std::mt19937 rng(rd()); // Tworzy raz generator (Mersenne Twister) z ziarnem z rd (Tablica 624 liczb)
	std::uniform_int_distribution<int> dist(0, 1000); // Skaluje liczby z rng do zakresu 0-1000

	// operator() funktor
	int n = dist(rng);

	if (n > 100)
	{
		m_type = EntityType::Nothing; // Prawdopodobienstwo na pole puste (Nothing)
	}
	else if (n > 50)
	{
		m_type = EntityType::Prey;    // Prawdopodobieństwo na ofiary (Preys)
	}
	else
	{
		m_type = EntityType::Predator; // Prawdopodobienstwo na drapiezniki (Predators)
	}
}


// Zwraca kolor encji w zaleznosci od jej typu i zdrowia
// Intensywnosc koloru odzwierciedla poziom zdrowia 
sf::Color Entity::getColour() noexcept
{
	if (m_type == EntityType::Nothing || m_health == 0)
	{
		return sf::Color::Black;
	}
	else
	{
		float normalisedHealth = (float)m_health / (float)MAX_HEALTH; // Normalizacja zdrowia do zakresu 0.0 - 1.0
		uint8_t col = static_cast<uint8_t>(normalisedHealth * 255); // Konwersja do wartosci RGB (0-255)

		switch (m_type)
		{
		case EntityType::Predator:
			return { col , 0, 0 }; // Czerwony dla drapieżnikow
		case EntityType::Prey:
			return { 0, col, 0 }; // Zielony dla ofiar
		default:
			return sf::Color::Black;
		}
	}
}


// Zwraca typ encji
EntityType Entity::getType() const noexcept
{
	return m_type;
}

// Ustawia nowy typ encji
void Entity::setType(EntityType type) noexcept
{
	m_type = type;

}

// Zmienia wartosc zdrowia encji o podana wartosc
void Entity::heal(int amount)
{
	m_health += amount;
	m_health = std::min(m_health, MAX_HEALTH); // Ogranicz do MAX_HEALTH
}

// Zwraca aktualne zdrowie encji
int Entity::getHealth() const noexcept
{
	return m_health;
}

// Aktualizuje stan encji w kazdej klatce symulacji
void Entity::update()
{
	switch (m_type)
	{
	case EntityType::Predator:
		heal(-1); // Drapieznik traci 1 punkt zdrowia
		break;
	case EntityType::Prey:
		heal(1); // Ofiara zyskuje 1 punkt zdrowia
		break;
	default:
		break;
	}
}

// Rozmanzanie ofiary - tworzenie ofiary na pustym polu
// Other to pole, na ktorym pojawi sie potomek
void Entity::reproduce(Entity& other)
{
	other.m_health = 10;                 // Poczatkowe zdrowie potomstwa
	other.m_type = EntityType::Prey;  // Nowo narodzone stworzenia to zawsze ofiary
}

// Przenosi encje na sasiednie puste pole
// Aktualna pozycja staje sie pusta, nowa pozcyja przejmuje parametry encji
void Entity::move(Entity& other)
{
	other.m_health = m_health; // Przekazanie zdrowia
	other.m_type = m_type;	// Przekazanie typu
	m_type = EntityType::Nothing; // Stara pozycja staje sie pusta
}

// Ustawia zdrowie encji na konkretna wartosc
void Entity::setHealth(int val) noexcept
{
	m_health = val;
}