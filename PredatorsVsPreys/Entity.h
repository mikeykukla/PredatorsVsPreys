#include "SFML/Graphics.hpp"

// Enum okreslajacy typ encji w symulacji
enum class EntityType
{
	Predator = 0,
	Prey = 1,
	Nothing = 2

};

class Entity
{

public:
	// Maksymalne zdrowie encji - stala wartosc dla wszystkich obiektow
	constexpr static int MAX_HEALTH = 100;

	// Konstruktor - tworzy losowa encje
	Entity();

	// Zwraca kolor encji bazujacy na jej typie i zdrowiu
	sf::Color getColour() noexcept;

	// Zwraca typ encji
	EntityType getType() const noexcept;

	// Ustawia typ encji
	void setType(EntityType type) noexcept;

	// Zmienia zdrowie encji o podana wartosc
	// Ujemna - zadaje obrazenia Dodatnia - leczy
	void heal(int amount);

	// Zwraca aktualne zdrowie encji
	int getHealth() const noexcept;

	// Ustawia zdrowie encji
	void setHealth(int val) noexcept;

	// Aktualizuje stan encji w kazdej klatce
	void update();

	// Tworzy nowa ofiare
	// other to pole na ktorym powstanie potomek
	void reproduce(Entity& other);

	// Przenosi encje na nowe pole
	// Other to docelowe pole
	void move(Entity& other);


private:
	// Typ encji
	EntityType m_type; 

	// Poczatkowe zdrowie encji 20% maksymalnego
	int m_health = MAX_HEALTH / 5;
};