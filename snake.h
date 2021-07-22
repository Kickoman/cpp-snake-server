#ifndef SNAKE_H
#define SNAKE_H

#include <array>
#include <deque>
#include <mutex>
#include <atomic>
#include "coordinates.h"
#include "field.h"

class Snake
{
public:
    Snake();

    void setField(std::shared_ptr<Field> field);
    std::shared_ptr<const Field> getField() const;
    void leaveField();

    std::deque<Coordinates> cells() const;
    Coordinates headPosition() const;
    void moveHead();
    void setDirection(Direction dir);

    int64_t id() const;
    size_t length() const;

private:
    static constexpr int kDefaultSnakeSize = 3;

    static std::atomic<int64_t> GlobalSnakeCounter;
    const int64_t kSnakeId;

    std::mutex _directionLocker;
    Direction _headDirection = Direction::North;
    Direction _lastMoveDirection = Direction::North;
    std::deque<Coordinates> _snakeCells;
    std::shared_ptr<Field> _field;

    int _satiety = 0;
};

#endif // SNAKE_H
