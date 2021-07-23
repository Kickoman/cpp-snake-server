#include "coordinates.h"

constexpr CoordType Coordinates::kInvalidValue;
constexpr CoordType Coordinates::kMaxPossibleValue;
constexpr std::array<CoordType, 4> Coordinates::kXShift;
constexpr std::array<CoordType, 4> Coordinates::kYShift;

Coordinates::Coordinates(size_t x, size_t y)
{
    assert(x < kMaxPossibleValue && y < kMaxPossibleValue);
    this->x = static_cast<CoordType>(x);
    this->y = static_cast<CoordType>(y);
}

Coordinates Coordinates::shift(Direction direction, CoordType distance) const
{
    return Coordinates(x + xShift(direction) * distance,
                       y + yShift(direction) * distance);
}

CoordinatesDiff Coordinates::diff(const Coordinates &other) const
{
    auto xdiff = other.x - this->x;
    auto ydiff = other.y - this->y;
    Direction xdir = xdiff > 0 ? Direction::East : Direction::West;
    Direction ydir = ydiff > 0 ? Direction::South : Direction::North;
    xdiff = abs(xdiff);
    ydiff = abs(ydiff);
    return CoordinatesDiff(xdiff, ydiff, xdir, ydir);
}

CoordinatesDiff::CoordinatesDiff(CoordType xdiff, CoordType ydiff, Direction xdir, Direction ydir)
    : xdiff(xdiff)
    , ydiff(ydiff)
    , xdir(xdir)
    , ydir(ydir)
{}
