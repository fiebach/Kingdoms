#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Menu
{
public:
    Menu()
    {
        text.setCharacterSize(30);
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
    void setPosition(int newPosition)
    {
        location.setPosition(sf::Vector2f{45, (float)50 + 50 * newPosition});
        text.setPosition(sf::Vector2f{50, (float)50 + 50 * newPosition});
    }
    void setTexture(sf::Texture texture)
    {
        location.setTexture(texture);
    }
    float getWidth()
    {
        return (float)(text.getGlobalBounds().width / 2);
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
    sf::Sprite location;
    std::string name;
};

#endif