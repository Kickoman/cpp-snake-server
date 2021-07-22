#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include "snake.h"

class SnakeController
{
public:
    virtual ~SnakeController() = default;

    virtual void setSnake(std::shared_ptr<Snake> snake);
    std::shared_ptr<Snake> & snake();

private:
    std::shared_ptr<Snake> _snake;
};

#endif // SNAKECONTROLLER_H
