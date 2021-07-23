#include "basicaisnake.h"
#include <queue>
#include <QDebug>
#include <cassert>

BasicAISnake::BasicAISnake(QObject *parent) : QObject(parent)
{}

void BasicAISnake::start(Game *game)
{
    size_t h = snake()->getField()->height();
    size_t w = snake()->getField()->width();

    field.resize(h, std::vector<CellType>(w));

    auto filled = snake()->getField()->getNonEmptyCells();
    for (const auto & coordinate: filled)
    {
        auto cell = snake()->getField()->get(coordinate);
        field[coordinate.y][coordinate.x] = cell.type;
    }

    head = snake()->headPosition();
    connect(game, &Game::updated, this, &BasicAISnake::processUpdate, Qt::QueuedConnection);
}

void BasicAISnake::processUpdate(std::set<Coordinates> updatedCells)
{
    for (const auto & cell : updatedCells)
        field[cell.y][cell.x] = snake()->getField()->get(cell).type;
    head = snake()->headPosition();


    static const auto bfs_shortest_path
        = [this](Coordinates start) -> std::vector<Coordinates> // path
    {
        using std::vector;
        using std::queue;
        vector<vector<int>> distance(field.size(), vector<int>(field.front().size()));
        for (auto & row : distance)
            std::fill(row.begin(), row.end(), INT_MAX);

        std::list<Coordinates> finishes;
        for (size_t i = 0; i < field.size(); ++i)
            for (size_t j = 0; j < field[i].size(); ++j)
                if (field[i][j] == CellType::Apple)
                    finishes.push_back(Coordinates{j, i});

//        qDebug() << "BASIC AI: Finish at " << finish.x << " " << finish.y;

        distance[start.y][start.x] = 0;

        queue<Coordinates> toCheck;
        toCheck.push(start);
        while (!toCheck.empty())
        {
            auto current = toCheck.front();
            auto currentDistance = distance[current.y][current.x];
            toCheck.pop();

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);
                Coordinates nxt = current.shift(dir, 1);

                if (snake()->getField()->validatePosition(nxt)
                    && !snake()->getField()->get(nxt).isSnake()
                    && currentDistance + 1 < distance[nxt.y][nxt.x])
                {
                    distance[nxt.y][nxt.x] = currentDistance + 1;
                    toCheck.push(nxt);
                }
            }
        }

        Coordinates finish{};
        for (const auto & f : finishes)
            if (finish.x == -1
                || distance[finish.y][finish.x] > distance[f.y][f.x])
                finish = f;

        if (finish.x == -1 ||
                distance[finish.y][finish.x] == INT_MAX) return vector<Coordinates>{};
//        qDebug() << "Check finished. Distance to finish: " << distance[finish.y][finish.x];

        vector<Coordinates> reversedPath;
        while (finish != start)
        {
            reversedPath.push_back(finish);
            auto currentDistance = distance[finish.y][finish.x];
//            qDebug() << "Calculating reversed path.";
//            qDebug() << "Current finish: " << finish.x << " " << finish.y;
//            qDebug() << "Current distance: " << distance[finish.y][finish.x];
            for (int i = 0; i < 4; ++i)
            {
                auto dir = static_cast<Direction>(i);
                auto neighbor = finish.shift(dir, 1);
                if (snake()->getField()->validatePosition(neighbor)
                        && distance[neighbor.y][neighbor.x] == currentDistance - 1)
                {
                    finish = neighbor;
                    break;
                }
            }
            assert(true);
        }
        return reversedPath;
    };

    auto path = bfs_shortest_path(head);
    Direction dir = Direction::North;
    if (!path.empty())
    {
        auto diff = head.diff(path.back());
        dir = diff.xdiff != 0 ? diff.xdir : diff.ydir;
    }

    snake()->setDirection(dir);

//    qDebug() << "Direction: " << static_cast<int>(dir) << "; size: " << snake()->length();
//    qDebug() << "Head: " << snake()->headPosition().x << snake()->headPosition().y;
}
