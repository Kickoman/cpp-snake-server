#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include "game.h"
#include "basicaisnake.h"

int main(int argc, char *argv[])
{
//    std::ios_base::sync_with_stdio(false);
    QCoreApplication a(argc, argv);
    Game::Config config;
    config.fieldWidth = 170;
    config.fieldHeight = 50;
    config.timePerTick = std::chrono::milliseconds(40);

    auto *game = new Game(config);

    auto *ai1 = new BasicAISnake();
    auto *ai2 = new BasicAISnake();
    auto *ai3 = new BasicAISnake();

    ai1->startGame(game);
    ai2->startGame(game);
    ai3->startGame(game);

    game->startGame();

    return a.exec();
}
