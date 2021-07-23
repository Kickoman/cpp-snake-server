#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include "snake.h"
#include "game.h"

class SnakeController
{
public:
    SnakeController();
    virtual ~SnakeController() = default;

    void startGame(Game * game);

    const Game *game() const;
    std::shared_ptr<Snake> & snake();

protected:
    virtual void initGame() = 0;

private:
    Game *_game = nullptr;
    std::shared_ptr<Snake> _snake;
};

#endif // SNAKECONTROLLER_H
