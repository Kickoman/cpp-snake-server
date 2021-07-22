#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "field.h"
#include "snake.h"
#include "foodmanager.h"

class Game : public QObject
{
    Q_OBJECT
public:
    struct Config
    {
        size_t fieldWidth = 100;
        size_t fieldHeight = 70;

        std::chrono::milliseconds timePerTick;
    };

    explicit Game(const Config &config, QObject *parent = nullptr);

    void startGame();
    void stopGame();

    void addSnake(std::shared_ptr<Snake> snake);
    void removeSnake(int64_t id);

signals:
    void updated(std::set<Coordinates> updatedCells);

private slots:
    void gameTick();

private:
    Config _config;
    QTimer *_timer = nullptr;

    std::unique_ptr<FoodManager> _foodManager;
    std::shared_ptr<Field> _field;
    std::map<int64_t, std::shared_ptr<Snake>> _snakes;
};

#endif // GAME_H
