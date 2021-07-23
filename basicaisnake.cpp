#include "basicaisnake.h"
#include <queue>
#include <QDebug>
#include <cassert>

BasicAISnake::BasicAISnake(QObject *parent) : QObject(parent)
{}

void BasicAISnake::processUpdate(std::set<Coordinates> updatedCells)
{
    for (const auto & cell : updatedCells)
        field[cell.uy()][cell.ux()] = remoteField->get(cell).type;
    head = snake()->headPosition();


    static const auto bfs_shortest_path
        = [this](Coordinates start) -> std::vector<Coordinates> // path
    {
        using std::vector;
        using std::queue;

        vector<vector<int>> distance(height, vector<int>(width, INT_MAX));

        std::list<Coordinates> finishes;
        for (size_t i = 0; i < field.size(); ++i)
            for (size_t j = 0; j < field[i].size(); ++j)
                if (field[i][j] == CellType::Apple)
                    finishes.push_back(Coordinates{j, i});

        distance[start.uy()][start.ux()] = 0;

        queue<Coordinates> toCheck;
        toCheck.push(start);
        while (!toCheck.empty())
        {
            auto current = toCheck.front();
            auto currentDistance = distance[current.uy()][current.ux()];
            toCheck.pop();

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);
                Coordinates nxt = current.shift(dir, 1);

                if (remoteField->validatePosition(nxt)
                    && !remoteField->get(nxt).isSnake()
                    && currentDistance + 1 < distance[nxt.uy()][nxt.ux()])
                {
                    distance[nxt.uy()][nxt.ux()] = currentDistance + 1;
                    toCheck.push(nxt);
                }
            }
        }

        Coordinates finish{};
        for (const auto & f : finishes)
            if (!finish.isValid() || distance[finish.uy()][finish.ux()] > distance[f.uy()][f.ux()])
                finish = f;

        if (!finish.isValid() || distance[finish.uy()][finish.ux()] == INT_MAX)
            return vector<Coordinates>{};

        vector<Coordinates> reversedPath;
        while (finish != start)
        {
            if (reversedPath.size() > 100)
            {
                qDebug() << "Shit!";
            }
            reversedPath.push_back(finish);
            auto currentDistance = distance[finish.uy()][finish.ux()];
            for (int i = 0; i < 4; ++i)
            {
                auto dir = static_cast<Direction>(i);
                auto neighbor = finish.shift(dir, 1);
                if (remoteField->validatePosition(neighbor)
                    && distance[neighbor.uy()][neighbor.ux()] == currentDistance - 1)
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
}

void BasicAISnake::initGame()
{
    remoteField = game()->getField();

    height = remoteField->height();
    width = remoteField->width();

    field.resize(height, std::vector<CellType>(width));

    auto filled = remoteField->getNonEmptyCells();
    for (const auto & coordinate: filled)
    {
        auto cell = remoteField->get(coordinate);
        field[coordinate.uy()][coordinate.ux()] = cell.type;
    }

    head = snake()->headPosition();
    connect(game(), &Game::updated, this, &BasicAISnake::processUpdate, Qt::QueuedConnection);
}
