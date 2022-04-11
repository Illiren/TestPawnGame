#include "controller.hpp"
#include <iostream>
#include <random>
#include <map>
#include <assert.h>
#include <limits>

Controller::Controller()
{
    //Просто случайный идентификатор, чтобы опозновать контроллер.
    static int id = 0;
    someID = id++;
}

bool Controller::mouseEvent(Position pos) const
{
    return game->mouseEvent(pos);
}

void Controller::init(PawnGame *owner)
{
    game = owner;
}

void Controller::yourMove()
{
    std::cout << "Your move, Player:" << someID << std::endl;
}

RandomAI::RandomAI() :
    Controller()
{}

void RandomAI::yourMove()
{
    auto &pawns = game->getMyPawns(this);

    vector<Pawn*> availablePawns;
    for(auto &pawn : pawns)
    {
        if(!game->getAvailableMoves(&pawn).empty())
            availablePawns.push_back(&pawn);
    }

    auto id = rand()%availablePawns.size();

    auto pawn = availablePawns[id];
    auto moves = game->getAvailableMoves(pawn);
    id = rand()%moves.size();
    auto movePos = moves[id];
    game->positionEvent(pawn->position());
    game->positionEvent(movePos);
}

AINo2::AINo2():
    Controller()
{}

float AINo2::minDist(Position p)
{
    Position areaCenter = {6, 6};
    float min = 1000000.f;
    for(Position it = {-1,-1}; it.x <= 1; it.x++)
        for(it.y = -1; it.y <= 1; it.y++)
        {
            float d;
            if((d = manhattanDist(p, areaCenter+it)) < min)
            {
                min = d;
            }
        }

    return min;
}


float AINo2::getBestMove(Pawn * pawn, Position &pos)
{
    auto available = game->getAvailableMoves(pawn);
    float result=std::numeric_limits<float>::max(); //Число, больше которого быть не может
    if(available.size() == 0)
        return result;

    //Если дошли до области, забиваемся в угол
    if(game->isPawnInArea(*pawn,this))
    {
        Position xPos = {pawn->position().x+1, pawn->position().y};
        Position yPos = {pawn->position().x, pawn->position().y+1};

        if(game->isValidPos(xPos))
        {
            pos = xPos;
            return 0.f;
        }
        else if(game->isValidPos(yPos))
        {
            pos = yPos;
            return 0.f;
        }
        else
        {
            return result;
        }
    }

    auto pawnDist = minDist(pawn->position());
    //Иначе ищем лучший ход
    for(auto &newPos : available)
    {
        float dis = minDist(newPos);
        auto diff = dis - pawnDist;
        if(diff < result)
        {
            result = diff;
            pos = newPos;
        }
    }

    return result;
}

void AINo2::yourMove()
{
    auto &pawns = game->getMyPawns(this);

    struct PawnBestMove
    {
        Pawn *pawn;
        Position pos;
    };

    std::map<float, PawnBestMove> bestMoves;

    float range;
    Position pos;
    for(auto &pawn : pawns)
    {
        range = getBestMove(&pawn,pos);
        bestMoves.insert({range,{&pawn,pos}});
    }

    PawnBestMove best = (*bestMoves.begin()).second;
    best.pos;

    game->positionEvent(best.pawn->position());
    game->positionEvent(best.pos);
}

void AINo2::init(PawnGame *owner)
{
    game = owner;    
}


AINo3::AINo3():
    Controller(),
    winPos({{7,7},{6,7},{6,6},{7,6},{7,5},{6,5},{5,5},{5,6},{6,7}})
{

}

Position AINo3::findFreePositionInArea(Position pawnPos) const
{
    float min = std::numeric_limits<float>::max();
    Position p = {7,7};
    for(const auto &pos : winPos)
    {
        auto d = manhattanDist(pawnPos,pos);
        if(game->isValidPos(pos) && d < min)
        {
            p = pos;
            min = d;
        }
    }
    return p;
}

void AINo3::updateMove(AINo3::MoveInfo &move)
{
    Position targetPos = {7,7};
    if(game->isPawnInArea(*move.pawn,this))
    {
        // targetPos = {7,7};
    }
    else
    {
        targetPos = findFreePositionInArea(move.pawn->position());
    }

    move.curentRange = manhattanDist(move.pawn->position(),targetPos);

    auto avPos = game->getAvailableMoves(move.pawn);

    move.bestMoveRange = std::numeric_limits<float>::max();

    for(auto &pos : avPos)
    {
        float newDist = manhattanDist(pos, targetPos);
        if(newDist < move.bestMoveRange)
        {
            move.bestMoveRange = newDist;
            move.pos = pos;
        }
    }
}

void AINo3::updateMoves()
{
    for(auto &pawnInfo : moveInfos)
    {
        updateMove(pawnInfo);
        if(game->isPawnInArea(*pawnInfo.pawn,this))
            pawnInfo.weight = 2;
    }
    moveInfos.sort([](const MoveInfo &lhs, const MoveInfo &rhs) -> bool {return lhs.getWeight() < rhs.getWeight();});
}

void AINo3::yourMove()
{
    updateMoves();

    auto it = std::find_if(moveInfos.begin(), moveInfos.end(),
                           [](const MoveInfo &mv) -> bool {return mv.getWeight() > 0;});

    assert(it != moveInfos.end() && "No moves");

    game->positionEvent((*it).pawn->position());
    game->positionEvent((*it).pos);
}

void AINo3::init(PawnGame *owner)
{
    game = owner;
    moveInfos.clear();
    auto &pawns = game->getMyPawns(this);

    for(auto &pawn : pawns)
    {
        moveInfos.push_back({&pawn,{0,0},0.f,0.f,0.f});
    }
}
