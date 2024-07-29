#include "Keyboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

std::string Keyboard::keyboardInput(std::string oldString)
{

    std::string newString;
    std::string emptyString = "";
    if (oldString.size() < 14)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                newString = "A";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                newString = "B";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                newString = "C";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                newString = "D";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                newString = "E";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                newString = "F";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            {
                newString = "G";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            {
                newString = "H";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            {
                newString = "I";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            {
                newString = "J";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            {
                newString = "K";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            {
                newString = "L";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                newString = "M";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                newString = "N";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
                newString = "O";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                newString = "P";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                newString = "Q";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                newString = "R";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                newString = "S";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                newString = "T";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
            {
                newString = "U";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            {
                newString = "V";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                newString = "W";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                newString = "X";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            {
                newString = "Y";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {
                newString = "Z";
            }
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                newString = "a";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                newString = "b";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                newString = "c";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                newString = "d";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                newString = "e";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                newString = "f";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            {
                newString = "g";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            {
                newString = "h";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            {
                newString = "i";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            {
                newString = "j";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            {
                newString = "k";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            {
                newString = "l";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                newString = "m";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                newString = "n";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
                newString = "o";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                newString = "p";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                newString = "q";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                newString = "r";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                newString = "s";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                newString = "t";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
            {
                newString = "u";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            {
                newString = "v";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                newString = "w";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                newString = "x";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            {
                newString = "y";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {
                newString = "z";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            {
                newString = "0";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                newString = "1";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                newString = "2";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                newString = "3";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            {
                newString = "4";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            {
                newString = "5";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
            {
                newString = "6";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
            {
                newString = "7";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            {
                newString = "8";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            {
                newString = "9";
            }
        }
    }
    oldString += newString;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        if (oldString != emptyString)
        {
            oldString = oldString.substr(0, oldString.size() - 1);
        }
    }
    return oldString;
};
