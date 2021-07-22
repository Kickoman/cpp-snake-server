#include "foodmanager.h"

FoodManager::FoodManager()
{

}

void FoodManager::setApplesCount(unsigned int apples)
{
    _requiredApplesCount = apples;
}

void FoodManager::setField(std::shared_ptr<Field> field)
{
    _field = std::move(field);
}

void FoodManager::restoreApples()
{
    if (!_field) return;
    while (_field->applesCount() < _requiredApplesCount)
    {
        auto freePlace = _field->getFreeCells(1, Orientation::Horizontal);
        _field->placeApple(freePlace);
    }
}
