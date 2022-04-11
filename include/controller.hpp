#pragma once

#include <tuple>
#include <list>
#include  "game.hpp"

/**
 * @brief Дистанция по метрики городских кварталов
 * @param lhs откуда
 * @param rhs куда
 * @return дистанция
 */
static float manhattanDist(Position lhs, Position rhs)
{
    return std::abs(lhs.x-rhs.x)+std::abs(lhs.y-rhs.y);
}


/**
 * @brief Класс Контроллера
 *
 * Контроллер представляет игрока. Определяет интерфейс взаимодействия игрока.
 * Определяет реализацию управления для игрока - человека
 *
 */
class Controller
{
public:
    Controller();

    bool mouseEvent(Position pos) const;

protected:
    int someID = 1;
    PawnGame *game;


    /**
     * @brief Функция инициализации
     * Вызывается классом игры, она же устанавлиает параметр
     * @param owner - Игра-владелец контроллера
     */
    virtual void init(PawnGame *owner);
    /**
     * @brief Функция хода
     * Событие, которое вызывается игрой, когда настуает ход этого контроллера
     */
    virtual void yourMove();

    friend class PawnGame;
};

/**
 * @brief Тупой рандомный ии
 *
 * ИИ, который ходит предельно тупо. Нужен для тестирования интерфейсов контроллера для ИИ
 */
class RandomAI : public Controller
{
public:
    RandomAI();

    // Controller interface
protected:
    void yourMove() override;
};


/**
 * @brief ИИ поумнее предыдущего
 *
 * Но тоже довольно тупой. Старается ходить поближе к победной области и всё
 */
class AINo2 : public Controller
{    

public:
    AINo2();

private:    
    /**
     * @brief Функция нахождения минимальной позиции между точкой p и выигрышной зоной
     * @param p - точка p
     * @return минимальная позиция
     */
    float minDist(Position p);
    /**
     * @brief Функция нахождения лучшего хода для пешки
     * @param pawn - пешка
     * @param pos - тоже возвращаемый параметр
     * @return на сколько ближе станет пешка к выигрышной зоне
     */
    float getBestMove(Pawn *pawn, Position &pos);

protected:  // Controller interface
    void yourMove() override;
    void init(PawnGame *owner) override;
};


/**
 * @brief Класс, который не нужен
 * Такой класс должен быть в каждом проекте
 */
class AINo3 : public Controller
{
    struct MoveInfo
    {
        Pawn *pawn;
        Position pos;
        float curentRange;
        float bestMoveRange;
        float weight;
        float getWeight() const {return weight+curentRange - bestMoveRange;}
    };

public:
    AINo3();

private:
    std::list<MoveInfo> moveInfos;
    vector<Position>    winPos;

    Position findFreePositionInArea(Position pawnPos) const;

    void updateMove(MoveInfo &move);
    void updateMoves();
    void sort();




    void calcWeight();
    //void


protected:  // Controller interface
    void yourMove() override;
    void init(PawnGame *owner) override;
};
