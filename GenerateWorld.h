#ifndef GENERATEWORLD_H
#define GENERATEWORLD_H
#include <list>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include "Unit.h"
#include "Location.h"
#include "Move.h"

class GenerateWorld
{
public:
    int width = 40;  //21
    int heigth = 40; //13
    int l_width = 38;
    int l_heigth = 38;
    long int secounds;
    int percentOfWalls = 40;
    int percentOfForests = 50;
    int numberOfConnectedPixels;
    int numberOfLocations = 20;
    int numberOfLocationsToSearch;
    std::list<Location> locationList;
    std::list<Unit> unitList;
    std::list<sf::CircleShape> mapList;
    std::list<AvailableFields> fields;
    std::list<AvailableFields> road;
    std::list<OpenFields> openF;
    std::list<Location> locationCopy;
    int *connectedCities;
    int elementNumber;
    int *map = new int[width * heigth];
    int *l_map = new int[(width - 2) * (heigth - 2)];

    void placeWalls();
    void showMap();
    void checkWall();
    void checkMap();
    void floodFill(int x, int y, int oldColor, int newColor);
    void cityGenerator(int numberOfPlayers);
    void checkMap2();
    void placeWater(int x, int y);
    void placeForest();
    void checkForest();
    void placeRoad();
    void searchRoad();
    void setRoadTexture(sf::RenderTexture &renderTexture, sf::Texture &worldTexture);
    int modusIdentifier(int x, int y, int x_next, int y_next);
    sf::Vector2f converter(sf::Vector2f v);
    std::tuple<int *, int *, int *, int, int, int, int, int *> worldG(std::string save_game, sf::Font &font, sf::RenderTexture &renderTexture, sf::Texture &unitsTexture, sf::Texture &locationsTexture, sf::Texture &worldTexture, int numberOfPlayers);
};
#endif