#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GenerateWorld.h"
#include "Move.h"
#include <string>
#include <time.h>
#include <list>
#include "Location.h"
#include "ArrayInformation.h"
#include <windows.h>
void GenerateWorld::placeWalls()
{
	srand(time(NULL));
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			bool TrueFalse = (rand() % 100) < percentOfWalls;
			if (TrueFalse)
			{
				l_map[i * l_width + j] = 2;
			}
			else
			{
				l_map[i * l_width + j] = 1;
			}
		}
	}
};

void GenerateWorld::showMap()
{
	std::string output;
	for (int i = 0; i < heigth; i++)
	{
		output = "";
		for (int j = 0; j < width; j++)
		{
			if (map[i * width + j] == 2)
			{
				output += "#";
			}
			else
			{
				output += " ";
			}
		}
	}
};

void GenerateWorld::checkWall()
{
	int startX;
	int endX;
	int startY;
	int endY;
	int counter;
	int *newMap = new int[l_heigth * l_width];
	for (int i = 0; i < l_width; i++)
	{
		for (int j = 0; j < l_heigth; j++)
		{

			startX = i - 1;
			endX = i + 1;
			startY = j - 1;
			endY = j + 1;

			counter = 0;

			for (int X = startX; X <= endX; X++)
			{
				for (int Y = startY; Y <= endY; Y++)
				{
					if (!(X == i && Y == j))
					{
						if (X < 0 || Y < 0 || X >= l_width || Y >= l_heigth)
						{
							counter++;
						}
						else if (l_map[Y * l_width + X] == 2)
						{
							counter++;
						}
					}
				}
			}

			if (l_map[j * l_width + i] == 2)
			{
				if (counter > 3)
				{
					newMap[j * l_width + i] = 2;
				}
				else
				{
					newMap[j * l_width + i] = 1;
				}
			}
			else
			{
				if (counter > 4)
				{
					newMap[j * l_width + i] = 2;
				}
				else
				{
					newMap[j * l_width + i] = 1;
				}
			}
		}
	}
	l_map = newMap;
};

void GenerateWorld::checkForest()
{
	int startX;
	int endX;
	int startY;
	int endY;
	int counter;
	int *newMap2 = new int[heigth * width];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < heigth; j++)
		{
			if (map[j * heigth + i] == 3 || map[j * heigth + i] == 1)
			{
				startX = i - 1;
				endX = i + 1;
				startY = j - 1;
				endY = j + 1;

				counter = 0;

				for (int X = startX; X <= endX; X++)
				{
					for (int Y = startY; Y <= endY; Y++)
					{
						if (!(X == i && Y == j))
						{

							if (X < 0 || Y < 0 || X >= width || Y >= heigth)
							{
								counter++;
							}
							else if (map[Y * width + X] == 3)
							{
								counter++;
							}
						}
					}
				}

				if (map[j * width + i] == 3)
				{
					if (counter > 3)
					{
						newMap2[j * width + i] = 3;
					}
					else
					{
						newMap2[j * width + i] = 1;
					}
				}
				else
				{
					if (counter > 4)
					{
						newMap2[j * width + i] = 3;
					}
					else
					{
						newMap2[j * width + i] = 1;
					}
				}
			}
			else
			{
				newMap2[j * heigth + i] = map[j * heigth + i];
			}
		}
	}
	map = newMap2;
};

void GenerateWorld::checkMap()
{
	srand(secounds);
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			bool TrueFalse = (rand() % 100) < percentOfWalls;
			if (TrueFalse)
			{
				l_map[i * l_width + j] = 2;
			}
			else
			{
				l_map[i * l_width + j] = 1;
			}
		}
	}
	for (int k = 0; k < 2; k++)
	{
		checkWall();
	}
	numberOfConnectedPixels = 0;
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			if (l_map[i * l_width + j] == 1)
			{
				l_map[i * l_width + j] = 3;
			}
		}
	}
	int randomX = rand() % l_width;
	int randomY = rand() % l_heigth;
	bool notFound = true;
	int maxIterations = 1000;
	while (notFound && maxIterations > 0)
	{
		randomX = rand() % l_width;
		randomY = rand() % l_heigth;
		if (l_map[randomY * l_width + randomX] == 3)
		{
			notFound = false;
		}
		secounds += 10;
		maxIterations--;
	}
	if (!notFound)
	{
		floodFill(randomX, randomY, 3, 1);
		if (!(numberOfConnectedPixels >= (int)(l_width * l_heigth * 0.55)))
		{
			secounds++;
			checkMap2();
		}
	}
	else
	{
		secounds++;
		checkMap2();
	}
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			if (l_map[i * l_width + j] == 3)
			{
				l_map[i * l_width + j] = 2;
			}
		}
	}
	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || j == 0 || i == heigth - 1 || j == width - 1)
			{
				map[i * width + j] = 2;
			}
			else
			{
				map[i * width + j] = l_map[(i - 1) * l_width + (j - 1)];
			}
		}
	}
	secounds++;
};

void GenerateWorld::checkMap2()
{
	srand(secounds);
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			bool TrueFalse = (rand() % 100) < percentOfWalls;
			if (TrueFalse)
			{
				l_map[i * l_width + j] = 2;
			}
			else
			{
				l_map[i * l_width + j] = 1;
			}
		}
	}
	for (int k = 0; k < 2; k++)
	{
		checkWall();
	}
	numberOfConnectedPixels = 0;
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			if (l_map[i * l_width + j] == 1)
			{
				l_map[i * l_width + j] = 3;
			}
		}
	}
	int randomX = rand() % l_width;
	int randomY = rand() % l_heigth;
	bool notFound = true;
	int maxIterations = 1000;
	while (notFound && maxIterations > 0)
	{
		randomX = rand() % l_width;
		randomY = rand() % l_heigth;
		if (l_map[randomY * l_width + randomX] == 3)
		{
			notFound = false;
		}
		maxIterations--;
	}
	if (!notFound)
	{
		floodFill(randomX, randomY, 3, 1);
		if (!(numberOfConnectedPixels >= (int)(l_width * l_heigth * 0.55)))
		{
			secounds++;
			checkMap2();
		}
	}
	else
	{
		secounds++;
		checkMap2();
	}
	for (int i = 0; i < l_heigth; i++)
	{
		for (int j = 0; j < l_width; j++)
		{
			if (l_map[i * l_width + j] == 3)
			{
				l_map[i * l_width + j] = 2;
			}
		}
	}
};

void GenerateWorld::floodFill(int x, int y, int oldColor, int newColor)
{
	if (y >= 0 && y < l_heigth && x >= 0 && x < l_width)
	{
		if (l_map[y * l_width + x] == oldColor)
		{
			l_map[y * l_width + x] = newColor;
			numberOfConnectedPixels++;
			floodFill(x, y + 1, oldColor, newColor);
			floodFill(x, y - 1, oldColor, newColor);
			floodFill(x + 1, y, oldColor, newColor);
			floodFill(x - 1, y, oldColor, newColor);
		}
	}
};

void GenerateWorld::cityGenerator(int numberOfPlayers)
{
	int randomX;
	int randomY;
	bool collision = true;
	bool collision2 = true;
	srand(secounds);
	ArrayInformation a;
	std::list<std::string> cityList = a.readCityNames();
	std::list<std::string>::iterator nameIterator = cityList.begin();
	for (int i = 0; i < numberOfLocations; i++)
	{
		collision = true;
		std::string &nameElement = *nameIterator;
		Location l;

		while (collision)
		{
			randomX = rand() % width;
			randomY = rand() % heigth;
			collision2 = true;
			if (map[randomY * width + randomX] == 1)
			{
				if (!locationList.empty())
				{
					for (Location l1 : locationList)
					{
						if (l1.x - 6 < randomX && l1.x + 6 > randomX && l1.y - 6 < randomY && l1.y + 6 > randomY)
						{
							collision2 = false;
						}
					}
				}
			}
			else
			{
				collision2 = false;
			}
			if (collision2)
			{
				collision = false;
			}
		}
		if (numberOfPlayers > 0)
		{
			l.owner = numberOfPlayers;
			numberOfPlayers--;
		}
		else
		{
			l.owner = 0;
		}

		l.x = randomX;
		l.y = randomY;
		l.name = nameElement;
		l.type = 1;
		l.income = 10;
		l.attack = 0;
		l.defense = 0;
		l.healthPoints = 0;
		l.maxHealthPoints = 0;
		locationList.push_back(l);
		*nameIterator = nameElement;
		++nameIterator;
	}
};

void GenerateWorld::placeWater(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < heigth)
	{
		if (map[y * width + x] == 2)
		{
			map[y * width + x] = 0;
			if (y % 2 == 0)
			{
				placeWater(x - 1, y - 1);
				placeWater(x, y - 1);
				placeWater(x + 1, y);
				placeWater(x, y + 1);
				placeWater(x - 1, y + 1);
				placeWater(x - 1, y);
			}
			else
			{
				placeWater(x, y - 1);
				placeWater(x + 1, y - 1);
				placeWater(x + 1, y);
				placeWater(x + 1, y + 1);
				placeWater(x, y + 1);
				placeWater(x - 1, y);
			}
		}
	}
};

void GenerateWorld::placeForest()
{
	srand(secounds);
	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			bool TrueFalse = (rand() % 100) < percentOfForests;
			if (map[i * width + j] == 1)
			{
				if (TrueFalse)
				{
					map[i * width + j] = 3;
				}
				else
				{
					map[i * width + j] = 1;
				}
			}
		}
	}
	for (int k = 0; k < 3; k++)
	{
		checkForest();
	}
};

void GenerateWorld::placeRoad()
{
	connectedCities = new int[numberOfLocations * numberOfLocations];
	for (int i = 0; i < numberOfLocations; i++)
	{
		for (int j = 0; j < numberOfLocations; j++)
		{
			connectedCities[i * numberOfLocations + j] = 0;
			if (i == j)
			{
				connectedCities[i * numberOfLocations + j] = 1;
			}
		}
	}
	locationCopy = locationList;
	bool findway;
	int numberOfConnectedLocations;
	elementNumber = 0;
	bool allCitiesFound;
	for (Location l3 : locationList)
	{
		allCitiesFound = true;
		while (allCitiesFound)
		{
			findway = true;
			numberOfConnectedLocations = 0;
			for (int i = 0; i < numberOfLocations; i++)
			{
				if (connectedCities[elementNumber * numberOfLocations + i] == 1)
				{
					numberOfConnectedLocations++;
				}
			}

			if (numberOfConnectedLocations < 3)
			{
				openF.clear();
				fields.clear();
				numberOfLocationsToSearch = 1;
				OpenFields newLocation;
				newLocation.x = l3.x;
				newLocation.y = l3.y;
				openF.push_back(newLocation);
				searchRoad();
				std::list<AvailableFields>::iterator a1 = fields.begin();
				AvailableFields &a2 = *a1;
				AvailableFields cityFound;
				cityFound.x = 0;
				cityFound.y = 0;
				road.push_back(cityFound);
				cityFound.x = a2.x;
				cityFound.y = a2.y;
				cityFound.x_predecessor = a2.x_predecessor;
				cityFound.y_predecessor = a2.y_predecessor;
				road.push_back(cityFound);

				while (findway)
				{
					for (AvailableFields a : fields)
					{
						if (a.x == cityFound.x_predecessor && a.y == cityFound.y_predecessor)
						{
							cityFound.x = a.x;
							cityFound.y = a.y;
							cityFound.x_predecessor = a.x_predecessor;
							cityFound.y_predecessor = a.y_predecessor;
							road.push_back(cityFound);
							break;
						}
					}
					if (newLocation.x == cityFound.x_predecessor && newLocation.y == cityFound.y_predecessor)
					{
						cityFound.x = newLocation.x;
						cityFound.y = newLocation.y;
						road.push_back(cityFound);
						findway = false;
					}
				}
			}
			else
			{
				allCitiesFound = false;
			}
		}
		elementNumber++;
	}
	AvailableFields cityFound2;
	cityFound2.x = 0;
	cityFound2.y = 0;
	road.push_back(cityFound2);

	for (int i = 0; i < numberOfLocations; i++)
	{
		std::string outputS = "";
		for (int j = 0; j < numberOfLocations; j++)
		{
			outputS += std::to_string(connectedCities[i * numberOfLocations + j]);
		}
	}
};

void GenerateWorld::searchRoad()
{
	OpenFields firstElement = openF.front();
	if (numberOfLocationsToSearch > 0)
	{
		int x = firstElement.x;
		int y = firstElement.y;
		int new_x;
		int new_y;
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
			if (map[new_y * width + new_x] == 1)
			{
				bool exist = false;
				bool e = true;
				int numberOfElement = 0;
				for (Location l : locationCopy)
				{
					if (l.x == new_x && l.y == new_y && connectedCities[elementNumber * numberOfLocations + numberOfElement] == 0)
					{
						connectedCities[elementNumber * numberOfLocations + numberOfElement] = 1;
						connectedCities[numberOfElement * numberOfLocations + elementNumber] = 1;
						e = false;
						break;
					}
					numberOfElement++;
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
						new_f.x = new_x;
						new_f.y = new_y;
						new_f.x_predecessor = x_p;
						new_f.y_predecessor = y_p;
						fields.push_back(new_f);
						OpenFields openF1;
						openF1.x = new_x;
						openF1.y = new_y;
						openF.push_back(openF1);
					}
				}
				else
				{
					numberOfLocationsToSearch--;
					AvailableFields new_f;
					new_f.x = new_x;
					new_f.y = new_y;
					new_f.x_predecessor = x_p;
					new_f.y_predecessor = y_p;
					fields.push_front(new_f);
				}
			}
		}
	}
	openF.pop_front();

	if (!openF.empty() && numberOfLocationsToSearch > 0)
	{
		searchRoad();
	}
};

void GenerateWorld::setRoadTexture(sf::RenderTexture &renderTexture, sf::Texture &worldTexture)
{
	sf::CircleShape hexagon(50, 6);
	hexagon.setOutlineThickness(4);
	hexagon.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
	hexagon.setTextureRect(sf::IntRect{10, 10, 100, 100});
	float new_x;
	float new_y;
	int roadModus = 1000;
	bool beginCity;
	std::list<AvailableFields>::iterator a1 = road.begin();
	std::list<AvailableFields>::iterator a2 = road.begin();
	a2++;
	std::list<AvailableFields>::iterator a3 = road.begin();
	a3++;
	a3++;
	while (a3 != road.end())
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
			beginCity = true;
			hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
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
		{ //end point
			beginCity = true;
			hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
			roadModus = modusIdentifier(currentElement.x, currentElement.y, beforeElement.x, beforeElement.y);
			switch (roadModus)
			{
			case 0:
				hexagon.setTextureRect(sf::IntRect{10, 913, 100, 100});
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
			{ //normal street
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
int GenerateWorld::modusIdentifier(int x, int y, int x_next, int y_next)
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
}

sf::Vector2f GenerateWorld::converter(sf::Vector2f v)
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

std::tuple<int *, int *, int *, int, int, int, int, int *> GenerateWorld::worldG(std::string save_game, sf::Font &font, sf::RenderTexture &renderTexture, sf::Texture &unitsTexture, sf::Texture &locationsTexture, sf::Texture &worldTexture, int numberOfPlayers)
{
	ArrayInformation loadFile;
	int *playerColors;
	int *unitStats;
	sf::CircleShape hexagon(50, 6);
	hexagon.setOutlineThickness(4);
	hexagon.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
	hexagon.setTextureRect(sf::IntRect{10, 10, 100, 100});

	playerColors = loadFile.readColorList();
	unitStats = loadFile.readUnitStats();

	int rows = heigth;
	int columns = width;
	renderTexture.create(columns * 90 + 45 + 10, rows * 80 + 4 + 50);
	renderTexture.clear(sf::Color::Color(0, 128, 192, 255));
	int *worldArray;
	bool workingOnMapGeneration = true;
	int NumberOfMountains = 0;
	secounds = static_cast<long int>(time(NULL));
	while (workingOnMapGeneration)
	{
		NumberOfMountains = 0;
		checkMap();
		placeWater(0, 0);

		for (int i = 0; i < heigth; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (map[i * width + j] == 2)
				{
					NumberOfMountains++;
				}
			}
		}

		if (!(NumberOfMountains >= (int)(l_width * l_heigth * 0.1)))
		{
			secounds++;
		}
		else
		{
			workingOnMapGeneration = false;
		}
	}

	cityGenerator(numberOfPlayers);
	placeRoad();
	placeForest();
	for (AvailableFields r : road)
	{
		if (map[r.y * width + r.x] == 3)
		{
			map[r.y * width + r.x] = 1;
		}
	}
	worldArray = map;
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

	std::list<Location>::iterator l1 = locationList.begin();
	while (l1 != locationList.end())
	{
		Location &l2 = *l1;
		l2.setFont(font);
		l2.setString(l2.name);
		l2.setPosition(sf::Vector2f{(float)l2.x, (float)l2.y});
		l2.setTexture(&locationsTexture);
		l2.setTextureRect(sf::IntRect{-10, 10, 200, 170});
		if (l2.owner != 0)
		{
			l2.setFillColor(sf::Color(playerColors[(l2.owner - 1) * 4], playerColors[(l2.owner - 1) * 4 + 1], playerColors[(l2.owner - 1) * 4 + 2], 255));
		}

		*l1 = l2;
		++l1;
	}
	int *incomeArray = new int[numberOfPlayers];
	for (int iA = 0; iA < numberOfPlayers; iA++)
	{
		incomeArray[iA] = 10;
	}
	return std::make_tuple(worldArray, playerColors, unitStats, rows, columns, numberOfPlayers, 1, incomeArray);
};