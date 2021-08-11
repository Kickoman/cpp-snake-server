#include "gamemessage.h"
#include <QDataStream>
#include <QByteArray>

void GameMessage::serialize(const GameMessage &msg, QIODevice *device)
{
    QDataStream out(device);
    out.setVersion(QDataStream::Qt_5_0);

    out << static_cast<qint32>(msg.type);
    out << msg.fieldWidth << msg.fieldHeight;
    out << static_cast<qint32>(msg.cells.size());
    for (const auto &cell : msg.cells)
    {
        out << cell.coordX << cell.coordY << cell.type << cell.internalId;
    }
}

GameMessage GameMessage::deserialize(QIODevice *data, bool *ok)
{
    bool localOk = false;
    if (!ok) ok = &localOk;

    GameMessage msg;
    QDataStream in(data);
    in.startTransaction();

    qint32 rawType = 1;
    qint32 cellsCount = 0;
    in >> rawType;
    msg.type = static_cast<MessageType>(rawType);
    in >> msg.fieldWidth >> msg.fieldHeight;
    in >> cellsCount;
    for (qint32 i = 0; i < cellsCount; ++i)
    {
        qint64 x = 0, y = 0, internalId = -1;
        int type = 0;
        in >> x >> y >> type >> internalId;
        msg.cells.push_back({x, y, type, internalId});
    }
    *ok = in.commitTransaction();
    return msg;
}
