#ifndef MOVE_H
#define MOVE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <tuple>
#include "Unit.h"

class OpenFields
{
public:
    int x;
    int y;
    int movingPoints;
};
class AvailableFields
{
public:
    int movingPoints;
    int x;
    int y;
    int x_predecessor;
    int y_predecessor;
    int x_next;
    int y_next;
};
class Move
{
public:
    std::tuple<std::list<OpenFields>, std::list<AvailableFields>> movement(std::list<OpenFields> openF, std::list<AvailableFields> fields, int *worldArray, std::list<Unit> unitList, int columns, int currentPlayer, bool ownUnits);
    std::tuple<std::list<OpenFields>, std::list<AvailableFields>> movementAttack(std::list<OpenFields> openF, std::list<AvailableFields> fields, int *worldArray, std::list<Unit> unitList, int columns);
    std::tuple<float, float> determinePosition(sf::Vector2i mousePosition);
    sf::Vector2f converter(sf::Vector2f v);
};

#endif