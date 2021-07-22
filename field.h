#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QList>
#include <memory>
#include <set>
#include "coordinates.h"

class Snake;
class Apple;

enum class CellType : int
{
    Empty       = 0,
    Snake       = 1,
    Apple       = 2
};

struct Cell
{
    Cell(CellType type = CellType::Empty) : type(type) {}

    CellType type = CellType::Empty;

    bool isFree() const { return type != CellType::Snake; }
};

class Field
{
public:
    Field(size_t width, size_t height);

    size_t width() const;
    size_t height() const;

    const Cell &get(size_t x, size_t y) const;
    const Cell &get(Coordinates coordinates) const;
    void set(size_t x, size_t y, Cell cell);
    void set(Coordinates coordinates, Cell cell);

    /// \return The top left cell of required
    Coordinates getFreeCells(size_t count, Orientation orientation);
    Coordinates nextPosition(Coordinates currentPosition, Direction direction) const;

    unsigned int applesCount() const;
    void placeApple(Coordinates coordinates);
    void consumeApple();

    void startRecordingChangedCells();
    std::set<Coordinates> stopRecordingChangedCells();

    std::set<Coordinates> getNonEmptyCells() const;

private:
    bool validatePosition(Coordinates pos) const;

    size_t _width;
    size_t _height;

    std::vector<std::vector<Cell>> _field;
    unsigned int _applesCount = 0;

    bool _recordingChanges = false;
    std::set<Coordinates> _changedCells;
    std::set<Coordinates> _nonEmptyCells;
};

#endif // FIELD_H
