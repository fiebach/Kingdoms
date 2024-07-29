#ifndef UNIT_H
#define UNIT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Unit
{
public:
     Unit()
     {
          unit.setRadius(50);
          unit.setPointCount(6);
          unit.setOutlineThickness(4);
          unit.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
          unitPicture.setRadius(50);
          unitPicture.setPointCount(6);
          unitPicture.setOutlineThickness(4);
          unitPicture.setOutlineColor(sf::Color::Color(105, 105, 105, 255));
          text.setString(std::to_string(movePoints) + " " + std::to_string(attacks) + " " + std::to_string(healthPoints));
          text.setPosition(sf::Vector2f{100, 100});
          text.setCharacterSize(12);
          text.setFillColor(sf::Color::White);
          text.setStyle(sf::Text::Bold);
     }
     void Unit::setString(int m, int a, int h)
     {
          text.setString(std::to_string(m) + " " + std::to_string(a) + " " + std::to_string(h));
     }
     void setFont(sf::Font &font)
     {
          text.setFont(font);
     }
     void setPosition(sf::Vector2f newPosition)
     {
          unit.setPosition(newPosition);
          unitPicture.setPosition(newPosition);
          text.setPosition(sf::Vector2f{newPosition.x + 35, newPosition.y + 67});
     }
     void setTexture(sf::Texture *texture)
     {
          unit.setTexture(texture);
          unitPicture.setTexture(texture);
     }
     void setTextureRect(sf::IntRect rect)
     {
          unit.setTextureRect(rect);
          if (type == 1)
          {
               unitPicture.setTextureRect(sf::IntRect{rect.left + 140, rect.top + 18, rect.width, rect.height});
          }
          if (type == 2)
          {
               unitPicture.setTextureRect(sf::IntRect{rect.left + 270, rect.top + 18, rect.width, rect.height});
          }
          if (type == 3)
          {
               unitPicture.setTextureRect(sf::IntRect{rect.left + 400, rect.top + 18, rect.width, rect.height});
          }
          if (type == 4)
          {
               unitPicture.setTextureRect(sf::IntRect{rect.left + 570, rect.top + 18, rect.width, rect.height});
          }
     }
     void setFillColor(sf::Color c)
     {
          unit.setFillColor(c);
     }
     sf::Vector2f getPosition()
     {
          return unit.getPosition();
     }
     sf::CircleShape unit;
     sf::CircleShape unitPicture;
     sf::Text text;
     int x_Coordinate;
     int y_Coordinate;
     int owner;
     int type; //0: air, 1: land, 2: player
     int healthPoints;
     int maxhealthPoints;
     int movePoints;
     int maxmovePoints;
     int attacks;
     int maxAttacks;
};

#endif