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
    auto *ai = new BasicAISnake();
    auto snake = std::make_shared<Snake>();
    ai->setSnake(snake);
    game->addSnake(snake);
    ai->start(game);

    auto *ai2 = new BasicAISnake();
    auto snake2 = std::make_shared<Snake>();
    ai2->setSnake(snake2);
//    game->addSnake(snake2);
//    ai2->start(game);

    auto *ai3 = new BasicAISnake();
    auto snake3 = std::make_shared<Snake>();
    ai3->setSnake(snake3);
//    game->addSnake(snake3);
//    ai3->start(game);

    game->startGame();


    QTimer::singleShot(3000, [&game, &snake2, &ai2]{
        game->addSnake(snake2);
        ai2->start(game);
    });
    QTimer::singleShot(6000, [&game, &snake3, &ai3]{
        game->addSnake(snake3);
        ai3->start(game);
    });

    return a.exec();
}
