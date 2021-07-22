#include "snakecontroller.h"

void SnakeController::setSnake(std::shared_ptr<Snake> snake)
{
    _snake = std::move(snake);
}

std::shared_ptr<Snake> &SnakeController::snake()
{
    return _snake;
}
