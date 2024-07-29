#ifndef ARRAYINFORMATION_H
#define ARRAYINFORMATION_H

#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <string>
#include "Unit.h"
#include "Location.h"
#include "Move.h"

class ArrayInformation
{
public:
    std::tuple<int, int, int *> readFile(std::string filename);
    std::list<Unit> readUnitFile(std::string filename);
    int *readColorList();
    int *readUnitStats();
    std::list<Location> readLocationFile(std::string filename);
    void writeFile(std::string filename, int rows, int columns, int *array2D);
    void writeInformationFile(std::string filename, std::string currentGame);
    std::list<AvailableFields> ArrayInformation::readRoadFile(std::string filename);
    std::string readInformationFile(std::string filename);
    std::tuple<int, int, int *> readGameInformation(std::string filename);
    void writeSaveGame(std::string folder, std::list<Location> locationsList, std::list<Unit> unitList, int rows, int columns, int *array2D, int numberOfPlayers, int currentPlayer, int *income, std::list<AvailableFields> roadList);
    std::list<std::string> readCityNames();
};

#endif