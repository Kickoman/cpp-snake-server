#include "game.h"
#include <QTimer>
#include <QDebug>
#include <cstdio>

Game::Game(const Config &config, QObject *parent)
    : QObject(parent)
    , _config(config)
    , _timer(new QTimer(this))
{
    qRegisterMetaType<std::set<Coordinates>>("std::set<Coordinates>");

    connect(_timer, &QTimer::timeout, this, &Game::gameTick);
    _timer->setInterval(config.timePerTick.count());
    _field = std::make_shared<Field>(config.fieldWidth, config.fieldHeight);
    _foodManager = std::make_unique<FoodManager>();
    _foodManager->setField(_field);
    _foodManager->setApplesCount(25);
    _foodManager->restoreApples();
}

void Game::startGame()
{
    _timer->start();
}

void Game::stopGame()
{
    _timer->stop();
}

void Game::addSnake(std::shared_ptr<Snake> snake)
{
    snake->setField(_field);
    _snakes[snake->id()] = snake;
}

void Game::removeSnake(int64_t id)
{
    auto snakeRecord = _snakes.find(id);
    if (snakeRecord == _snakes.end()) return;
    snakeRecord->second->leaveField();
    _snakes.erase(snakeRecord);
}

std::shared_ptr<const Field> Game::getField() const
{
    return _field;
}

void Game::gameTick()
{
//    qDebug() << "Game tick";
    _field->startRecordingChangedCells();
    for (auto & snakeRecord : _snakes)
        snakeRecord.second->moveHead();
    _foodManager->restoreApples();
    auto changed = _field->stopRecordingChangedCells();
    emit updated(changed);

    printf("\033[%ldA", _field->height());
    for (size_t i = 0; i < _field->height(); ++i)
    {
        for (size_t j = 0; j < _field->width(); ++j)
            switch (_field->get(j, i).type) {
            case CellType::Empty: printf("\u2591"); break;
            case CellType::Apple: printf("\033[1;33m\u25CF\033[0m"); break;
            case CellType::Snake: printf("\033[1;3%ldm\u2588\033[0m", _field->get(j, i).internalId); break;
            case CellType::SnakeHead: printf("\u2593"); break;
            }
        putchar('\n');
    }

}
