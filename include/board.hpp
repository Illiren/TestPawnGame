#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "typedefines.hpp"

/**
 * @brief Класс Доски
 *
 * Данный класс описывает Шахматную доску со всеми параметрами отрисовки
 * Хранит в себе параметры окна, и количество клеток
 *
 */
class Board : public sf::Drawable
{
public:
    /**
     * @brief Board - конструктор всего этого
     * @param boardSize - размер доски в клетках
     * @param windowSize - размер окна
     * @param color1 - цвет раз
     * @param color2 - цвет два
     */
    Board(Size boardSize, Size windowSize, Color color1, Color color2);

    /**
     * @brief setColor - функция установки цвета
     * @param color1 - цвет один
     * @param color2 - цвет второй
     */
    void setColor(Color color1, Color color2);

    /**
     * @brief Функция получения размера доски в клетках
     * @return размер доски в клетках
     */
    const Size &boardSize() const;
    /**
     * @brief Функция получения размера окна
     * @return размер окна
     */
    const Size &windowSize() const;

    /**
     * @brief функция проверки валидности позиции
     * @param pos - позиция
     * @return true, если позиция находится на доске, иначе false
     */
    bool isValid(Position pos) const;

    /**
     * @brief calcScale функция расчёта масштаба клетки относительно окна
     * @return результат - маштаб одной клетки
     */
    Scale calcScale();

private:
    std::vector<sf::RectangleShape> _boardRects; //< Массив клеток рисованых
    Size   _boardSize;  //< Размер доски в клетках
    Size   _windowSize; //< Размер окна, в которое должна быть вписана доска
    Color  _colors[2];  //< Используемые цвета

    /**
     * @brief перевод 2д позиции в индекс в одномерном массиве квадратов
     * @param pos - 2д координаты
     * @return 1д координаты
     */
    size_t pTi(Position pos) const;

    // Drawable interface
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
