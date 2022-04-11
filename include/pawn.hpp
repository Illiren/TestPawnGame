#pragma once

#include <SFML/Graphics.hpp>
#include "typedefines.hpp"


/**
 * @brief Класс пешки (Других фигур нету)
 *
 * Класс описывает как выглядит пешка, где находится и всё.
 */

class Pawn : public sf::Drawable
{
public:    
    /**
     * @brief Конструктор пешек
     * @param pos - позиция
     * @param scale - маштаб
     * @param texture - текстура для спрайта
     */
    Pawn(Position pos, Scale scale, const sf::Texture &texture);

    /**
     * @brief установить позицию
     * @param pos - новая позиция пешки
     */
    void setPos(Position pos);
    /**
     * @brief Функция получения координат пешки
     * @return координаты пешки
     */
    const Position &position() const;

private:
    sf::Sprite _sprite; //< Спрайт пешки
    Position _pos;      //< Позиция пешки
    Scale    _scale;    //< Маштаб пешки

    // Drawable interface
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
