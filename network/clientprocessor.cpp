#include "clientprocessor.h"
#include <QDebug>
#include "gamemessage.h"
#include <QBuffer>

ClientProcessor::ClientProcessor(QTcpSocket *socket, Game *game, QObject *parent)
    : QObject(parent)
    , socket(socket)
    , game(game)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        qDebug() << "Socket is not connected!";

    connect(socket, &QTcpSocket::disconnected, this, &ClientProcessor::processConnectionIssue);
    connect(game, &Game::updated, this, &ClientProcessor::processGameUpdate);

    // Send initial state
    auto cells = game->getField()->getNonEmptyCells();
    GameMessage msg;
    msg.type = MessageType::FullState;
    msg.fieldWidth = qint64(game->getField()->width());
    msg.fieldHeight = qint64(game->getField()->height());
    for (const auto & cell : cells)
    {
        auto content = game->getField()->get(cell);
        msg.cells.push_back({cell.x, cell.y, static_cast<int>(content.type), content.internalId});
    }
    QByteArray data;
    QBuffer buf(&data);
    GameMessage::serialize(msg, &buf);
    socket->write(data);
}

void ClientProcessor::processConnectionIssue()
{
    emit dead();
}

void ClientProcessor::processGameUpdate(std::set<Coordinates> coordinates)
{
    GameMessage msg;
    msg.type = MessageType::Update;

    auto field = game->getField();
    for (const auto & cell : coordinates)
    {
        auto content = field->get(cell);
        msg.cells.push_back({cell.x, cell.y, static_cast<int>(content.type), content.internalId});
    }

    QByteArray data;
    QBuffer buf(&data);
    GameMessage::serialize(msg, &buf);
    socket->write(data);
}
