#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QThread>
#include "game.h"
#include "basicaisnake.h"
#include "network/server.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Snake server");
    QCoreApplication::setApplicationVersion("0.2");

    QCommandLineParser parser;
    parser.setApplicationDescription("Basic AI Snake server");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption widthOpt(QStringList() << "w" << "width",
        "Field width", "width", "170");
    parser.addOption(widthOpt);

    QCommandLineOption heightOpt(QStringList() << "H" << "height",
        "Field height", "height", "50");
    parser.addOption(heightOpt);

    QCommandLineOption tickOpt(QStringList() << "t" << "time",
        "Time per tick in ms", "time", "40");
    parser.addOption(tickOpt);

    QCommandLineOption applesOpt(QStringList() << "a" << "apples",
        "Amount of apples on the field", "apples", "10");
    parser.addOption(applesOpt);

    QCommandLineOption botOpt(QStringList() << "b" << "bots",
        "Number of bot-snakes", "bots", "3");
    parser.addOption(botOpt);

    QCommandLineOption portOpt(QStringList() << "p" << "port",
        "Port for server to run", "port", "9898");

    parser.process(a);

    Game::Config config;
    config.fieldWidth = parser.value(widthOpt).toULongLong();
    config.fieldHeight = parser.value(heightOpt).toULongLong();
    config.timePerTick = std::chrono::milliseconds(parser.value(tickOpt).toInt());
    config.applesCount = parser.value(applesOpt).toUInt();

    unsigned int numberOfBots = parser.value(botOpt).toUInt();

    qDebug() << "Starting game";
    qDebug() << "Field size:" << config.fieldWidth << "x" << config.fieldHeight;
    qDebug() << "Time per tick:" << config.timePerTick.count();
    qDebug() << "Apples count:" << config.applesCount;
    qDebug() << "Number of bots:" << numberOfBots;

    auto *game = new Game(config);

    for (unsigned int i = 0; i < numberOfBots; ++i)
    {
        qDebug() << "Add bot" << i + 1;
        auto *ai = new BasicAISnake();
        ai->startGame(game);
    }

    quint16 port = 9898;
    if (parser.isSet(portOpt))
    {
        bool ok = false;
        port = parser.value(portOpt).toUShort(&ok);
        if (!ok)
        {
            qDebug() << "Can't parse port" << parser.value(portOpt);
            qDebug() << "Using default value.";
            port = 9898;
        }
    }

    qDebug() << "Starting server on port" << port;
    game->startGame();

    Server *server = new Server();
    QThread *serverThread = new QThread();
    server->moveToThread(serverThread);
    serverThread->start();
    server->setGame(game);
    server->startServer(port);

    return a.exec();
}
