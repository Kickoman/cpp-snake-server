#include "snake.h"
#include <random>

std::atomic<int64_t> Snake::GlobalSnakeCounter{1};

Snake::Snake()
    : kSnakeId(GlobalSnakeCounter.fetch_add(1))
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 4);
    Direction startDirection = static_cast<Direction>(distribution(generator));
    _headDirection = startDirection;
    _lastMoveDirection = startDirection;

    // Initialize initial cells
    Coordinates coord;
    coord.x = 0;
    coord.y = 0;
    Direction tailDirection = oppositeDirection(startDirection);
    for (int i = 0; i < kDefaultSnakeSize; ++i)
    {
        auto currentCoordinates = coord.shift(tailDirection, i);
        _snakeCells.push_back(currentCoordinates);
    }
}

void Snake::setField(std::shared_ptr<Field> field)
{
    auto orientation = directionOrientation(_headDirection);
    auto initialPosition = field->getFreeCells(_snakeCells.size(), orientation);

    auto diff = _snakeCells.front().diff(initialPosition);

    for (auto & cell : _snakeCells)
    {
        cell = cell.shift(diff.xdir, diff.xdiff);
        cell = cell.shift(diff.ydir, diff.ydiff);

        field->set(cell, {CellType::Snake});
    }

    _field = std::move(field);
}

std::shared_ptr<const Field> Snake::getField() const
{
    return _field;
}

void Snake::leaveField()
{
    for (auto & cell : _snakeCells)
    {
        _field->set(cell, {CellType::Empty});
    }
    _field.reset();
}

Coordinates Snake::headPosition() const
{
    return _snakeCells.front();
}

void Snake::moveHead()
{
    if (!_field) return;

    auto nextPosition = _field->nextPosition(headPosition(), _headDirection);
    auto cell = _field->get(nextPosition);
    if (cell.isFree())
    {
        // Move head
        _snakeCells.push_front(nextPosition);
        // Consume apple if exists
        if (cell.type == CellType::Apple)
        {
            _satiety++;
            _field->consumeApple();
        }
        _field->set(nextPosition, {CellType::Snake});
        _lastMoveDirection = _headDirection;
    }

    // Move tail if necessary
    if (_satiety)
        --_satiety;
    else
        if (_snakeCells.size() > kDefaultSnakeSize)
        {
            auto coordinates = _snakeCells.back();
            _field->set(coordinates, {CellType::Empty});
            _snakeCells.pop_back();
    }
}

void Snake::setDirection(Direction dir)
{
    if (oppositeDirection(dir) == _lastMoveDirection) return;
    _headDirection = dir;
}

int64_t Snake::id() const
{
    return kSnakeId;
}

size_t Snake::length() const
{
    return _snakeCells.size();
}
