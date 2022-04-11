#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

/**
 *
 * @file Typedefines.hpp
 *
 * @brief Файл для определения используемых типов
 *
 * Просто файл для определения используемы типов плюс несколько полезных функций
 */


using Position = sf::Vector2i;
using Scale = sf::Vector2f;
using Size = sf::Vector2i;

using std::vector;
using std::size_t;
using sf::Color;

template<typename T, typename U>
inline sf::Vector2<T> operator/(sf::Vector2<T> lhs, U rhs)
{
    return sf::Vector2<T>(lhs.x/rhs,lhs.y/rhs);
}

template<>
inline sf::Vector2<float> operator/(sf::Vector2<float> lhs, float rhs)
{
    return sf::Vector2<float>(lhs.x/rhs,lhs.y/rhs);
}

template<typename T, typename U>
inline sf::Vector2<T> operator/(sf::Vector2<T> lhs, sf::Vector2<U> rhs)
{
    return sf::Vector2<T>(lhs.x/rhs.x,lhs.y/rhs.y);
}

template<typename T>
inline sf::Vector2<float> operator/(sf::Vector2<T> lhs, sf::Vector2<float> rhs)
{
    return sf::Vector2<float>(lhs.x/rhs.x,lhs.y/rhs.y);
}

template<typename T, typename U>
inline sf::Vector2<T> operator*(sf::Vector2<T> lhs, sf::Vector2<U> rhs)
{
    return sf::Vector2<T>(lhs.x*rhs.x,lhs.y*rhs.y);
}

template<typename T>
inline sf::Vector2<float> operator*(sf::Vector2<T> lhs, sf::Vector2<float> rhs)
{
    return sf::Vector2<float>(lhs.x*rhs.x,lhs.y*rhs.y);
}
