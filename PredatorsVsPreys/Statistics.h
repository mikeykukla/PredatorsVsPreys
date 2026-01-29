#pragma once

//SZABLON: Statistics zbiera i analizuje dane statystyczne
template<typename T>
class Statistics
{
public:
    
    // Dodaje nowa wartosc do statystyk
    void add(const T& value)
    {
        m_sum += value;
    }

    // Zwraca liczbe dodanych wartosci
    size_t sum() const
    {
        return static_cast<size_t>(m_sum); // Jawna konwersja int na size_t
    }


private:
    T m_sum{ 0 }; // Suma wszystkich dodanych wartosci (inicjalizacja: 0)       
};