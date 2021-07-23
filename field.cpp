#include "field.h"
#include <random>
#include <cassert>
#include <QTimer>
#include <QDebug>
#include "snake.h"

#ifdef NDEBUG
#define SEED    static_cast<std::default_random_engine::result_type>(time(nullptr))
#else
#define SEED    std::default_random_engine::default_seed
#endif //NDEBUG

Field::Field(size_t width, size_t height)
    : _width(width)
    , _height(height)
{
    _field.resize(height, std::vector<Cell>(width));
}

size_t Field::width() const
{
    return _width;
}

size_t Field::height() const
{
    return _height;
}

const Cell &Field::get(size_t x, size_t y) const { return _field[y][x]; }

const Cell &Field::get(Coordinates coordinates) const { return get(size_t(coordinates.x), size_t(coordinates.y)); }

void Field::set(size_t x, size_t y, Cell cell)
{
    if (_field[y][x] != cell)
    {
        Coordinates coordinates(x, y);
        if (_recordingChanges) _changedCells.insert(coordinates);
        if (cell.isEmpty())
            _nonEmptyCells.erase(coordinates);
        else
            _nonEmptyCells.insert(coordinates);
    }

    _field[y][x] = cell;
}

void Field::set(Coordinates coordinates, Cell cell)
{
    set(size_t(coordinates.x), size_t(coordinates.y), cell);
}

Coordinates Field::getFreeCells(size_t count, Orientation orientation)
{
    const Direction dir = orientation == Orientation::Horizontal ? Direction::East : Direction::South;

    static std::default_random_engine engine(SEED);
    std::uniform_int_distribution<int> xDistr(0, static_cast<int>(_width) - 1);
    std::uniform_int_distribution<int> yDistr(0, static_cast<int>(_height) - 1);

    int maxTries = 100;
    while (maxTries--)
    {
        Coordinates coordinates;
        coordinates.x = xDistr(engine);
        coordinates.y = yDistr(engine);

        // check
        bool onlyFree = true;
        for (int i = 0; i < int(count) && onlyFree; ++i)
        {
            auto currentCoordinates = coordinates.shift(dir, i);
            onlyFree &= validatePosition(currentCoordinates)
                        && get(currentCoordinates).isEmpty();
        }
        if (onlyFree)
            return coordinates;
    }
    return {};
}

Coordinates Field::nextPosition(Coordinates currentPosition, Direction direction) const
{
    auto nxt = currentPosition.shift(direction, 1);
    return validatePosition(nxt) ? nxt : currentPosition;
}

unsigned int Field::applesCount() const
{
    return _applesCount;
}

void Field::placeApple(Coordinates coordinates)
{
    assert(get(coordinates).isEmpty());
    set(coordinates, CellType::Apple);
    ++_applesCount;
}

void Field::consumeApple()
{
    assert(_applesCount > 0);
    --_applesCount;
}

void Field::startRecordingChangedCells()
{
    if (_recordingChanges) return;
    _recordingChanges = true;
    _changedCells.clear();
}

std::set<Coordinates> Field::stopRecordingChangedCells()
{
    _recordingChanges = false;
    return _changedCells;
}

std::set<Coordinates> Field::getNonEmptyCells() const
{
    return _nonEmptyCells;
}

bool Field::validatePosition(Coordinates pos) const
{
    return pos.x >= 0 && pos.x < static_cast<int>(_width) && pos.y >= 0 && pos.y < static_cast<int>(_height);
}

