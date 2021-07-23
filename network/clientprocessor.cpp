#include "clientprocessor.h"
#include <QDebug>
#include "headers.h"
#include <QDataStream>

ClientProcessor::ClientProcessor(QTcpSocket *socket, Game *game, QObject *parent)
    : QObject(parent)
    , game(game)
{
    this->socket = socket;
    if (socket->state() != QAbstractSocket::ConnectedState)
        qDebug() << "Socket is not connected!";
    connect(socket, &QTcpSocket::errorOccurred, this, &ClientProcessor::processConnectionIssue);
    connect(socket, &QTcpSocket::disconnected, this, &ClientProcessor::processConnectionIssue);
    connect(game, &Game::updated, this, &ClientProcessor::processGameUpdate);

    // Send initial state
    auto cells = game->getField()->getNonEmptyCells();
    QByteArray data;
    int32_t type = static_cast<int32_t>(MessageType::FullState);
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << type;
    out << cells.size();
    for (const auto & cell : cells)
    {
        auto content = game->getField()->get(cell);
        out << cell.x << cell.y
            << static_cast<int>(content.type)
            << content.internalId;
    }
    socket->write(data);
}

void ClientProcessor::processConnectionIssue()
{
    emit dead();
}

void ClientProcessor::processGameUpdate(std::set<Coordinates> coordinates)
{
    const int32_t type = static_cast<int32_t>(MessageType::Update);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << type;
    out << coordinates.size();
    for (const auto & cell : coordinates)
    {
        auto content = game->getField()->get(cell);
        out << cell.x << cell.y
            << static_cast<int>(content.type)
            << content.internalId;
    }
    socket->write(data);
}
