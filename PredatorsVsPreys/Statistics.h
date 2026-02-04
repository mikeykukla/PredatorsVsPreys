#pragma once

template<typename T>
class Statistics
{
public:
    
    void add(const T& value)
    {
        m_sum += value;
    }

    size_t sum() const
    {
        return static_cast<size_t>(m_sum);
    }


private:
    T m_sum{ 0 };  
};