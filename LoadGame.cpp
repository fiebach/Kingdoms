#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <tuple>
#include <list>
#include <string>
#include "LoadGame.h"
#include "ArrayInformation.h"
#include "Unit.h"
#include "Location.h"

sf::Vector2f LoadGame::converter(sf::Vector2f v)
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

std::tuple<int *, int *, int *, int, int, int, int, int *> LoadGame::loadSaveGame(std::string save_game, sf::Font &font, sf::RenderTexture &renderTexture, sf::Texture &unitsTexture, sf::Texture &locationsTexture, sf::Texture &worldTexture)
{
	ArrayInformation loadFile;
	int *playerColors;
	int *unitStats;
	sf::CircleShape hexagon(50, 6);
	hexagon.setOutlineThickness(4);
	hexagon.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
	hexagon.setTextureRect(sf::IntRect{10, 10, 100, 100});

	auto world = loadFile.readFile(save_game + "/world.txt");
	unitList = loadFile.readUnitFile(save_game + "/units.txt");
	locationsList = loadFile.readLocationFile(save_game + "/locations.txt");
	playerColors = loadFile.readColorList();
	unitStats = loadFile.readUnitStats();
	roadList = loadFile.readRoadFile(save_game + "/roads.txt");
	auto gameInformation = loadFile.readGameInformation(save_game + "/gameInformation.txt");

	int rows = std::get<0>(world);
	int columns = std::get<1>(world);
	renderTexture.create(columns * 90 + 45 + 10, rows * 80 + 4 + 50);
	renderTexture.clear(sf::Color::Color(0, 128, 192, 255));
	int *worldArray;
	worldArray = std::get<2>(world);

	float Y = 4;

	for (int row = 0; row < rows; row++)
	{
		float X = 0;
		if (row % 2 == 0)
		{
			X = 0;
		}
		else
		{
			X = 45;
		}
		for (int column = 0; column < columns; column++)
		{

			hexagon.setPosition(sf::Vector2f{X, Y});

			sf::Transform matrix = hexagon.getTransform();
			if (row == 4)
			{
				if (column == 8)
				{
					for (int i = 0; i < 6; i++)
					{
						const auto pointInModelSpace = matrix.transformPoint(hexagon.getPoint(i));
					}
				}
				if (column == 2)
				{
					for (int i = 0; i < 6; i++)
					{
						const auto pointInModelSpace2 = matrix.transformPoint(hexagon.getPoint(i));
					}
				}
			}

			//World Texture
			hexagon.setTexture(&worldTexture);
			mapListValue.push_back(worldArray[row * columns + column]);
			switch (worldArray[row * columns + column])
			{
			case 0: //Sea
				hexagon.setTextureRect(sf::IntRect{10, 300, 100, 100});
				break;
			case 1: //Land
				hexagon.setTextureRect(sf::IntRect{600, 10, 100, 100});
				break;
			case 2: //Mountains
				hexagon.setTextureRect(sf::IntRect{10, 570, 100, 100});
				break;
			case 3: //Forest
				hexagon.setTextureRect(sf::IntRect{8, 2, 100, 100});
				break;
			case 4: //farmland
				hexagon.setTextureRect(sf::IntRect{8, 2, 100, 100});
				break;
			}
			mapList.push_back(hexagon);
			renderTexture.draw(hexagon);
			X += 90;
		}
		Y += 80;
	}
	setRoadTexture(renderTexture, worldTexture);

	std::list<Unit>::iterator u1 = unitList.begin();
	while (u1 != unitList.end())
	{
		Unit &update = *u1;
		update.setPosition(converter(sf::Vector2f{(float)update.x_Coordinate, (float)update.y_Coordinate}));
		update.setFont(font);
		update.setString(update.movePoints, update.attacks, update.healthPoints);
		update.setTexture(&unitsTexture);
		update.setTextureRect(sf::IntRect{13, 0, 145, 200});
		update.setFillColor(sf::Color(playerColors[(update.owner - 1) * 4], playerColors[(update.owner - 1) * 4 + 1], playerColors[(update.owner - 1) * 4 + 2], playerColors[(update.owner - 1) * 4 + 3]));
		*u1 = update;
		++u1;
	}

	std::list<Location>::iterator l1 = locationsList.begin();
	while (l1 != locationsList.end())
	{
		Location &l2 = *l1;
		l2.setFont(font);
		l2.setString(l2.name);
		l2.setPosition(sf::Vector2f{(float)l2.x, (float)l2.y});
		l2.setTexture(&locationsTexture);
		l2.setTextureRect(sf::IntRect{-10, 10, 200, 170});
		if (l2.owner != 0)
		{
			l2.setFillColor(sf::Color(playerColors[(l2.owner - 1) * 4], playerColors[(l2.owner - 1) * 4 + 1], playerColors[(l2.owner - 1) * 4 + 2], playerColors[(l2.owner - 1) * 4 + 3]));
		}

		*l1 = l2;
		++l1;
	}
	int *a = std::get<2>(gameInformation);
	return std::make_tuple(worldArray, playerColors, unitStats, rows, columns, std::get<0>(gameInformation), std::get<1>(gameInformation), std::get<2>(gameInformation));
};

void LoadGame::setRoadTexture(sf::RenderTexture &renderTexture, sf::Texture &worldTexture)
{
	sf::CircleShape hexagon(50, 6);
	hexagon.setOutlineThickness(4);
	hexagon.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
	hexagon.setTextureRect(sf::IntRect{10, 10, 100, 100});
	float new_x;
	float new_y;
	int roadModus = 1000;
	bool beginCity;
	std::list<AvailableFields>::iterator a1 = roadList.begin();
	std::list<AvailableFields>::iterator a2 = roadList.begin();
	a2++;
	std::list<AvailableFields>::iterator a3 = roadList.begin();
	a3++;
	a3++;
	while (a3 != roadList.end())
	{
		beginCity = false;
		roadModus = 1000;
		AvailableFields &beforeElement = *a1;
		AvailableFields &currentElement = *a2;
		AvailableFields &nextElement = *a3;

		new_x = converter(sf::Vector2f{(float)currentElement.x, (float)currentElement.y}).x;
		new_y = converter(sf::Vector2f{(float)currentElement.x, (float)currentElement.y}).y;
		hexagon.setTexture(&worldTexture);
		if (beforeElement.x == 0 && beforeElement.y == 0)
		{ //begin point
			hexagon.setTextureRect(sf::IntRect{10, 910, 100, 100});
			roadModus = modusIdentifier(currentElement.x, currentElement.y, nextElement.x, nextElement.y);
			switch (roadModus)
			{
			case 0:
				hexagon.setTextureRect(sf::IntRect{10, 915, 100, 100});
				break;
			case 1:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 2:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 3:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 4:
				hexagon.setTextureRect(sf::IntRect{-30, 913, 100, 100});
				break;
			case 5:
				hexagon.setTextureRect(sf::IntRect{-30, 913, 100, 100});
				break;
			}
		}
		else if (nextElement.x == 0 && nextElement.y == 0)
		{
			hexagon.setTextureRect(sf::IntRect{10, 910, 100, 100});
			roadModus = modusIdentifier(currentElement.x, currentElement.y, beforeElement.x, beforeElement.y);
			switch (roadModus)
			{
			case 0:
				hexagon.setTextureRect(sf::IntRect{10, 915, 100, 100});
				break;
			case 1:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 2:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 3:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
				break;
			case 4:
				hexagon.setTextureRect(sf::IntRect{-30, 913, 100, 100});
				break;
			case 5:
				hexagon.setTextureRect(sf::IntRect{-30, 913, 100, 100});
				break;
			}
		}
		else
		{
			if (currentElement.y % 2 == 0)
			{
				if ((beforeElement.x + 2 == nextElement.x && beforeElement.y == nextElement.y) ||
					(beforeElement.x - 2 == nextElement.x && beforeElement.y == nextElement.y) ||
					(beforeElement.x + 1 == nextElement.x && beforeElement.y + 2 == nextElement.y) ||
					(beforeElement.x - 1 == nextElement.x && beforeElement.y - 2 == nextElement.y) ||
					(beforeElement.x + 1 == nextElement.x && beforeElement.y - 2 == nextElement.y) ||
					(beforeElement.x - 1 == nextElement.x && beforeElement.y + 2 == nextElement.y))
				{
					roadModus = modusIdentifier(currentElement.x, currentElement.y, nextElement.x, nextElement.y);
					hexagon.setTextureRect(sf::IntRect{70, 913, 100, 100});
				}
			}
			else
			{
				if ((beforeElement.x + 2 == nextElement.x && beforeElement.y == nextElement.y) ||
					(beforeElement.x - 2 == nextElement.x && beforeElement.y == nextElement.y) ||
					(beforeElement.x + 1 == nextElement.x && beforeElement.y - 2 == nextElement.y) ||
					(beforeElement.x - 1 == nextElement.x && beforeElement.y + 2 == nextElement.y) ||
					(beforeElement.x + 1 == nextElement.x && beforeElement.y + 2 == nextElement.y) ||
					(beforeElement.x - 1 == nextElement.x && beforeElement.y - 2 == nextElement.y))
				{
					roadModus = modusIdentifier(currentElement.x, currentElement.y, nextElement.x, nextElement.y);
					hexagon.setTextureRect(sf::IntRect{70, 913, 100, 100});
				}
			}
			if (roadModus == 1000)
			{
				hexagon.setTextureRect(sf::IntRect{400, 956, 100, 100});
				if (currentElement.y % 2 == 0)
				{
					if ((beforeElement.x + 2 == nextElement.x && beforeElement.y + 1 == nextElement.y) ||
						(beforeElement.x - 2 == nextElement.x && beforeElement.y - 1 == nextElement.y))
					{
						roadModus = 0;
					}
					else if ((beforeElement.x == nextElement.x && beforeElement.y + 2 == nextElement.y) ||
							 (beforeElement.x == nextElement.x && beforeElement.y - 2 == nextElement.y))
					{
						if (currentElement.x == beforeElement.x)
						{
							roadModus = 1;
						}
						else
						{
							roadModus = 4;
						}
					}
					else if ((beforeElement.x - 2 == nextElement.x && beforeElement.y + 1 == nextElement.y) ||
							 (beforeElement.x + 2 == nextElement.x && beforeElement.y - 1 == nextElement.y))
					{
						roadModus = 2;
					}
					else if ((beforeElement.x - 1 == nextElement.x && beforeElement.y - 1 == nextElement.y) ||
							 (beforeElement.x + 1 == nextElement.x && beforeElement.y + 1 == nextElement.y))
					{
						roadModus = 3;
					}
					else if ((beforeElement.x + 1 == nextElement.x && beforeElement.y - 1 == nextElement.y) ||
							 (beforeElement.x - 1 == nextElement.x && beforeElement.y + 1 == nextElement.y))
					{
						roadModus = 5;
					}
				}
				else
				{
					if ((beforeElement.x + 1 == nextElement.x && beforeElement.y + 1 == nextElement.y) ||
						(beforeElement.x - 1 == nextElement.x && beforeElement.y - 1 == nextElement.y))
					{
						roadModus = 0;
					}
					else if ((beforeElement.x == nextElement.x && beforeElement.y + 2 == nextElement.y) ||
							 (beforeElement.x == nextElement.x && beforeElement.y - 2 == nextElement.y))
					{
						if (currentElement.x < beforeElement.x)
						{
							roadModus = 1;
						}
						else
						{
							roadModus = 4;
						}
					}
					else if ((beforeElement.x - 1 == nextElement.x && beforeElement.y + 1 == nextElement.y) ||
							 (beforeElement.x + 1 == nextElement.x && beforeElement.y - 1 == nextElement.y))
					{
						roadModus = 2;
					}
					else if ((beforeElement.x - 2 == nextElement.x && beforeElement.y - 1 == nextElement.y) ||
							 (beforeElement.x + 2 == nextElement.x && beforeElement.y + 1 == nextElement.y))
					{
						roadModus = 3;
					}
					else if ((beforeElement.x + 2 == nextElement.x && beforeElement.y - 1 == nextElement.y) ||
							 (beforeElement.x - 2 == nextElement.x && beforeElement.y + 1 == nextElement.y))
					{
						roadModus = 5;
					}
				}
			}
		}
		if (roadModus > 2 && roadModus != 1000)
		{
			hexagon.setTextureRect(sf::IntRect{hexagon.getTextureRect().left + 100, hexagon.getTextureRect().top + 100, -100, -100});
			roadModus -= 3;
		}

		switch (roadModus)
		{
		case 0:
			hexagon.setPosition(sf::Vector2f{new_x, new_y});
			hexagon.setRotation(0);
			break;
		case 1:
			hexagon.setPosition(sf::Vector2f{new_x + 68, new_y - 19});
			hexagon.setRotation(60);
			break;
		case 2:
			hexagon.setPosition(sf::Vector2f{new_x + 119, new_y + 32});
			hexagon.setRotation(120);
			break;
		case 3:
			hexagon.setPosition(sf::Vector2f{new_x + 100, new_y + 100});
			hexagon.setRotation(180);
			break;
		case 4:
			hexagon.setPosition(sf::Vector2f{new_x + 31, new_y + 118});
			hexagon.setRotation(240);
			break;
		case 5:
			hexagon.setPosition(sf::Vector2f{new_x - 19, new_y + 68});
			hexagon.setRotation(300);
			break;
		}
		if (roadModus != 1000)
		{
			mapList.push_back(hexagon);
			renderTexture.draw(hexagon);
			if (beginCity)
			{
				hexagon.setPosition(sf::Vector2f{new_x, new_y});
				hexagon.setTextureRect(sf::IntRect{540, 910, 100, 100});
				hexagon.setRotation(0);
				renderTexture.draw(hexagon);
				mapList.push_back(hexagon);
			}
		}
		++a1;
		++a2;
		++a3;
	}
};

int LoadGame::modusIdentifier(int x, int y, int x_next, int y_next)
{
	int modusRoad = 1000;
	if (y % 2 == 0)
	{
		if (x + 1 == x_next && y == y_next)
		{
			modusRoad = 0;
		}
		if (x == x_next && y + 1 == y_next)
		{
			modusRoad = 1;
		}
		if (x - 1 == x_next && y + 1 == y_next)
		{
			modusRoad = 2;
		}
		if (x - 1 == x_next && y == y_next)
		{
			modusRoad = 3;
		}
		if (x - 1 == x_next && y - 1 == y_next)
		{
			modusRoad = 4;
		}
		if (x == x_next && y - 1 == y_next)
		{
			modusRoad = 5;
		}
	}
	else
	{
		if (x + 1 == x_next && y == y_next)
		{
			modusRoad = 0;
		}
		if (x + 1 == x_next && y + 1 == y_next)
		{
			modusRoad = 1;
		}
		if (x == x_next && y + 1 == y_next)
		{
			modusRoad = 2;
		}
		if (x - 1 == x_next && y == y_next)
		{
			modusRoad = 3;
		}
		if (x == x_next && y - 1 == y_next)
		{
			modusRoad = 4;
		}
		if (x + 1 == x_next && y - 1 == y_next)
		{
			modusRoad = 5;
		}
	}
	return modusRoad;
};