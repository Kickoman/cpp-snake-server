#ifndef BASICAISNAKE_H
#define BASICAISNAKE_H

#include <QObject>
#include <map>
#include "game.h"
#include "snakecontroller.h"

class BasicAISnake : public QObject, public SnakeController
{
    Q_OBJECT
public:
    explicit BasicAISnake(QObject *parent = nullptr);

public slots:
    void processUpdate(std::set<Coordinates> updatedCells);

protected:
    void initGame() override;

private:
    std::vector<std::vector<CellType>> field;
    Coordinates head;
};

#endif // BASICAISNAKE_H
