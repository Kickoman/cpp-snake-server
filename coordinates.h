#ifndef COORDINATES_H
#define COORDINATES_H

#include <array>
#include <cassert>

enum class Direction : size_t
{
    North = 0,      ///< Negative Y
    West  = 1,      ///< Positive X
    South = 2,      ///< Positive Y
    East  = 3       ///< Negative X
};

inline Direction oppositeDirection(Direction dir){ return static_cast<Direction>(static_cast<size_t>(dir) ^ 2); }

enum class Orientation : int
{
    Horizontal,
    Vertical
};

inline Orientation directionOrientation(Direction dir)
{
    return static_cast<size_t>(dir) % 2 == 0 ? Orientation::Vertical : Orientation::Horizontal;
}

using CoordType = int64_t;

struct CoordinatesDiff
{
    CoordType xdiff = 0;
    CoordType ydiff = 0;
    Direction xdir = Direction::East;
    Direction ydir = Direction::South;

    CoordinatesDiff() = default;
    CoordinatesDiff(CoordType xdiff, CoordType ydiff, Direction xdir, Direction ydir);
};

struct Coordinates
{
    static constexpr CoordType kInvalidValue = std::numeric_limits<CoordType>::max();
    static constexpr CoordType kMaxPossibleValue = kInvalidValue - 1;

    static constexpr std::array<CoordType, 4> kXShift = {0, -1, 0, 1};
    static constexpr std::array<CoordType, 4> kYShift = {-1, 0, 1, 0};

    static inline CoordType xShift(Direction direction) { return kXShift[static_cast<size_t>(direction)]; }
    static inline CoordType yShift(Direction direction) { return kYShift[static_cast<size_t>(direction)]; }

    CoordType x = kInvalidValue;   // horizontal
    CoordType y = kInvalidValue;   // vertical

    Coordinates() = default;
    Coordinates(CoordType x, CoordType y) : x(x), y(y) {}
    Coordinates(size_t x, size_t y);

    size_t ux() const { assert(x >= 0); return static_cast<size_t>(x); }
    size_t uy() const { assert(y >= 0); return static_cast<size_t>(y); }

    bool operator<(const Coordinates &other)  const { return x == other.x ? y < other.y : x < other.x; }
    bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Coordinates &other) const { return !(*this == other); }

    bool isValid() const { return x != kInvalidValue && y != kInvalidValue; }

    Coordinates shift(Direction direction, CoordType distance) const;
    CoordinatesDiff diff(const Coordinates &other) const;
};

#endif // COORDINATES_H
