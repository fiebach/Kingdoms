#ifndef LOADGAME_H
#define LOADGAME_H

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <tuple>
#include <list>
#include <string>
#include "Unit.h"
#include "Location.h"
#include "Move.h"

class LoadGame
{
public:
    std::list<Unit> unitList;
    std::list<Location> locationsList;
    std::list<sf::CircleShape> mapList;
    std::list<int> mapListValue;
    std::list<AvailableFields> roadList;
    int columns;
    int rows;
    int *playerColors;
    int *unitStats;
    void setRoadTexture(sf::RenderTexture &renderTexture, sf::Texture &worldTexture);
    sf::Vector2f converter(sf::Vector2f v);
    int modusIdentifier(int x, int y, int x_next, int y_next);
    std::tuple<int *, int *, int *, int, int, int, int, int *> loadSaveGame(std::string save_game, sf::Font &font, sf::RenderTexture &renderTexture, sf::Texture &unitsTexture, sf::Texture &locationsTexture, sf::Texture &worldTexture);
};

#endif