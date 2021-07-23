#include "server.h"
#include <cassert>

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection,
            this, &Server::processNewConnections);
}

void Server::startServer(quint16 port)
{
    QMetaObject::invokeMethod(this, "startServerPrivate", Qt::QueuedConnection,
                              Q_ARG(quint16, port));
}

void Server::setGame(Game *game)
{
    this->game = game;
}

void Server::startServerPrivate(quint16 port)
{
    server->listen(QHostAddress::Any, port);
}

void Server::processNewConnections()
{
    while (server->hasPendingConnections())
    {
        auto socket = server->nextPendingConnection();
        auto client = new ClientProcessor(socket, game);
        connect(client, &ClientProcessor::dead,
                this, &Server::processClientDeath);
        clients.append(client);
    }
}

void Server::processClientDeath()
{
    auto client = dynamic_cast<ClientProcessor*>(sender());
    assert(client);
    clients.removeAll(client);
    client->deleteLater();
}
