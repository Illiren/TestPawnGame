#pragma once


#include "board.hpp"
#include "pawn.hpp"

class Controller; //Forward declaration

/**
 * @brief Класс игры
 *
 * Рисует доску и фигуры и управляет распределением указаний из контроллера
 *
 */
class PawnGame : public sf::Drawable
{
public:
    /**
     * @brief Конструктор игры
     * @param windowSize - размер окна
     * @param whitePlayer - контроллер, который управляет белыми пешками
     * @param blackPlayer - контроллер, который управляет чёрными
     */
    PawnGame(Size windowSize, Controller *whitePlayer, Controller *blackPlayer);

    /**
     * @brief Проверка выигрышной ситуации
     * @return возвращает, если ситуация выигрышная, то контроллер, который выиграл, иначе nullptr
     */
    Controller *isWinCond() const;
    /**
     * @brief Обрабатывает нажатие мыши
     * @param clickPos позиция мыши
     * @return true, если был сделан ход, иначе false
     */
    bool mouseEvent(Position clickPos);
    /**
     * @brief Нажатие на позицию на доске
     * @param pos - позиция на доске
     * @return true, если был сделан ход, иначе false
     */
    bool positionEvent(Position pos);

    /**
     * @brief Запуск
     * Устанавливает пешки на места и даёт ход первому игроку
     */
    void start();

    /**
     * @brief Проверка, находится ли пешка в выигрышной позиции для выбранного игрока
     * @param pawn - пешка
     * @param owner - игрок
     * @return true, если находится
     */
    bool isPawnInArea(const Pawn &pawn, Controller *owner) const;
    /**
     * @brief Проверка на валидность позиции
     * @param pos - позиция
     * @return true - если позицию не занимает другая пешка и если позиция не выходит за рамки поля
     */
    bool isValidPos(Position pos);

    /**
     * @brief Получить ссылку на пешки игрока \n
     * В основном, функция для ИИ
     * @param caller - игрок
     * @return ссылка на пешки игроа
     */
    vector<Pawn> &getMyPawns(Controller *caller);
    /**
     * @brief Получить ссылку на пешки опонента игрока
     * @param caller - игрок
     * @return ссылка на пешки игроа
     */
    vector<Pawn> &getOpponentPawns(Controller *caller);

    /**
     * @brief Получить список возможных ходов для пешки
     * @param pawn - пешка
     * @return Массив возможных ходов
     */
    std::vector<Position> getAvailableMoves(Pawn *pawn);

    /**
     * @brief Получить указатель на игрока, который ходит в данный момент
     * @return указатель на контроллер игрока
     */
    const Controller *getCurrentController() const;

private:
    Board _board;               //< Доска
    vector<Pawn> whitePawns;    //< Массив белых пешек
    vector<Pawn> blackPawns;    //< Массив чёрных пешек
    sf::Texture texturedb[2];   //< Тут храняться текстуры для пешек

    Pawn*                      _selectedPawn;       //< Текущая выделенная пешка
    sf::RectangleShape         _selection;          //< Красивая обводка выделенной пешки
    vector<sf::RectangleShape> _availableMoveRects; //< Отметки возможных ходов для выделенной пешки
    vector<Position>           _availableMovePos;   //< Позиции возможных ходов для выделенной пешки

    Controller *controller[2]; //< Два игрока
    bool        currentPlayer; //< текущий игрок

    bool gameOver;             //< Бит, который показывает, закончена игра или нет
    sf::Text youWinText;       //< Текст для победителя,
    sf::Font font;             //< Шрифт для текста для победителя


    static std::string PathWhiteTxt; //< Путь к ресурсу
    static std::string PathBlackTxt; //< Путь к ресурсу
    static std::string PathFont;     //< Путь к ресурсу


    /**
     * @brief Функция для нахождения пешки на заданной позиции
     * @param pos - позиция
     * @return nullptr, если пешка не найдена
     */
    Pawn *pawnAt(Position pos);

    /**
     * @brief Выделить пешку
     * @param pawn - пешка
     */
    void setSelected(Pawn *pawn);

    /**
     * @brief Проверить Возможные ходы для пешки
     * @param pawn - пешка
     */
    void checkMoves(Pawn *pawn);

    /**
     * @brief Выделить Возможные ходы для пешки
     * @param pawn - пешка
     */
    void generateAvailableMoveRects();

    /**
     * @brief Функция для нахождения пешки на заданной позиции
     * @param ctrl - позиция
     * @return nullptr, если пешка не найдена
     */
    void setWinner(Controller *ctrl);

    /**
     * @brief Переключить игроков
     * Передаёт ход следующиму игроку
     */
    void switchPlayer();

    /**
     * @brief Получить указатель на пешку игрока, которая находится на данной позиции
     * @param caller - игрок
     * @param pos - позиция
     * @return nullptr, если пешка не найдена
     */
    Pawn *getMyPawnAt(Controller *caller, Position pos);

    // Drawable interface
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
