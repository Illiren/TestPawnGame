#include <iostream>
#include <assert.h>
#include "game.hpp"
#include "controller.hpp"


std::string PawnGame::PathWhiteTxt = "resources/whitepawn.png";
std::string PawnGame::PathBlackTxt = "resources/blackpawn.png";
std::string PawnGame::PathFont = "resources/xkcd-script.ttf";

PawnGame::PawnGame(Size windowSize, Controller *whitePlayer, Controller *blackPlayer) :
    _board({8,8},windowSize,Color(100,100,100),Color(150,150,150)),
    _selectedPawn(nullptr),
    controller{whitePlayer,blackPlayer},
    currentPlayer{0}
{    
    if(!texturedb[0].loadFromFile(PathWhiteTxt))
        std::cout << "Error loading white pawn texture\n";

    if(!texturedb[1].loadFromFile(PathBlackTxt))
        std::cout << "Error loading black pawn texture\n";
}

bool PawnGame::isPawnInArea(const Pawn &pawn, Controller *owner) const
{
    //TODO: as square it can be just one position
    Position areaTopLeft;
    Position areaBotRight;

    if(owner==controller[0])
    {
        areaTopLeft = {0,0};
        areaBotRight = {2,2};
    }
    else
    {
        areaTopLeft = {_board.boardSize().x-3, _board.boardSize().y-3};
        areaBotRight = {_board.boardSize().x-1, _board.boardSize().y-1};
    }

    if(!(pawn.position().x >= areaTopLeft.x && pawn.position().x <=areaBotRight.x &&
         pawn.position().y >= areaTopLeft.y && pawn.position().y <=areaBotRight.y))
    {
        return false;
    }

    return true;
}

Controller *PawnGame::isWinCond() const
{
    bool isWhiteWin = true;
    for(const auto &pawn : whitePawns)
    {
        if(!(isWhiteWin = isPawnInArea(pawn,controller[0])))
        {
            break;
        }
    }
    bool isBlackWin = true;

    auto leftBound = _board.boardSize().x - 3;
    auto topBound = _board.boardSize().y - 3;

    for(const auto &pawn : blackPawns)
    {
        if(!(isBlackWin=isPawnInArea(pawn,controller[1])))
        {
            break;
        }
    }

    if(isWhiteWin)
        return controller[0];
    if(isBlackWin)
        return controller[1];

    return nullptr;
}

bool PawnGame::mouseEvent(Position clickPos)
{
    auto boardSize = _board.boardSize();
    auto windowSize = _board.windowSize();

    auto pos = clickPos/(windowSize/boardSize);

    //std::cout << "X: " << pos.x << "," << " Y: " << pos.y << std::endl;

    return positionEvent(pos);
}

bool PawnGame::positionEvent(Position pos)
{
    if(auto pawn = getMyPawnAt(controller[currentPlayer],pos))
    {
        setSelected(pawn);

        checkMoves(pawn);
        generateAvailableMoveRects();

        //For debug sake
        //for(auto &pos : _availableMovePos)
        //    std::cout << "X: " << pos.x << "," << " Y: " << pos.y << std::endl;
    }
    else if(_selectedPawn)
    {
        for(auto &newPos : _availableMovePos)
        {
            if(newPos == pos)
            {
                _selectedPawn->setPos(newPos);
                _selectedPawn = nullptr;
                _availableMovePos.clear();
                _availableMoveRects.clear();
                switchPlayer();
                auto Controller = isWinCond();
                if(Controller)
                {
                    setWinner(Controller);
                    std::cout << Controller->someID << " win" << std::endl;
                }

                return true; //Move was maden
            }
            else
            {
                //Clear selection ?
            }
        }
    }
    return false;
}

void PawnGame::switchPlayer()
{
    currentPlayer ^=1;
    controller[currentPlayer]->yourMove();
}

void PawnGame::start()
{
    blackPawns.clear();
    whitePawns.clear();
    auto boardSize = _board.boardSize();
    auto scale = _board.calcScale();
    for(auto i=0;i<3;i++)
        for(auto j=0;j<3;j++)
        {
            blackPawns.emplace_back(Pawn({i,j},scale,texturedb[1]));
            whitePawns.emplace_back(Pawn({boardSize.x-1-i,boardSize.y-1-j},scale,texturedb[0]));
        }
    controller[0]->init(this);
    controller[1]->init(this);
    currentPlayer=1;
    switchPlayer();
}

vector<Pawn> &PawnGame::getMyPawns(Controller *caller)
{
    if(controller[0] == caller)
        return whitePawns;
    else
        return blackPawns;
}

vector<Pawn> &PawnGame::getOpponentPawns(Controller *caller)
{
    if(controller[1] == caller)
        return whitePawns;
    else
        return blackPawns;
}

Pawn *PawnGame::getMyPawnAt(Controller *caller, Position pos)
{
    auto &pawns = getMyPawns(controller[currentPlayer]);
    for(auto &pawn : pawns)
        if(pawn.position() == pos)
            return &pawn;
    return nullptr;
}

std::vector<Position> PawnGame::getAvailableMoves(Pawn *pawn)
{
    std::vector<Position> moves;
    auto pos = pawn->position();
    for(int i=-1; i<=1;++i)
    {
        if(i==0) continue;
        Position newPosX{pos.x+i,pos.y};
        Position newPosY{pos.x,pos.y+i};

        if(_board.isValid(newPosX) && (pawnAt(newPosX) == nullptr))
            moves.push_back(newPosX);
        if(_board.isValid(newPosY) && (pawnAt(newPosY) == nullptr))
            moves.push_back(newPosY);
    }
    return moves;
}

const Controller *PawnGame::getCurrentController() const
{
    return controller[currentPlayer];
}


Pawn *PawnGame::pawnAt(Position pos)
{
    for(auto &pawn : blackPawns)
        if(pawn.position() == pos)
            return &pawn;

    for(auto &pawn : whitePawns)
        if(pawn.position() == pos)
            return &pawn;

    return nullptr;
}

void PawnGame::setSelected(Pawn *pawn)
{
    assert(pawn!=nullptr && "Pawn is invalid");
    _selectedPawn = pawn;

    auto pos = pawn->position();
    auto scale = _board.calcScale();

    _selection.setPosition(pos*scale+scale/2.f);
    _selection.setSize(scale);
    _selection.setOrigin(scale/2.f);
    _selection.setFillColor(sf::Color(0x00FF0055));
    _selection.setOutlineColor(sf::Color::Green);
    _selection.setOutlineThickness(-2.f);
}

bool PawnGame::isValidPos(Position pos)
{
    return _board.isValid(pos) && (pawnAt(pos) == nullptr);
}

void PawnGame::checkMoves(Pawn *pawn)
{
    assert(pawn!=nullptr && "Pawn is invalid");

    _availableMovePos.clear();;

    auto pos = pawn->position();

    /* Chebyshev distance
    for(Position it{-1,-1};it.x<=1;it.x++)
        for(it.y=-1;it.y<=1;it.y++)
        {
            Position newPos = pos+it;

            if(pos == newPos) continue;

            if(_board.isValid(newPos) && (pawnAt(newPos) == nullptr))
            {
                _abailableMovePos.push_back(newPos);
            }
        }*/

    //Manhattan distance
    for(int i=-1; i<=1;++i)
    {
        if(i==0) continue;
        Position newPosX{pos.x+i,pos.y};
        Position newPosY{pos.x,pos.y+i};

        if(isValidPos(newPosX))
            _availableMovePos.push_back(newPosX);
        if(isValidPos(newPosY))
            _availableMovePos.push_back(newPosY);
    }
}

void PawnGame::generateAvailableMoveRects()
{
    _availableMoveRects.clear();
    auto scale = _board.calcScale();

    for(auto &pos : _availableMovePos)
    {
        sf::RectangleShape rect;
        rect.setPosition(pos*scale+scale/2.f);
        rect.setSize(scale);
        rect.setOrigin(scale/2.f);
        rect.setFillColor(sf::Color(0x00FF0088));
        _availableMoveRects.push_back(rect);
    }
}

void PawnGame::setWinner(Controller *ctrl)
{
    std::string winText;

    assert(ctrl != nullptr && "Something is very very wrong");

    if(ctrl == controller[0])
        winText = "The white is win";
    else
        winText = "The black is win";

    font.loadFromFile(PathFont);
    youWinText.setFont(font);
    youWinText.setString(winText);
    youWinText.setFillColor(sf::Color::Red);
    youWinText.setCharacterSize(64);
    auto rect = youWinText.getLocalBounds();
    youWinText.setOrigin(rect.left + rect.width/2.f,
                         rect.top + rect.height/2.f);
    youWinText.setPosition(_board.windowSize().x/2, _board.windowSize().y/2);

    gameOver=true;
}

void PawnGame::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.clear(sf::Color::Black);
    target.draw(_board);

    if(gameOver)
    {
        target.draw(youWinText);
    }
    else
    {
        for(auto &pawn : whitePawns)
            target.draw(pawn);

        for(auto &pawn : blackPawns)
            target.draw(pawn);

        for(auto &rect : _availableMoveRects)
            target.draw(rect);

        if(_selectedPawn !=nullptr)
            target.draw(_selection);
    }
}
