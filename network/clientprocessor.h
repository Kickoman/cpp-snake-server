#ifndef CLIENTPROCESSOR_H
#define CLIENTPROCESSOR_H

#include <QObject>
#include <QTcpSocket>
#include "game.h"

class ClientProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ClientProcessor(QTcpSocket * socket,
                             Game * game,
                             QObject *parent = nullptr);

signals:
    void dead();

private slots:
    void processConnectionIssue();
    void processGameUpdate(std::set<Coordinates> coordinates);

private:
    QTcpSocket *socket;
    Game *game;
};

#endif // CLIENTPROCESSOR_H
