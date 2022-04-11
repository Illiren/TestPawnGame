#include "board.hpp"



Board::Board(Size boardSize, Size windowSize, Color color1, Color color2) :
    _boardRects(boardSize.x*boardSize.y),
    _boardSize(boardSize),
    _windowSize(windowSize)
{
    setColor(color1,color2);
}

void Board::setColor(sf::Color color1, sf::Color color2)
{
    auto scale = calcScale();
    bool colorId = 0;
    _colors[0]=color1;
    _colors[1] = color2;
    for(auto y=_boardSize.y;y--;)
    {
        colorId = (y & 1);
        for(auto x=_boardSize.x;x--;)
        {
            _boardRects[pTi({x,y})].setPosition(sf::Vector2f(y*scale.x, x*scale.y));
            _boardRects[pTi({x,y})].setSize(sf::Vector2f(scale.x,scale.y));
            _boardRects[pTi({x,y})].setFillColor(_colors[colorId]);
            colorId ^= 1;
        }
    }
}

const Size &Board::boardSize() const
{
    return _boardSize;
}

const Size &Board::windowSize() const
{
    return _windowSize;
}

Scale Board::calcScale()
{
    return Scale(float(_windowSize.x)/float(_boardSize.x),
                 float(_windowSize.y)/float(_boardSize.y));
}

bool Board::isValid(Position pos) const
{
    if(pos.x >= 0 && pos.y >= 0 && pos.x < _boardSize.x && pos.y < _boardSize.y)
        return true;
    return false;
}

size_t Board::pTi(Position pos) const
{
    return pos.x+pos.y*_boardSize.x;
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(auto i = _boardRects.size(); i--; )
        target.draw(_boardRects[i]);
}
