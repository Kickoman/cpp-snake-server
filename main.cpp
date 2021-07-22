#include <QCoreApplication>
#include <QDebug>
#include "game.h"
#include "basicaisnake.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Game::Config config;
    config.fieldWidth = 20;
    config.fieldHeight = 20;
    config.timePerTick = std::chrono::milliseconds(100);

    auto *game = new Game(config);
    auto *ai = new BasicAISnake();
    auto snake = std::make_shared<Snake>();
    ai->setSnake(snake);
    game->addSnake(snake);
    ai->start(game);
    game->startGame();

    return a.exec();
}
