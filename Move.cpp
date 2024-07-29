#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <tuple>
#include "Move.h"
#include "Unit.h"

std::tuple<std::list<OpenFields>, std::list<AvailableFields>> Move::movement(std::list<OpenFields> openF, std::list<AvailableFields> fields, int *worldArray, std::list<Unit> unitList, int columns, int currentPlayer, bool ownUnits)
{
    OpenFields firstElement = openF.front();
    if (firstElement.movingPoints > 0)
    {
        int x = firstElement.x;
        int y = firstElement.y;
        int new_x;
        int new_y;
        int new_mPoints = (firstElement.movingPoints) - 1;
        int x_p = x;
        int y_p = y;

        for (int i = 0; i < 6; i++)
        {
            if (y % 2 == 0)
            {
                switch (i)
                {
                case 0:
                    new_x = x - 1;
                    new_y = y - 1;
                    break;
                case 1:
                    new_x = x;
                    new_y = y - 1;
                    break;
                case 2:
                    new_x = x + 1;
                    new_y = y;
                    break;
                case 3:
                    new_x = x;
                    new_y = y + 1;
                    break;
                case 4:
                    new_x = x - 1;
                    new_y = y + 1;
                    break;
                case 5:
                    new_x = x - 1;
                    new_y = y;
                    break;
                }
            }
            else
            {
                switch (i)
                {
                case 0:
                    new_x = x;
                    new_y = y - 1;
                    break;
                case 1:
                    new_x = x + 1;
                    new_y = y - 1;
                    break;
                case 2:
                    new_x = x + 1;
                    new_y = y;
                    break;
                case 3:
                    new_x = x + 1;
                    new_y = y + 1;
                    break;
                case 4:
                    new_x = x;
                    new_y = y + 1;
                    break;
                case 5:
                    new_x = x - 1;
                    new_y = y;
                    break;
                }
            }
            bool freeField = true;
            if (worldArray[new_y * columns + new_x] == 1 || worldArray[new_y * columns + new_x] == 3)
            {
                bool exist = false;
                bool e = true;
                for (Unit u : unitList)
                {
                    if (u.x_Coordinate == new_x && u.y_Coordinate == new_y && u.owner != currentPlayer)
                    {
                        e = false;
                        //break;
                    }
                    if (u.x_Coordinate == new_x && u.y_Coordinate == new_y && u.owner == currentPlayer && ownUnits)
                    {
                        freeField = false;
                    }
                }
                if (e)
                {
                    if (!fields.empty())
                    {
                        for (AvailableFields f : fields)
                        {
                            if (f.x == new_x && f.y == new_y)
                            {
                                exist = true;
                                break;
                            }
                        }
                    }
                    if (!exist)
                    {
                        AvailableFields new_f;
                        new_f.movingPoints = new_mPoints;
                        new_f.x = new_x;
                        new_f.y = new_y;
                        new_f.x_predecessor = x_p;
                        new_f.y_predecessor = y_p;
                        if (freeField)
                        {
                            fields.push_back(new_f);
                        }
                        OpenFields openF1;
                        openF1.movingPoints = new_mPoints;
                        openF1.x = new_x;
                        openF1.y = new_y;
                        openF.push_back(openF1);
                    }
                }
            }
        }
    }

    openF.pop_front();

    if (!openF.empty())
    {
        Move iterative_m;
        auto result = iterative_m.movement(openF, fields, worldArray, unitList, columns, currentPlayer, ownUnits);
        openF = std::get<0>(result);
        fields = std::get<1>(result);
    }

    return std::make_tuple(openF, fields);
};
std::tuple<std::list<OpenFields>, std::list<AvailableFields>> Move::movementAttack(std::list<OpenFields> openF, std::list<AvailableFields> fields, int *worldArray, std::list<Unit> unitList, int columns)
{
    OpenFields firstElement = openF.front();
    if (firstElement.movingPoints > 0)
    {
        int x = firstElement.x;
        int y = firstElement.y;
        int new_x;
        int new_y;
        int new_mPoints = (firstElement.movingPoints) - 1;
        int x_p = x;
        int y_p = y;

        for (int i = 0; i < 6; i++)
        {
            if (y % 2 == 0)
            {
                switch (i)
                {
                case 0:
                    new_x = x - 1;
                    new_y = y - 1;
                    break;
                case 1:
                    new_x = x;
                    new_y = y - 1;
                    break;
                case 2:
                    new_x = x + 1;
                    new_y = y;
                    break;
                case 3:
                    new_x = x;
                    new_y = y + 1;
                    break;
                case 4:
                    new_x = x - 1;
                    new_y = y + 1;
                    break;
                case 5:
                    new_x = x - 1;
                    new_y = y;
                    break;
                }
            }
            else
            {
                switch (i)
                {
                case 0:
                    new_x = x;
                    new_y = y - 1;
                    break;
                case 1:
                    new_x = x + 1;
                    new_y = y - 1;
                    break;
                case 2:
                    new_x = x + 1;
                    new_y = y;
                    break;
                case 3:
                    new_x = x + 1;
                    new_y = y + 1;
                    break;
                case 4:
                    new_x = x;
                    new_y = y + 1;
                    break;
                case 5:
                    new_x = x - 1;
                    new_y = y;
                    break;
                }
            }
            bool exist = false;

            if (!fields.empty())
            {
                for (AvailableFields f : fields)
                {
                    if (f.x == new_x && f.y == new_y)
                    {
                        exist = true;
                        break;
                    }
                }
            }
            if (!exist)
            {
                AvailableFields new_f;
                new_f.movingPoints = new_mPoints;
                new_f.x = new_x;
                new_f.y = new_y;
                new_f.x_predecessor = x_p;
                new_f.y_predecessor = y_p;
                fields.push_back(new_f);
                OpenFields openF1;
                openF1.movingPoints = new_mPoints;
                openF1.x = new_x;
                openF1.y = new_y;
                openF.push_back(openF1);
            }
        }
    }

    openF.pop_front();

    if (!openF.empty())
    {
        Move iterative_m;
        auto result = iterative_m.movementAttack(openF, fields, worldArray, unitList, columns);
        openF = std::get<0>(result);
        fields = std::get<1>(result);
    }

    return std::make_tuple(openF, fields);
};

std::tuple<float, float> Move::determinePosition(sf::Vector2i mousePosition)
{
    int X = mousePosition.x - 7;
    int Y = mousePosition.y;
    int shortX = 45;
    int realShortX = 43;
    int longX = 90;
    int shortY = 29;
    int longY = 50;
    int height = 0;
    double line;
    int modus = 100;

    while (true)
    {
        if (Y >= shortY)
        {
            Y -= shortY;
        }
        else
        {
            modus = 0;
            break;
        }
        if (Y >= longY)
        {
            Y -= longY;
            height += 1;
        }
        else
        {
            modus = 1;
            break;
        }
        if (Y >= shortY)
        {
            Y -= shortY;
        }
        else
        {
            modus = 2;
            break;
        }
        if (Y >= longY)
        {
            Y -= longY;
            height += 1;
        }
        else
        {
            modus = 3;
            break;
        }
    }
    int wide;
    if (modus == 3)
    {
        wide = (int)floor((X - shortX) / longX);
    }
    else
    {
        wide = (int)floor(X / longX);
    }

    int spezialWide = (int)X % longX;

    if (modus == 0)
    {
        if (spezialWide < realShortX)
        { //left
            line = ((double)(realShortX - spezialWide)) * (0.581);
            if (line > Y)
            { //up
                wide -= 1;
                height -= 1;
            }
            else
            { //down
            }
        }
        else
        { //right
            line = ((double)(spezialWide - realShortX)) * (0.581);
            if (line > Y)
            { //up
                height -= 1;
            }
            else
            { //down
            }
        }
    }

    if (modus == 1)
    {
    }

    if (modus == 2)
    {
        if (spezialWide < realShortX)
        { //left
            line = shortY - ((double)(realShortX - spezialWide)) * (0.581);
            if (line < Y)
            { //down
                wide -= 1;
            }
            else
            { //up
                height -= 1;
            }
        }
        else
        {
            line = shortY - ((double)(spezialWide - realShortX)) * (0.581);
            if (line < Y)
            {
            }
            else
            {
                height -= 1;
            }
        }
    }

    if (modus == 3)
    {
    }

    return std::make_tuple((float)wide, (float)height);
};

sf::Vector2f Move::converter(sf::Vector2f v)
{
    float Y = 4;
    float X = 0;
    if ((int)v.y % 2 == 0)
    {
        X = 0;
    }
    else
    {
        X = 45;
    }
    X += 90 * v.x;
    Y += 80 * v.y;
    return sf::Vector2f{X, Y};
};
