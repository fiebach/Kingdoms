#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Keyboard
{
public:
    std::string keyboardInput(std::string oldString);
};
#endif