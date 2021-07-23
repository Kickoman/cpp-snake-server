#include "snakecontroller.h"

SnakeController::SnakeController()
    : _snake(std::make_shared<Snake>())
{}

void SnakeController::startGame(Game *game)
{
    _game = game;
    game->addSnake(snake());
    initGame();
}

const Game *SnakeController::game() const
{
    return _game;
}

std::shared_ptr<Snake> &SnakeController::snake()
{
    return _snake;
}
