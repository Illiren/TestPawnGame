#include "pawn.hpp"

Pawn::Pawn(Position pos, Scale scale, const sf::Texture &texture):
    _sprite(texture),
    _pos(pos),
    _scale(scale)
{
    auto size = texture.getSize();

    _sprite.setOrigin(Scale(size)/2.f);
    _sprite.setScale(scale/size);
    _sprite.setPosition(pos*scale+scale/2);
}

void Pawn::setPos(Position pos)
{
    _pos = pos;
    _sprite.setPosition(_pos*_scale+_scale/2);
}

const Position &Pawn::position() const
{
    return _pos;
}

void Pawn::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite);    
}
