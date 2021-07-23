#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "game.h"
#include "basicaisnake.h"

int main(int argc, char *argv[])
{
//    std::ios_base::sync_with_stdio(false);
    QCoreApplication a(argc, argv);
    Game::Config config;
    config.fieldWidth = 150;
    config.fieldHeight = 36;
    config.timePerTick = std::chrono::milliseconds(40);

    auto *game = new Game(config);
    auto *ai = new BasicAISnake();
    auto snake = std::make_shared<Snake>();
    ai->setSnake(snake);
    game->addSnake(snake);
    ai->start(game);

    auto *ai2 = new BasicAISnake();
    auto snake2 = std::make_shared<Snake>();
    ai2->setSnake(snake2);
    game->addSnake(snake2);
    ai2->start(game);

    game->startGame();

    return a.exec();
}
