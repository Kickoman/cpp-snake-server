#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "game.h"
#include "clientprocessor.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void startServer(quint16 port);
    void setGame(Game *game);

private slots:
    void startServerPrivate(quint16 port);
    void processNewConnections();
    void processClientDeath();

private:
    QTcpServer *server = nullptr;
    Game *game = nullptr;

    QList<ClientProcessor*> clients;
};

#endif // SERVER_H
