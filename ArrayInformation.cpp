#include <iostream>
#include <fstream>
#include <tuple>
#include <list>
#include <string>
#include "ArrayInformation.h"
#include "Unit.h"
#include "Location.h"
#include "Move.h"
#include <direct.h>
using namespace std;

std::list<Unit> ArrayInformation::readUnitFile(std::string filename)
{
    std::ifstream fin(filename.c_str());
    int num_points;
    std::list<Unit> unitsList;
    fin >> num_points;
    int numberOfUnits = num_points;

    for (int i = 0; i < numberOfUnits; i++)
    {
        Unit u;
        fin >> u.x_Coordinate;
        fin >> u.y_Coordinate;
        fin >> u.owner;
        fin >> u.type;
        fin >> u.healthPoints;
        fin >> u.maxhealthPoints;
        fin >> u.movePoints;
        fin >> u.maxmovePoints;
        fin >> u.attacks;
        fin >> u.maxAttacks;
        unitsList.push_back(u);
    }

    fin.close();
    return unitsList;
};

int *ArrayInformation::readColorList()
{
    std::string filename = "../../save_game/playerColors.txt";
    std::ifstream fin(filename.c_str());
    int *array2D = new int[40];
    for (int i = 0; i < 10; i++)
    {
        fin >> array2D[i * 4];
        fin >> array2D[i * 4 + 1];
        fin >> array2D[i * 4 + 2];
        array2D[i * 4 + 3] = 255;
    }
    fin.close();
    return array2D;
};

int *ArrayInformation::readUnitStats()
{
    std::string filename = "../../save_game/unitStats.txt";
    std::ifstream fin(filename.c_str());
    int *array2D = new int[200];
    int notimportant;
    for (int i = 0; i < 10; i++)
    {
        fin >> notimportant;
        for (int j = 0; j < 10; j++)
        {
            fin >> array2D[i * 20 + j * 2];
            fin >> array2D[i * 20 + j * 2 + 1];
        }
    }
    fin.close();
    return array2D;
};

std::tuple<int, int, int *> ArrayInformation::readGameInformation(std::string filename)
{
    std::ifstream fin(filename.c_str());
    int numberOfPlayers;
    fin >> numberOfPlayers;
    int currentPlayer;
    fin >> currentPlayer;
    int num_points;
    int *incomeOfPlayer = new int[numberOfPlayers];
    for (int i = 0; i < numberOfPlayers; i++)
    {
        fin >> num_points;
        incomeOfPlayer[i] = num_points;
    }
    fin.close();
    return std::make_tuple(numberOfPlayers, currentPlayer, incomeOfPlayer);
};

std::tuple<int, int, int *> ArrayInformation::readFile(std::string filename)
{
    std::ifstream fin(filename.c_str());

    int num_points;

    fin >> num_points;
    int rows = num_points;

    fin >> num_points;
    int columns = num_points;

    int *array2D = new int[rows * columns];

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            fin >> num_points;
            array2D[row * columns + column] = num_points;
        }
    }
    fin.close();
    return std::make_tuple(rows, columns, array2D);
};

std::list<AvailableFields> ArrayInformation::readRoadFile(std::string filename)
{
    std::ifstream fin(filename.c_str());
    std::list<AvailableFields> roadList;
    AvailableFields a1;
    int num_points;
    fin >> num_points;
    for (int row = 0; row < num_points; row++)
    {
        fin >> a1.x;
        fin >> a1.y;
        roadList.push_back(a1);
    }
    fin.close();
    return roadList;
};

std::list<Location> ArrayInformation::readLocationFile(std::string filename)
{
    std::ifstream fin(filename.c_str());
    int num_points;
    std::list<Location> locationsList;
    fin >> num_points;
    int numberOfLocations = num_points;

    for (int i = 0; i < numberOfLocations; i++)
    {
        Location l;
        fin >> l.x;
        fin >> l.y;
        fin >> l.name;
        fin >> l.owner;
        fin >> l.type;
        fin >> l.income;
        fin >> l.attack;
        fin >> l.defense;
        fin >> l.healthPoints;
        fin >> l.maxHealthPoints;
        locationsList.push_back(l);
    }

    fin.close();
    return locationsList;
};

std::string ArrayInformation::readInformationFile(std::string filename)
{
    std::ifstream fin(filename.c_str());
    std::string currentGame;
    fin >> currentGame;
    fin.close();
    return currentGame;
};

void ArrayInformation::writeFile(std::string filename, int rows, int columns, int *array2D)
{
    std::ofstream fout(filename.c_str(), std::ios::out);
    fout << rows << std::endl;
    fout << columns << std::endl;
    std::string s;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            s += std::to_string(array2D[i * columns + j]) + " ";
        }
        fout << s << std::endl;
        s = "";
    }

    fout.close();
};

void ArrayInformation::writeInformationFile(std::string filename, std::string currentGame)
{
    // Open the file for writing
    std::ofstream fout(filename.c_str(), std::ios::out);

    // Write the title and number of points
    fout << currentGame << std::endl;
    fout.close();
};

void ArrayInformation::writeSaveGame(std::string folder, std::list<Location> locationsList, std::list<Unit> unitList, int rows, int columns, int *array2D, int numberOfPlayers, int currentPlayer, int *income, std::list<AvailableFields> roadList)
{
    std::string folderName = "../../save_game/" + folder;
    std::string filename;
    std::string input;
    const char *c = folderName.c_str();
    _mkdir(c);
    filename = folderName + "/locations.txt";
    std::ofstream fout(filename.c_str(), std::ios::out);

    int numberOfLocations = (int)locationsList.size();
    fout << numberOfLocations << std::endl;
    for (Location l : locationsList)
    {
        input = std::to_string(l.x) + " " + std::to_string(l.y) + " " + l.name + " " + std::to_string(l.owner) + " " + std::to_string(l.type) + " " + std::to_string(l.income) + " " + std::to_string(l.attack) + " " + std::to_string(l.defense) + " " + std::to_string(l.healthPoints) + " " + std::to_string(l.maxHealthPoints);
        fout << input << std::endl;
    }
    fout.close();

    filename = folderName + "/units.txt";
    std::ofstream fout2(filename.c_str(), std::ios::out);
    int numberOfUnits = (int)unitList.size();
    fout2 << numberOfUnits << std::endl;
    for (Unit u : unitList)
    {
        input = std::to_string(u.x_Coordinate) + " " + std::to_string(u.y_Coordinate) + " " + std::to_string(u.owner) + " " + std::to_string(u.type) + " " + std::to_string(u.healthPoints) + " " + std::to_string(u.maxhealthPoints) + " " + std::to_string(u.movePoints) + " " + std::to_string(u.maxmovePoints) + " " + std::to_string(u.attacks) + " " + std::to_string(u.maxAttacks);
        fout2 << input << std::endl;
    }
    fout2.close();

    filename = folderName + "/roads.txt";
    std::ofstream fout5(filename.c_str(), std::ios::out);
    int numberOfRoads = (int)roadList.size();
    fout5 << numberOfRoads << std::endl;
    for (AvailableFields aF : roadList)
    {
        input = std::to_string(aF.x) + " " + std::to_string(aF.y);
        fout5 << input << std::endl;
    }
    fout5.close();

    filename = folderName + "/world.txt";
    std::ofstream fout3(filename.c_str(), std::ios::out);

    fout3 << rows << std::endl;
    fout3 << columns << std::endl;
    input = "";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            input += std::to_string(array2D[i * columns + j]) + " ";
        }
        fout3 << input << std::endl;
        input = "";
    }
    fout3.close();

    filename = folderName + "/gameInformation.txt";
    std::ofstream fout4(filename.c_str(), std::ios::out);
    fout4 << numberOfPlayers << std::endl;
    fout4 << currentPlayer << std::endl;

    for (int i = 0; i < numberOfPlayers; i++)
    {
        fout4 << income[i] << std::endl;
    }
    fout4.close();
};

std::list<std::string> ArrayInformation::readCityNames()
{
    std::list<std::string> cityList;
    std::string filename = "../../save_game/cityNames.txt";
    std::string name;
    std::ifstream fin(filename.c_str());
    for (int i = 0; i < 33; i++)
    {
        fin >> name;
        cityList.push_back(name);
    }
    fin.close();
    return cityList;
}