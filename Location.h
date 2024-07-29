#ifndef LOCATION_H
#define LOCATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Location
{
public:
    Location()
    {
        location.setRadius(50);
        location.setPointCount(6);
        location.setOutlineThickness(4);
        location.setOutlineColor(sf::Color(105, 105, 105, 255));
        location.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f{100, 100});
        text.setString(name);
        text.setPosition(sf::Vector2f{100, 100});
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
    }
    void setFont(sf::Font &font)
    {
        text.setFont(font);
    }
    void setString(std::string n)
    {
        text.setString(n);
    }
    void setPosition(sf::Vector2f newPosition)
    {
        sf::Vector2f v = converter(sf::Vector2f{(float)x, (float)y});
        location.setPosition(sf::Vector2f{v.x, v.y});
        text.setPosition(sf::Vector2f{(float)(v.x + 50 - (text.getGlobalBounds().width / 2)), v.y + 65});
    }
    void setTexture(sf::Texture *texture)
    {
        location.setTexture(texture);
    }
    void setTextureRect(sf::IntRect rect)
    {
        location.setTextureRect(rect);
    }
    void setFillColor(sf::Color c)
    {
        location.setFillColor(c);
    }
    sf::Vector2f getPosition()
    {
        return location.getPosition();
    }
    sf::Vector2f converter(sf::Vector2f v)
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
    }
    sf::Text text;
    sf::CircleShape location;
    int x;
    int y;
    std::string name;
    int owner;
    int type;
    int income;
    int attack;
    int defense;
    int healthPoints;
    int maxHealthPoints;
};

#endif