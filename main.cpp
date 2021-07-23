#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QThread>
#include "game.h"
#include "basicaisnake.h"
#include "network/server.h"

int main(int argc, char *argv[])
{
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

    Server *server = new Server();
    QThread *serverThread = new QThread();
    server->moveToThread(serverThread);
    serverThread->start();
    server->setGame(game);
    server->startServer(9898);

    return a.exec();
}
