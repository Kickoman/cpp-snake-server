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
    _foodManager->setApplesCount(_config.applesCount);
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
    _field->startRecordingChangedCells();
    for (auto & snakeRecord : _snakes)
        snakeRecord.second->moveHead();
    _foodManager->restoreApples();
    auto changed = _field->stopRecordingChangedCells();
    emit updated(changed);
}
