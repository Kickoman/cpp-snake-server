#ifndef COORDINATES_H
#define COORDINATES_H

#include <array>

enum class Direction : size_t
{
    North = 0,
    West  = 1,
    South = 2,
    East  = 3
};
inline Direction oppositeDirection(Direction dir) { return static_cast<Direction>(static_cast<size_t>(dir) ^ 2); }

enum class Orientation : int
{
    Horizontal,
    Vertical
};
inline Orientation directionOrientation(Direction dir)
{
    return static_cast<size_t>(dir) % 2 == 0 ? Orientation::Horizontal : Orientation::Vertical;
}

struct CoordinatesDiff
{
    int xdiff = 0;
    int ydiff = 0;
    Direction xdir = Direction::East;
    Direction ydir = Direction::South;
};

struct Coordinates
{    
    static constexpr std::array<int, 4> kXShift = {0, -1, 0, 1};
    static constexpr std::array<int, 4> kYShift = {-1, 0, 1, 0};
    static inline int xShift(Direction direction) { return kXShift[static_cast<size_t>(direction)]; }
    static inline int yShift(Direction direction) { return kYShift[static_cast<size_t>(direction)]; }

    int x = -1;   // horizontal
    int y = -1;   // vertical

    Coordinates() = default;
    Coordinates(int x, int y) : x(x), y(y) {}
    Coordinates(size_t x, size_t y) : x(static_cast<int>(x)), y(static_cast<int>(y)) {}

    bool operator<(const Coordinates &other) const { return x == other.x ? y < other.y : x < other.x; }
    bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Coordinates &other) const { return !(*this == other); }

    Coordinates shift(Direction direction, int distance) const
    {
        Coordinates c;
        c.x = x + xShift(direction) * distance;
        c.y = y + yShift(direction) * distance;
        return c;
    }

    CoordinatesDiff diff(const Coordinates &other) const
    {
        auto xdiff = other.x - this->x;
        auto ydiff = other.y - this->y;
        Direction xdir = xdiff > 0 ? Direction::East : Direction::West;
        Direction ydir = ydiff > 0 ? Direction::South : Direction::North;
        xdiff = abs(xdiff);
        ydiff = abs(ydiff);

        CoordinatesDiff diff;
        diff.xdiff = xdiff;
        diff.ydiff = ydiff;
        diff.xdir = xdir;
        diff.ydir = ydir;
        return diff;
    }
};

#endif // COORDINATES_H
