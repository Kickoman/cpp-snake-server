#include "basicaisnake.h"
#include <queue>

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
    connect(game, &Game::updated, this, &BasicAISnake::processUpdate);
}

void BasicAISnake::processUpdate(std::set<Coordinates> updatedCells)
{
    for (const auto & cell : updatedCells)
        field[cell.y][cell.x] = snake()->getField()->get(cell).type;
    head = snake()->headPosition();

    static const auto bfs_shortest_path
        = [this](Coordinates start, Coordinates finish) -> std::vector<Coordinates> // path
    {
        static const int kUsedCell = -1;
        static const int kForbiddenCell = -2;
        static const int kUnusedCell = -3;

        std::vector<std::vector<int>> used(field.size(), std::vector<int>(field.front().size()));
        for (size_t i = 0; i < used.size(); ++i)
            for (size_t j = 0; j < used[i].size(); ++j)
                used[i][j] = field[i][j] == CellType::Empty ? kUnusedCell : kForbiddenCell;
        used[finish.y][finish.x] = kUnusedCell;
        used[start.y][start.x] = 0;

        std::queue<std::pair<Coordinates, int>> check;
        check.push(std::make_pair(start, 0));
        while (!check.empty())
        {
            auto current = check.front();
            check.pop();
        }
    };
}
