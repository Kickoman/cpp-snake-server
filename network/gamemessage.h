#ifndef GAMEMESSAGE_H
#define GAMEMESSAGE_H

#include <QList>
#include "headers.h"

class QIODevice;

struct CellInfo
{
    qint64 coordX = 0;
    qint64 coordY = 0;
    int type = 0;
    qint64 internalId = -1;

    CellInfo(qint64 x, qint64 y, int t, qint64 id)
        : coordX(x), coordY(y), type(t), internalId(id) {}
};

struct GameMessage
{
    MessageType type;
    qint64 fieldWidth;
    qint64 fieldHeight;
    QList<CellInfo> cells;

    static void serialize(const GameMessage &msg, QIODevice *device);
    static GameMessage deserialize(QIODevice *data, bool *ok = nullptr);
};

#endif // GAMEMESSAGE_H
