#ifndef FOODMANAGER_H
#define FOODMANAGER_H

#include "field.h"

class FoodManager
{
public:
    FoodManager();

    void setApplesCount(unsigned int apples);
    void setField(std::shared_ptr<Field> field);

    void restoreApples();

private:
    unsigned int _requiredApplesCount = 2;
    std::shared_ptr<Field> _field;
};

#endif // FOODMANAGER_H
