#ifndef BASICAISNAKE_H
#define BASICAISNAKE_H

#include <QObject>
#include "game.h"
#include "snakecontroller.h"

class BasicAISnake : public QObject, public SnakeController
{
    Q_OBJECT
public:
    explicit BasicAISnake(QObject *parent = nullptr);

    void start(Game *game);

public slots:
    void processUpdate(std::set<Coordinates> updatedCells);

private:
    std::vector<std::vector<CellType>> field;
    Coordinates head;
};

#endif // BASICAISNAKE_H
