#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <list>
#include <math.h>
#include <map>
#include <vector>
#include <functional>
#include <Windows.h>
#include <vector>
#include <stdio.h>

#include "ArrayInformation.h"
#include "Unit.h"
#include "Move.h"
#include "Location.h"
#include "Menu.h"
#include "LoadGame.h"
#include "Keyboard.h"
#include "GenerateWorld.h"

std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
    std::vector<std::string> names;
    std::string search_path = folder + "/*";
    WIN32_FIND_DATA fd;
    HANDLE handleFinder = ::FindFirstFile(search_path.c_str(), &fd);
    int count = 0;
    if (handleFinder != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                if (count > 1)
                {
                    names.push_back(fd.cFileName);
                }
                count++;
            }
        } while (::FindNextFile(handleFinder, &fd));
        ::FindClose(handleFinder);
    }
    return names;
};

void deleteSaveGame(std::string folderName)
{
    std::string fullName = "../../save_game/" + folderName + "/gameInformation.txt";
    const char *c = fullName.c_str();
    remove(c);
    fullName = "../../save_game/" + folderName + "/locations.txt";
    const char *c1 = fullName.c_str();
    remove(c1);
    fullName = "../../save_game/" + folderName + "/units.txt";
    const char *c2 = fullName.c_str();
    remove(c2);
    fullName = "../../save_game/" + folderName + "/world.txt";
    const char *c3 = fullName.c_str();
    remove(c3);
    fullName = "../../save_game/" + folderName + "/roads.txt";
    const char *c4 = fullName.c_str();
    remove(c4);
    fullName = "../../save_game/" + folderName;
    const char *c5 = fullName.c_str();
    ::RemoveDirectory(("../../save_game/" + folderName).c_str());
}

std::tuple<float, float> determinePosition(sf::Vector2i mousePosition, int x_c, int y_c)
{
    int X = mousePosition.x - 1 - x_c;
    int Y = mousePosition.y - y_c;
    int shortX = 45;
    int realShortX = 43;
    int longX = 90;
    int shortY = 30;
    int longY = 50;
    int height = 0;
    double line;
    int modus = 100;

    while (true)
    {
        if (Y >= shortY)
        {
            Y -= shortY;
        }
        else
        {
            modus = 0;
            break;
        }
        if (Y >= longY)
        {
            Y -= longY;
            height += 1;
        }
        else
        {
            modus = 1;
            break;
        }
        if (Y >= shortY)
        {
            Y -= shortY;
        }
        else
        {
            modus = 2;
            break;
        }
        if (Y >= longY)
        {
            Y -= longY;
            height += 1;
        }
        else
        {
            modus = 3;
            break;
        }
    }
    int wide;
    if (modus == 3)
    {
        wide = (int)floor((X - shortX) / longX);
    }
    else
    {
        wide = (int)floor(X / longX);
    }

    int spezialWide = (int)X % longX;

    if (modus == 0)
    {
        if (spezialWide < realShortX)
        { //left
            line = ((double)(realShortX - spezialWide)) * (0.581);
            if (line > Y)
            { //up
                wide -= 1;
                height -= 1;
            }
            else
            { //down
            }
        }
        else
        { //right
            line = ((double)(spezialWide - realShortX)) * (0.581);
            if (line > Y)
            { //up
                height -= 1;
            }
            else
            { //down
            }
        }
    }

    if (modus == 1)
    {
    }

    if (modus == 2)
    {
        if (spezialWide < realShortX)
        { //left
            line = shortY - ((double)(realShortX - spezialWide)) * (0.581);
            if (line < Y)
            { //down
                wide -= 1;
            }
            else
            { //up
                height -= 1;
            }
        }
        else
        {
            line = shortY - ((double)(spezialWide - realShortX)) * (0.581);
            if (line < Y)
            {
            }
            else
            {
                height -= 1;
            }
        }
    }

    if (modus == 3)
    {
    }

    return std::make_tuple((float)wide, (float)height);
}

sf::Vector2f converter(sf::Vector2f v, int x_c, int y_c)
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
    X += 90 * v.x + x_c;
    Y += 80 * v.y + y_c;
    return sf::Vector2f{X, Y};
}

int main()
{
    std::cout << "Start Kingdoms" << std::endl;
    int status = 0;
    int numberOfPlayers = 2;
    int currentPlayer = 1;
    int columns;
    int rows;
    int *incomeOfPlayers;
    std::string save_game = "save_game";
    std::list<Unit> unitList;
    std::list<Unit> movementList;
    std::list<Move> moveList;
    std::list<Location> locationsList;
    std::list<Menu> menuList;
    std::list<sf::CircleShape> mapList;
    std::list<int> mapListValue;
    std::list<AvailableFields> roadList;
    int *worldArray;
    int *playerColors;
    int *unitStats;
    bool fClick = false;
    bool newUnitPurchased = false;
    int touchedUnit = 0;
    std::string continueGame;
    bool start_correction = false;
    int map_size = -1;
    int x_correction = 0;
    int y_correction = 0;
    int x_correction_local = 0;
    int y_correction_local = 0;
    bool fightDocumentation = false;
    sf::Texture locationsTexture;
    locationsTexture.loadFromFile("../../texture/city.png");
    //Load game
    ArrayInformation loadArray;
    continueGame = loadArray.readInformationFile("../../" + save_game + "/informationFile.txt");

    sf::RenderTexture renderTexture;
    sf::Texture unitsTexture;
    unitsTexture.setSmooth(true);
    unitsTexture.loadFromFile("../../texture/unit.png");

    sf::Texture marbleTexture;
    marbleTexture.loadFromFile("../../texture/buymenu.png");

    sf::Texture moveTexture;
    moveTexture.setSmooth(true);
    moveTexture.loadFromFile("../../texture/move.png");

    sf::Texture menuTexture;
    menuTexture.loadFromFile("../../texture/menu.png");

    sf::Texture loadGameTexture;
    loadGameTexture.loadFromFile("../../texture/line.png");

    sf::Texture menuBackground;
    menuBackground.loadFromFile("../../texture/menuBackground.png");

    sf::Texture map;
    map.loadFromFile("../../texture/map.jpg");

    sf::Font font;
    font.loadFromFile("../../texture/OpenSans.ttf");

    sf::Font medievalFont;
    medievalFont.loadFromFile("../../texture/BLKCHCRY.ttf");

    sf::Texture menuField;
    menuField.loadFromFile("../../texture/menuField.png");

    unitList.clear();
    moveList.clear();

    sf::CircleShape nextPlayer(200, 3);
    nextPlayer.setOutlineThickness(4);
    nextPlayer.setOutlineColor(sf::Color(218, 165, 32, 255));
    nextPlayer.setPosition(sf::Vector2f{2130, 70});
    nextPlayer.setRotation(165);

    sf::Sprite buyUnit;
    buyUnit.setTexture(marbleTexture);
    buyUnit.setTextureRect(sf::IntRect{400, 400, 55, 105});
    buyUnit.setPosition(sf::Vector2f{10, 10});
    buyUnit.setScale((float)0.8, (float)0.8);

    sf::Sprite buyUnit2;
    buyUnit2.setTexture(marbleTexture);
    buyUnit2.setTextureRect(sf::IntRect{400, 400, 55, 105});
    buyUnit2.setPosition(sf::Vector2f{90, 10});
    buyUnit2.setScale((float)0.8, (float)0.8);

    sf::Sprite buyUnit3;
    buyUnit3.setTexture(marbleTexture);
    buyUnit3.setTextureRect(sf::IntRect{400, 400, 55, 105});
    buyUnit3.setPosition(sf::Vector2f{160, 10});
    buyUnit3.setScale((float)0.8, (float)0.8);

    sf::Sprite buyUnit4;
    buyUnit4.setTexture(marbleTexture);
    buyUnit4.setTextureRect(sf::IntRect{400, 400, 55, 105});
    buyUnit4.setPosition(sf::Vector2f{235, 10});
    buyUnit4.setScale((float)0.8, (float)0.8);

    sf::Sprite buyUnit5;
    buyUnit5.setTexture(marbleTexture);
    buyUnit5.setTextureRect(sf::IntRect{25, 50, 1000, 150});
    buyUnit5.setPosition(sf::Vector2f{0, 0});
    buyUnit5.setScale((float)0.8, (float)0.8);

    sf::Sprite buyUnit6;
    buyUnit6.setTexture(marbleTexture);
    buyUnit6.setTextureRect(sf::IntRect{25, 200, 1000, 220});
    buyUnit6.setPosition(sf::Vector2f{0, 200});
    buyUnit6.setScale((float)0.8, (float)0.8);

    Menu backButton;
    backButton.location.setColor(sf::Color::Magenta);
    backButton.location.setTexture(menuTexture);
    backButton.location.setTextureRect(sf::IntRect{0 + 200, 0 + 200, 200, 80});
    backButton.location.setScale((float)0.45, (float)0.45);
    backButton.setFont(font);
    backButton.text.setPosition(sf::Vector2f(50, 950));
    backButton.location.setPosition(sf::Vector2f(45, 950));
    backButton.setString("Back");

    Menu menuPoint;
    menuPoint.location.setColor(sf::Color::Magenta);
    menuPoint.location.setTexture(menuTexture);
    menuPoint.location.setTextureRect(sf::IntRect{0 + 200, 0 + 200, 400, 80});
    menuPoint.location.setScale((float)0.45, (float)0.45);
    menuPoint.setFont(font);
    menuPoint.setPosition(0);
    menuPoint.setString("Continue");
    menuList.push_back(menuPoint);
    menuPoint.setPosition(1);
    menuPoint.setString("New Game");
    menuList.push_back(menuPoint);
    menuPoint.setPosition(2);
    menuPoint.setString("Load Game");
    menuList.push_back(menuPoint);
    menuPoint.setPosition(3);
    menuPoint.setString("Credits");
    menuList.push_back(menuPoint);
    menuPoint.setPosition(4);
    menuPoint.setString("Quit");
    menuList.push_back(menuPoint);

    std::list<AvailableFields> localMovingList;
    std::list<sf::Text> attackText;

    sf::View view;
    view.reset(sf::FloatRect(0, 0, 1920, 1080 - 63));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    // create and initialize the window (+open)
    sf::RenderWindow window(sf::VideoMode{1920, 1080 - 63}, "Kingdoms");
    window.setPosition(sf::Vector2i{0,0});
    sf::Image icon;
    icon.loadFromFile("../../texture/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    int counter = 0;

    // this sets how much time has to pass for an update, in case more time passed, do several updates at once
    sf::Time updateTime = sf::milliseconds(10);
    int waitTime = 0;
    sf::Vector2f firstPosition;
    Unit firstUnit;

    // the clock to keep track of how much time has passed between a cycle
    sf::Clock clock;

    // this saves how much time has passed
    sf::Time elapsed = clock.restart();

    // keep track if we have to redraw things. No need if nothing has been updated!
    bool update = true;
    int menuStatus = 0;

    sf::Texture throne;
    throne.setSmooth(true);
    throne.loadFromFile("../../texture/throne.png");

    sf::Texture castle;
    castle.loadFromFile("../../texture/castle.jpg");

    sf::Texture worldTexture;
    worldTexture.setSmooth(true);
    worldTexture.loadFromFile("../../texture/field.png");

    sf::Texture flagTexture;
    flagTexture.loadFromFile("../../texture/flag.png");

    sf::Texture stickTexture;
    stickTexture.loadFromFile("../../texture/stick.png");

    int intFirstPlayer;

    float window_size = (float)((signed)window.getSize().x) / 2;

    sf::Sprite flag;
    flag.setTextureRect(sf::IntRect{0, 0, 260, 1000});
    flag.setTexture(flagTexture);

    sf::Sprite stick;
    stick.setTextureRect(sf::IntRect{0, 0, 350, 1000});
    stick.setTexture(stickTexture);

    sf::Sprite startBackground;
    startBackground.setTexture(throne);
    startBackground.setPosition(sf::Vector2f{0, 0});

    sf::Sprite testSprite;
    testSprite.setColor(sf::Color::Green);
    testSprite.setPosition(sf::Vector2f{0, 0});

    sf::Sprite loadGameBackground;
    loadGameBackground.setTexture(castle);
    loadGameBackground.setPosition(sf::Vector2f{0, 0});
    loadGameBackground.setScale(1, 1);

    sf::Sprite createGameBackground;
    createGameBackground.setTexture(map);
    createGameBackground.setPosition(sf::Vector2f{0, 0});
    createGameBackground.setScale((float)1.3, (float)1.3);

    sf::Sprite creditBackground;
    creditBackground.setTexture(castle);
    creditBackground.setPosition(sf::Vector2f{0, -200});
    creditBackground.setScale((float)1.2, (float)1.2);

    Menu numPlayer;
    numPlayer.setFont(font);
    numPlayer.setString("Number of Players");
    numPlayer.text.setCharacterSize(50);
    numPlayer.text.setPosition(sf::Vector2f{window_size - numPlayer.getWidth(), 50});

    Menu numPlayer1;
    numPlayer1.setFont(font);
    numPlayer1.setString("-");
    numPlayer1.text.setCharacterSize(50);
    numPlayer1.text.setPosition(sf::Vector2f{window_size - numPlayer1.getWidth() - 50, 120});

    Menu numPlayer2;
    numPlayer2.setFont(font);
    numPlayer2.setString("+");
    numPlayer2.text.setCharacterSize(50);
    numPlayer2.text.setPosition(sf::Vector2f{window_size - numPlayer2.getWidth() + 50, 120});

    Menu numPlayer3;
    numPlayer3.setFont(font);
    numPlayer3.setString("2");
    numPlayer3.text.setCharacterSize(50);
    numPlayer3.text.setPosition(sf::Vector2f{window_size - numPlayer3.getWidth(), 120});

    Menu numPlayer4;
    numPlayer4.setFont(font);
    numPlayer4.setString("Generate World");
    numPlayer4.text.setCharacterSize(50);
    numPlayer4.text.setPosition(sf::Vector2f{window_size - numPlayer4.getWidth(), 800});

    Menu numPlayer5;
    numPlayer5.setFont(font);
    numPlayer5.setString("Map size");
    numPlayer5.text.setCharacterSize(50);
    numPlayer5.text.setPosition(sf::Vector2f{window_size - numPlayer5.getWidth(), 250});

    Menu numPlayer7;
    numPlayer7.setFont(font);
    numPlayer7.setString("normal");
    numPlayer7.text.setCharacterSize(50);
    numPlayer7.text.setPosition(sf::Vector2f{window_size - numPlayer7.getWidth(), 320});

    Menu numPlayer6;
    numPlayer6.setFont(font);
    numPlayer6.setString("small");
    numPlayer6.text.setCharacterSize(50);
    numPlayer6.text.setPosition(sf::Vector2f{window_size - numPlayer7.getWidth() - numPlayer6.getWidth() * 2 - 100, 320});

    Menu numPlayer8;
    numPlayer8.setFont(font);
    numPlayer8.setString("big");
    numPlayer8.text.setCharacterSize(50);
    numPlayer8.text.setPosition(sf::Vector2f{window_size + numPlayer7.getWidth() + 100, 320});

    Menu deleteGame;
    deleteGame.setFont(font);
    deleteGame.text.setPosition(sf::Vector2f(980, 950));
    deleteGame.setString("Delete");
    deleteGame.text.setCharacterSize(40);

    Menu loadGame;
    loadGame.setFont(font);
    loadGame.text.setPosition(sf::Vector2f(750, 950));
    loadGame.setString("Load");
    loadGame.text.setCharacterSize(40);

    Menu inGameMenu;
    inGameMenu.setFont(font);
    inGameMenu.location.setTexture(menuBackground);
    inGameMenu.location.setTextureRect(sf::IntRect{0, 0, 330, 500});
    inGameMenu.location.setPosition((1920 - inGameMenu.location.getLocalBounds().width) / 2, 100);
    inGameMenu.setString("Menu");
    inGameMenu.text.setPosition((1920 - inGameMenu.text.getLocalBounds().width) / 2, 120);

    Menu inGameMenu1;
    inGameMenu1.setFont(font);
    inGameMenu1.location.setTexture(menuField);
    inGameMenu1.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu1.location.setPosition(((1920 - inGameMenu1.location.getLocalBounds().width) / 2) - 3, 180);
    inGameMenu1.setString("Continue");
    inGameMenu1.text.setPosition((1920 - inGameMenu1.text.getLocalBounds().width) / 2, 190);

    Menu inGameMenu2;
    inGameMenu2.setFont(font);
    inGameMenu2.location.setTexture(menuField);
    inGameMenu2.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu2.location.setPosition(((1920 - inGameMenu2.location.getLocalBounds().width) / 2) - 3, 260);
    inGameMenu2.setString("Save Game");
    inGameMenu2.text.setPosition((1920 - inGameMenu2.text.getLocalBounds().width) / 2, 270);

    Menu inGameMenu3;
    inGameMenu3.setFont(font);
    inGameMenu3.location.setTexture(menuField);
    inGameMenu3.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu3.location.setPosition(((1920 - inGameMenu3.location.getLocalBounds().width) / 2) - 3, 340);
    inGameMenu3.setString("Load Game");
    inGameMenu3.text.setPosition((1920 - inGameMenu3.text.getLocalBounds().width) / 2, 350);

    Menu inGameMenu4;
    inGameMenu4.setFont(font);
    inGameMenu4.location.setTexture(menuField);
    inGameMenu4.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu4.location.setPosition(((1920 - inGameMenu4.location.getLocalBounds().width) / 2) - 3, 420);
    inGameMenu4.setString("Go to Main Menu");
    inGameMenu4.text.setPosition(((1920 - inGameMenu4.text.getLocalBounds().width) / 2) - 5, 430);

    Menu inGameMenu5;
    inGameMenu5.setFont(font);
    inGameMenu5.location.setTexture(menuField);
    inGameMenu5.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu5.location.setPosition(((1920 - inGameMenu5.location.getLocalBounds().width) / 2) - 3, 500);
    inGameMenu5.setString("Quit");
    inGameMenu5.text.setPosition((1920 - inGameMenu5.text.getLocalBounds().width) / 2, 510);

    Menu inGameMenu6;
    inGameMenu6.setFont(font);
    inGameMenu6.location.setTexture(menuField);
    inGameMenu6.location.setTextureRect(sf::IntRect{0, 0, 273, 60});
    inGameMenu6.location.setPosition(((1920 - inGameMenu6.location.getLocalBounds().width) / 2) - 3, 420);
    inGameMenu6.setString("Save Game");
    inGameMenu6.text.setPosition((1920 - inGameMenu6.text.getLocalBounds().width) / 2, 430);

    Menu inGameMenu7;
    inGameMenu7.setFont(font);
    inGameMenu7.setString("");
    inGameMenu7.text.setPosition((1920 - inGameMenu7.text.getLocalBounds().width) / 2, 350);

    Menu inGameMenu8;
    inGameMenu8.setFont(medievalFont);
    inGameMenu8.location.setTexture(menuBackground);
    inGameMenu8.location.setTextureRect(sf::IntRect{0, 0, 320, 500});
    inGameMenu8.location.setPosition((1920 - inGameMenu8.location.getLocalBounds().width) / 2, 100);
    inGameMenu8.setString("Long live \n the emperor!");
    inGameMenu8.text.setCharacterSize(100);
    inGameMenu8.text.setPosition(window_size - inGameMenu8.getWidth(), 100);

    Menu inGameMenu9;
    inGameMenu9.setFont(font);
    inGameMenu9.location.setTexture(menuBackground);
    inGameMenu9.location.setTextureRect(sf::IntRect{0, 0, 250, 500});
    inGameMenu9.location.setPosition((1920 - inGameMenu8.location.getLocalBounds().width) / 2, 100);
    inGameMenu9.setString("10");
    inGameMenu9.text.setPosition(sf::Vector2f(330, 35));

    Menu inGameMenu10;
    inGameMenu10.setFont(font);
    inGameMenu10.location.setTexture(marbleTexture);
    inGameMenu10.location.setTextureRect(sf::IntRect{3, 1030, 1000, 180});
    inGameMenu10.location.setPosition(sf::Vector2f(1650, 470));
    inGameMenu10.text.setPosition(sf::Vector2f(1670, 500));
    inGameMenu10.text.setCharacterSize(16);
    inGameMenu10.setString("");

    Menu inGameMenu11;
    inGameMenu11.setFont(font);
    inGameMenu11.location.setTexture(marbleTexture);
    inGameMenu11.location.setTextureRect(sf::IntRect{365, 1030, 1000, 180});
    inGameMenu11.location.setPosition(sf::Vector2f(1800, 470));

    srand(time(NULL));
    bool firstClick = false;
    bool buy = false;
    bool begin = true;
    bool conquerCity = false;
    int conquerCity_x = 0;
    int conquerCity_y = 0;

    int goal_x;
    int goal_y;
    std::string fightInformation;
    Move m;
    std::list<OpenFields> openList;
    std::list<AvailableFields> availableList;
    std::list<AvailableFields> availableList2;
    std::list<AvailableFields> attackList;
    std::list<Menu> credits;
    OpenFields openF;
    AvailableFields availableF;
    AvailableFields attackF;
    std::string selectedSaveGame;
    std::vector<Menu> loadGameVector;
    bool openMenu = false;
    bool endGame = false;
    Menu u;
    u.setFont(font);
    u.text.setFillColor(sf::Color(212, 175, 55));
    u.setPosition(0);
    u.setString("Artist");
    credits.push_back(u);
    u.setPosition(1);
    u.setString("- Keith333 - Weathered Wood - Seamless Texture with Normalmap");
    credits.push_back(u);
    u.setPosition(2);
    u.setString("- phaelax - Stone Texture + Bump");
    credits.push_back(u);
    u.setPosition(3);
    u.setString("- domsson - Two pixelish Tree Sprites ");
    credits.push_back(u);
    u.setPosition(4);
    u.setString("- LuizMelo - Medieval Warrior Pack 2");
    credits.push_back(u);
    u.setPosition(5);
    u.setString("- sanctumpixel - Loreon Kingdom Pixel Art Character Asset");
    credits.push_back(u);
    u.setPosition(6);
    u.setString("- William.Thomsonj (contributor) and Sergei Churbanov, ");
    credits.push_back(u);
    u.setPosition(7);
    u.setString("  aka CatBlack (graphic artist) - Kujasa - The Beginning");
    credits.push_back(u);
    u.setPosition(8);
    u.setString("- Thatnewdevgirl - Concept Art - Landscape 1");
    credits.push_back(u);
    u.setPosition(9);
    u.setString("- Ironthunder/Markeus B - Cold Scene");
    credits.push_back(u);
    u.setPosition(10);
    u.setString("- Steve Matteson (Designer) - OpenSans");
    credits.push_back(u);
    u.setPosition(11);
    u.setString("- Doug Miles - BlackChancery Font");
    credits.push_back(u);
    u.setPosition(12);
    u.setString("more Information can be found in the Readme");
    credits.push_back(u);
    u.setPosition(13);
    u.setString("");
    credits.push_back(u);
    u.setPosition(14);
    u.setString("Programmer and Tester");
    credits.push_back(u);
    u.setPosition(15);
    u.setString("- Vinzenz Fiebach");

    credits.push_back(u);

    std::string input = "";
    int buyUnitType = 0;

    while (window.isOpen())
    {
        elapsed = elapsed + clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        while (elapsed > updateTime)
        {
            updateTime = sf::milliseconds(10);
            int oldStatus = status;
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            if (localPosition.x >= 0 && localPosition.x <= 1920 && localPosition.y >= -44 && localPosition.y <= 1017)
            {
                auto mouse_pos = sf::Mouse::getPosition(window); // Mouse position relative to the window

                if(status != 5){
                    openMenu = false;
                } 

                if (status == 0 && waitTime == 0)
                {
                    std::list<Menu>::iterator menuIterator = menuList.begin();
                    int listElement;
                    int i = 0;
                    while (menuIterator != menuList.end())
                    {
                        Menu &menuElement = *menuIterator;
                        update = true;
                        if (menuElement.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            menuElement.text.setFillColor(sf::Color(212, 175, 55));
                            waitTime = 5;
                            listElement = i;
                        }
                        else
                        {
                            menuElement.text.setFillColor(sf::Color::White);
                        }
                        *menuIterator = menuElement;
                        ++menuIterator;
                        ++i;
                    }
                    if (update && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        switch (listElement)
                        {
                        case 0:
                            status = 1;
                            break;
                        case 1:
                            status = 2;
                            numberOfPlayers = 2;
                            numPlayer3.setString(std::to_string(numberOfPlayers));
                            numPlayer3.text.setPosition(sf::Vector2f{window_size - numPlayer3.getWidth(), 120});
                            map_size = -1;
                            break;
                        case 2:
                            status = 3;
                            begin = true;
                            fClick = false;
                            break;
                        case 3:
                            status = 4;
                            break;
                        case 4:
                            window.close();
                            break;
                        }
                        waitTime = 0;
                        update = true;
                    }
                }

                if (status == 2 && waitTime == 0)
                {
                    if (numberOfPlayers < 3)
                    {
                        numPlayer7.text.setPosition(sf::Vector2f{window_size - numPlayer7.getWidth(), 320});
                        numPlayer6.text.setPosition(sf::Vector2f{window_size - numPlayer7.getWidth() - numPlayer6.getWidth() * 2 - 100, 320});
                        numPlayer8.text.setPosition(sf::Vector2f{window_size + numPlayer7.getWidth() + 100, 320});
                    }
                    else if (numberOfPlayers < 6)
                    {
                        numPlayer7.text.setPosition(sf::Vector2f{window_size - numPlayer7.getWidth() * 2 - 50, 320});
                        numPlayer8.text.setPosition(sf::Vector2f{window_size + 50, 320});
                    }
                    else
                    {
                        numPlayer8.text.setPosition(sf::Vector2f{window_size - numPlayer8.getWidth(), 320});
                    }
                    if (numPlayer1.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                    {
                        numPlayer1.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            if (numberOfPlayers > 2)
                            {
                                map_size = -1;
                                numberOfPlayers--;
                                numPlayer3.setString(std::to_string(numberOfPlayers));
                                numPlayer3.text.setPosition(sf::Vector2f{window_size - numPlayer3.getWidth(), 120});
                            }
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        numPlayer1.text.setFillColor(sf::Color::White);
                    }

                    if (numPlayer2.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                    {
                        numPlayer2.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            if (numberOfPlayers < 10)
                            {
                                map_size = -1;
                                numberOfPlayers++;
                                numPlayer3.setString(std::to_string(numberOfPlayers));
                                numPlayer3.text.setPosition(sf::Vector2f{window_size - numPlayer3.getWidth(), 120});
                            }
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        numPlayer2.text.setFillColor(sf::Color::White);
                    }

                    if ((numPlayer6.text.getGlobalBounds().contains((sf::Vector2f)localPosition) || map_size == 1))
                    {
                        numPlayer6.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            map_size = 1;
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        numPlayer6.text.setFillColor(sf::Color::White);
                    }
                    if ((numPlayer7.text.getGlobalBounds().contains((sf::Vector2f)localPosition) || map_size == 2))
                    {
                        numPlayer7.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            map_size = 2;
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        numPlayer7.text.setFillColor(sf::Color::White);
                    }
                    if ((numPlayer8.text.getGlobalBounds().contains((sf::Vector2f)localPosition) || map_size == 3))
                    {
                        numPlayer8.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            map_size = 3;
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        numPlayer8.text.setFillColor(sf::Color::White);
                    }

                    if ((numPlayer4.text.getGlobalBounds().contains((sf::Vector2f)localPosition) && map_size != -1))
                    {
                        numPlayer4.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            x_correction = 0;
                            y_correction = 0;
                            endGame = false;
                            update = true;
                            waitTime = 20;
                            status = 5;
                            inGameMenu10.setString("");
                            GenerateWorld gW;
                            if (map_size == 1)
                            {
                                gW.width = 25;
                                gW.heigth = 25;
                                gW.l_width = 23;
                                gW.l_heigth = 23;
                                gW.numberOfLocations = 8;
                            }
                            if (map_size == 2)
                            {
                                gW.width = 40;
                                gW.heigth = 40;
                                gW.l_width = 38;
                                gW.l_heigth = 38;
                                gW.numberOfLocations = 20;
                            }
                            if (map_size == 3)
                            {
                                gW.width = 50;
                                gW.heigth = 50;
                                gW.l_width = 48;
                                gW.l_heigth = 48;
                                gW.numberOfLocations = 30;
                                gW.map = new int[gW.width * gW.heigth];
                                gW.l_map = new int[gW.l_width * gW.l_heigth];
                            }
                            view.reset(sf::FloatRect(0, 0, 1920, 1080 - 63));
                            view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                            auto saveGame = gW.worldG("../../save_game/" + continueGame, font, renderTexture, unitsTexture, locationsTexture, worldTexture, numberOfPlayers);
                            worldArray = std::get<0>(saveGame);
                            playerColors = std::get<1>(saveGame);
                            unitStats = std::get<2>(saveGame);
                            rows = std::get<3>(saveGame);
                            columns = std::get<4>(saveGame);
                            numberOfPlayers = std::get<5>(saveGame);
                            currentPlayer = std::get<6>(saveGame);
                            incomeOfPlayers = std::get<7>(saveGame);
                            inGameMenu9.setString("10");
                            input = "newWorld";
                            gW.showMap();
                            locationsList = gW.locationList;
                            unitList = gW.unitList;
                            mapList = gW.mapList;
                            roadList = gW.road;
                            nextPlayer.setFillColor(sf::Color(playerColors[(currentPlayer - 1) * 4], playerColors[(currentPlayer - 1) * 4 + 1], playerColors[(currentPlayer - 1) * 4 + 2], playerColors[(currentPlayer - 1) * 4 + 3]));
                            ;
                            renderTexture.display();
                        }
                    }
                    else
                    {
                        numPlayer4.text.setFillColor(sf::Color::White);
                    }
                    if (backButton.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                    {
                        backButton.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            status = 0;
                            waitTime = 20;
                        }
                    }
                    else
                    {
                        if (backButton.text.getFillColor() != sf::Color::White)
                        {
                            backButton.text.setFillColor(sf::Color::White);
                        }
                    }
                    update = true;
                }
                if (status == 4 && waitTime == 0)
                {
                    if (backButton.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                    {
                        backButton.text.setFillColor(sf::Color(212, 175, 55));
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            status = 0;
                            waitTime = 20;
                        }
                        update = true;
                    }
                    else
                    {
                        if (backButton.text.getFillColor() != sf::Color::White)
                        {
                            backButton.text.setFillColor(sf::Color::White);
                            update = true;
                        }
                    }
                }
                if (status == 5 && waitTime == 0)
                {
                    if (localMovingList.empty())
                    {
                        if (endGame)
                        {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                            {
                                status = 0;
                                update = true;
                                waitTime = 10;
                            }
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        {
                            if (openMenu)
                            {
                                openMenu = false;
                            }
                            else
                            {
                                openMenu = true;
                            }

                            update = true;
                            waitTime = 20;
                        }
                        if (openMenu)
                        {
                            if (menuStatus == 0)
                            {
                                if (inGameMenu1.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                {
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        openMenu = false;
                                        update = true;
                                        waitTime = 20;
                                    }
                                }
                                if (inGameMenu2.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                { //SaveGame
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        menuStatus = 1;
                                        update = true;
                                        waitTime = 20;
                                    }
                                }
                                if (inGameMenu3.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                { //Load Game
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        status = 3;
                                        begin = true;
                                        update = true;
                                        waitTime = 20;
                                    }
                                }
                                if (inGameMenu4.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                {
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        openMenu = false;
                                        update = true;
                                        waitTime = 20;
                                        status = 0;
                                    }
                                }
                                if (inGameMenu5.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                {
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        window.close();
                                    }
                                }
                            }
                            else if (menuStatus == 1 && waitTime == 0)
                            {
                                Keyboard keyPressing;
                                input = keyPressing.keyboardInput(input);
                                update = true;
                                waitTime = 7;
                                inGameMenu7.setString(input);
                                inGameMenu7.text.setPosition((1920 - inGameMenu7.text.getLocalBounds().width) / 2, 350);

                                if (inGameMenu5.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                {
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        update = true;
                                        waitTime = 20;
                                        menuStatus = 0;
                                    }
                                }

                                if (inGameMenu6.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                                {
                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                    {
                                        update = true;
                                        waitTime = 20;
                                        menuStatus = 0;
                                        loadArray.writeSaveGame(input, locationsList, unitList, rows, columns, worldArray, numberOfPlayers, currentPlayer, incomeOfPlayers, roadList);
                                        loadArray.writeInformationFile("../../" + save_game + "/informationFile.txt", input);
                                        continueGame = input;
                                    }
                                }
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            start_correction = false;
                            x_correction_local = 0;
                            y_correction_local = 0;
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && waitTime == 0)
                            {
                                if (x_correction > ((signed)window.getSize().x - (columns * 90 + 45 + 6)))
                                {
                                    x_correction -= 1;
                                    x_correction_local -= 1;
                                    start_correction = true;
                                    view.move(1.f, 0.f);
                                    updateTime = sf::milliseconds(2);
                                }
                            }
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && waitTime == 0)
                            {
                                if (x_correction < 0)
                                {
                                    x_correction += 1;
                                    x_correction_local += 1;
                                    start_correction = true;
                                    view.move(-1.f, 0.f);
                                    updateTime = sf::milliseconds(2);
                                }
                            }
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && waitTime == 0)
                            {
                                if (y_correction < 0)
                                {
                                    y_correction += 1;
                                    y_correction_local += 1;
                                    start_correction = true;
                                    view.move(0.f, -1.f);
                                    updateTime = sf::milliseconds(2);
                                }
                            }
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && waitTime == 0)
                            {
                                if (y_correction > (((signed)window.getSize().y) - (rows * 80 + 4 + 22)))
                                { //44
                                    y_correction -= 1;
                                    y_correction_local -= 1;
                                    view.move(0.f, 1.f);
                                    start_correction = true;
                                    updateTime = sf::milliseconds(2);
                                }
                            }
                            if (start_correction)
                            {
                                update = true;
                                waitTime = 1;

                                std::list<Unit>::iterator unitListIterator = unitList.begin();
                                while (unitListIterator != unitList.end())
                                {
                                    Unit &unitListElement = *unitListIterator;
                                    unitListElement.setPosition(sf::Vector2f{unitListElement.getPosition().x + x_correction_local, unitListElement.getPosition().y + y_correction_local});
                                    *unitListIterator = unitListElement;
                                    ++unitListIterator;
                                }

                                std::list<Unit>::iterator movementListIterator = movementList.begin();
                                while (movementListIterator != movementList.end())
                                {
                                    Unit &movementListElement = *movementListIterator;
                                    movementListElement.setPosition(sf::Vector2f{movementListElement.getPosition().x + x_correction_local, movementListElement.getPosition().y + y_correction_local});
                                    *movementListIterator = movementListElement;
                                    ++movementListIterator;
                                }
                            }

                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && waitTime == 0)
                            {
                                if ((1810 + localPosition.y) <= localPosition.x && 110 >= localPosition.y)
                                {
                                    bool livingPlayerFound = true;
                                    while (livingPlayerFound)
                                    {
                                        if (currentPlayer == numberOfPlayers)
                                        {
                                            currentPlayer = 1;
                                            std::list<Unit>::iterator after1 = unitList.begin();
                                            while (after1 != unitList.end())
                                            {
                                                Unit &afterRound = *after1;
                                                afterRound.attacks = afterRound.maxAttacks;
                                                afterRound.movePoints = afterRound.maxmovePoints;
                                                for (Location l13 : locationsList)
                                                {
                                                    if (l13.x == afterRound.x_Coordinate && l13.y == afterRound.y_Coordinate)
                                                    {
                                                        if (afterRound.healthPoints < afterRound.maxhealthPoints)
                                                        {
                                                            afterRound.healthPoints += 1;
                                                        }
                                                        break;
                                                    }
                                                }
                                                afterRound.setString(afterRound.movePoints, afterRound.attacks, afterRound.healthPoints);
                                                *after1 = afterRound;
                                                ++after1;
                                                if (afterRound.owner == currentPlayer)
                                                {
                                                    livingPlayerFound = false;
                                                }
                                            }
                                            for (Location l11 : locationsList)
                                            {
                                                incomeOfPlayers[l11.owner - 1] += 10;
                                                if (l11.owner == currentPlayer)
                                                {
                                                    livingPlayerFound = false;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            currentPlayer++;
                                            for (Unit u11 : unitList)
                                            {
                                                if (u11.owner == currentPlayer)
                                                {
                                                    livingPlayerFound = false;
                                                }
                                            }
                                            for (Location l12 : locationsList)
                                            {
                                                if (l12.owner == currentPlayer)
                                                {
                                                    livingPlayerFound = false;
                                                }
                                            }
                                        }
                                    }

                                    waitTime = 20;
                                    nextPlayer.setFillColor(sf::Color(playerColors[(currentPlayer - 1) * 4], playerColors[(currentPlayer - 1) * 4 + 1], playerColors[(currentPlayer - 1) * 4 + 2], playerColors[(currentPlayer - 1) * 4 + 3]));
                                    bool boolFirstPlayer = true;

                                    endGame = true;
                                    for (Location l3 : locationsList)
                                    {
                                        if (boolFirstPlayer)
                                        {
                                            boolFirstPlayer = false;
                                            intFirstPlayer = l3.owner;
                                        }
                                        else
                                        {
                                            if (intFirstPlayer != l3.owner && l3.owner != 0)
                                            {
                                                endGame = false;
                                            }
                                        }
                                    }
                                    inGameMenu9.setString(std::to_string(incomeOfPlayers[currentPlayer - 1]));
                                    inGameMenu9.text.setPosition(sf::Vector2f(330, 35));
                                    if (incomeOfPlayers[currentPlayer - 1] > 99)
                                    {
                                        inGameMenu9.text.setPosition(sf::Vector2f(310, 35));
                                    }
                                    if (incomeOfPlayers[currentPlayer - 1] > 999)
                                    {
                                        inGameMenu9.text.setPosition(sf::Vector2f(290, 35));
                                    }
                                }

                                auto position = determinePosition(localPosition, x_correction, y_correction);

                                if (firstClick)
                                {
                                    int updateHP = 0;

                                    if (buy)
                                    {
                                        bool placeUnit = false;
                                        for (Location x : locationsList)
                                        {
                                            if (x.x == std::get<0>(position) && x.y == std::get<1>(position) && x.owner == currentPlayer)
                                            {
                                                placeUnit = true;
                                                for (Unit y : unitList)
                                                {
                                                    if (y.x_Coordinate == std::get<0>(position) && y.y_Coordinate == std::get<1>(position))
                                                    {
                                                        placeUnit = false;
                                                    }
                                                }
                                            }
                                        }
                                        if (placeUnit)
                                        {
                                            Unit newUnit;
                                            newUnit.owner = currentPlayer;
                                            newUnit.x_Coordinate = (int)std::get<0>(position);
                                            newUnit.y_Coordinate = (int)std::get<1>(position);

                                            if (buyUnitType == 1)
                                            {
                                                newUnit.type = 1;
                                                newUnit.healthPoints = 8;
                                                newUnit.maxhealthPoints = 8;
                                                newUnit.movePoints = 0;
                                                newUnit.maxmovePoints = 6;
                                                newUnit.attacks = 0;
                                                newUnit.maxAttacks = 1;
                                            }
                                            else if (buyUnitType == 2)
                                            {
                                                newUnit.type = 2;
                                                newUnit.healthPoints = 8;
                                                newUnit.maxhealthPoints = 8;
                                                newUnit.movePoints = 0;
                                                newUnit.maxmovePoints = 4;
                                                newUnit.attacks = 0;
                                                newUnit.maxAttacks = 1;
                                            }
                                            else if (buyUnitType == 3)
                                            {
                                                newUnit.type = 3;
                                                newUnit.healthPoints = 8;
                                                newUnit.maxhealthPoints = 8;
                                                newUnit.movePoints = 0;
                                                newUnit.maxmovePoints = 4;
                                                newUnit.attacks = 0;
                                                newUnit.maxAttacks = 1;
                                            }
                                            else
                                            {
                                                newUnit.type = 4;
                                                newUnit.healthPoints = 8;
                                                newUnit.maxhealthPoints = 8;
                                                newUnit.movePoints = 0;
                                                newUnit.maxmovePoints = 6;
                                                newUnit.attacks = 0;
                                                newUnit.maxAttacks = 2;
                                            }
                                            newUnit.setPosition(converter(sf::Vector2f{std::get<0>(position), std::get<1>(position)}, x_correction, y_correction));
                                            newUnit.setFont(font);
                                            newUnit.setString(newUnit.movePoints, newUnit.attacks, newUnit.healthPoints);
                                            newUnit.setTexture(&unitsTexture);
                                            newUnit.setTextureRect(sf::IntRect{13, 0, 145, 200});
                                            newUnit.setFillColor(sf::Color(playerColors[(newUnit.owner - 1) * 4], playerColors[(newUnit.owner - 1) * 4 + 1], playerColors[(newUnit.owner - 1) * 4 + 2], playerColors[(newUnit.owner - 1) * 4 + 3]));
                                            unitList.push_back(newUnit);
                                            if (buyUnitType == 1)
                                            {
                                                incomeOfPlayers[newUnit.owner - 1] -= 10;
                                            }
                                            else if (buyUnitType == 4)
                                            {
                                                incomeOfPlayers[newUnit.owner - 1] -= 40;
                                            }
                                            else
                                            {
                                                incomeOfPlayers[newUnit.owner - 1] -= 20;
                                            }

                                            inGameMenu9.setString(std::to_string(incomeOfPlayers[newUnit.owner - 1]));
                                        }
                                        buy = false;
                                        firstClick = false;
                                    }
                                    else
                                    {
                                        if (firstPosition == sf::Vector2f{std::get<0>(position), std::get<1>(position)})
                                        {
                                            firstClick = false; //click a secound time on the same figure
                                        }
                                        else
                                        {
                                            bool empty = true; //click a secound time not on the same figure
                                            int defenseBonus = 0;
                                            std::list<Unit>::iterator i1 = unitList.begin();
                                            while (i1 != unitList.end())
                                            {
                                                Unit &i2 = *i1;
                                                if (i2.x_Coordinate == std::get<0>(position) && i2.y_Coordinate == std::get<1>(position))
                                                {
                                                    firstClick = false; //another figure stand on the same position
                                                    empty = false;
                                                    if (i2.owner != currentPlayer)
                                                    { //fight!!!!
                                                        std::list<Unit>::iterator i3 = unitList.begin();
                                                        update = true;
                                                        while (i3 != unitList.end())
                                                        {
                                                            Unit &i4 = *i3;

                                                            if (firstPosition == sf::Vector2f{(float)i4.x_Coordinate, (float)i4.y_Coordinate} && i4.attacks > 0)
                                                            {
                                                                bool enemyFound = false;
                                                                for (AvailableFields attackFields : attackList)
                                                                {
                                                                    if (i2.x_Coordinate == attackFields.x && i2.y_Coordinate == attackFields.y)
                                                                    {
                                                                        enemyFound = true;
                                                                        break;
                                                                    }
                                                                }
                                                                if (enemyFound)
                                                                {
                                                                    if (i4.attacks > 0)
                                                                    {
                                                                        i4.attacks--;
                                                                        i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                        update = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        break;
                                                                    }
                                                                    int randomBit = (rand() % 3) - 1;
                                                                    //terrain
                                                                    if (worldArray[i2.y_Coordinate * columns + i2.x_Coordinate] == 3)
                                                                    {
                                                                        defenseBonus = 1;
                                                                    }
                                                                    for (Location l8 : locationsList)
                                                                    {
                                                                        if (l8.x == i2.x_Coordinate && l8.y == i2.y_Coordinate)
                                                                        {
                                                                            defenseBonus = 1;
                                                                            break;
                                                                        }
                                                                    }
                                                                    fightInformation = "";

                                                                    if ((unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2] - defenseBonus + randomBit) > 0)
                                                                    {
                                                                        i2.healthPoints -= unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2] - defenseBonus + randomBit;
                                                                        i2.setString(i2.movePoints, i2.attacks, i2.healthPoints);
                                                                    }
                                                                    if (randomBit > -1)
                                                                    {
                                                                        fightInformation = "Attacker: " + std::to_string(unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2]) + " AV - " + std::to_string(defenseBonus) + " D + " + std::to_string(randomBit) + " R";
                                                                    }
                                                                    else
                                                                    {
                                                                        fightInformation = "Attacker: " + std::to_string(unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2]) + " AV - " + std::to_string(defenseBonus) + " D - 1 R";
                                                                    }
                                                                    if ((unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2] - defenseBonus + randomBit) > 0)
                                                                    {
                                                                        fightInformation += " = " + std::to_string((unitStats[(i4.type - 1) * 20 + (i2.type - 1) * 2] - defenseBonus + randomBit)) + "\n";
                                                                    }
                                                                    else
                                                                    {
                                                                        fightInformation += " = 0 \n";
                                                                    }
                                                                    bool counterAttack = true;
                                                                    randomBit = (rand() % 3) - 1;
                                                                    if ((unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit) > 0)
                                                                    {
                                                                        if (i2.y_Coordinate % 2 == 0)
                                                                        {
                                                                            if ((i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) || (i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate) || (i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate))
                                                                            {
                                                                                i4.healthPoints -= unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit;
                                                                                i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                            }
                                                                            else
                                                                            {
                                                                                if (i2.type == 2)
                                                                                {
                                                                                    i4.healthPoints -= unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit;
                                                                                    i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                                }
                                                                                else
                                                                                {
                                                                                    counterAttack = false;
                                                                                    i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                                }
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            if ((i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate))
                                                                            {
                                                                                i4.healthPoints -= unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit;
                                                                                i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                            }
                                                                            else
                                                                            {
                                                                                if (i2.type == 2)
                                                                                {
                                                                                    i4.healthPoints -= unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit;
                                                                                    i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                                }
                                                                                else
                                                                                {
                                                                                    counterAttack = false;
                                                                                    i4.setString(i4.movePoints, i4.attacks, i4.healthPoints);
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        if (i2.y_Coordinate % 2 == 0)
                                                                        {
                                                                            if ((i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate))
                                                                            {
                                                                            }
                                                                            else
                                                                            {
                                                                                if (i2.type == 2)
                                                                                {
                                                                                }
                                                                                else
                                                                                {
                                                                                    counterAttack = false;
                                                                                }
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            if ((i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate - 1 == i4.y_Coordinate) || (i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate) ||
                                                                                (i2.x_Coordinate - 1 == i4.x_Coordinate && i2.y_Coordinate + 1 == i4.y_Coordinate) || (i2.x_Coordinate + 1 == i4.x_Coordinate && i2.y_Coordinate == i4.y_Coordinate))
                                                                            {
                                                                            }
                                                                            else
                                                                            {
                                                                                if (i2.type == 2)
                                                                                {
                                                                                }
                                                                                else
                                                                                {
                                                                                    counterAttack = false;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    if (randomBit > -1 && counterAttack)
                                                                    {
                                                                        fightInformation += "Defender: " + std::to_string(unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2]) + " AV + " + std::to_string(randomBit) + " R";
                                                                    }
                                                                    else
                                                                    {
                                                                        if (counterAttack)
                                                                        {
                                                                            fightInformation += "Defender: " + std::to_string(unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2]) + " AV - 1 R";
                                                                        }
                                                                        else
                                                                        {
                                                                            fightInformation += "Defender: 0 AV + 0 R";
                                                                        }
                                                                    }
                                                                    if ((unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit) > 0 && counterAttack)
                                                                    {
                                                                        fightInformation += " = " + std::to_string((unitStats[(i2.type - 1) * 20 + (i4.type - 1) * 2] + randomBit)) + "";
                                                                    }
                                                                    else
                                                                    {
                                                                        fightInformation += " = 0";
                                                                    }
                                                                    inGameMenu10.setString(fightInformation);
                                                                    *i1 = i2;
                                                                    *i3 = i4;
                                                                    if (i2.healthPoints <= 0)
                                                                    {
                                                                        unitList.erase(i1);
                                                                    }
                                                                    if (i4.healthPoints <= 0)
                                                                    {
                                                                        unitList.erase(i3);
                                                                    }
                                                                }

                                                                break;
                                                            }
                                                            ++i3;
                                                        }
                                                    }
                                                    break;
                                                }
                                                ++i1;
                                            }
                                            if (worldArray[(int)std::get<1>(position) * columns + (int)std::get<0>(position)] == 0 || worldArray[(int)std::get<1>(position) * columns + (int)std::get<0>(position)] == 2)
                                            {
                                                empty = false;
                                            }
                                            if (empty)
                                            {
                                                empty = false;
                                                firstClick = false;
                                                for (AvailableFields a : availableList)
                                                {
                                                    if (a.x == std::get<0>(position) && a.y == std::get<1>(position))
                                                    {
                                                        localMovingList.clear();
                                                        AvailableFields a14;
                                                        updateHP = a.movingPoints;
                                                        firstClick = true;
                                                        empty = true;
                                                        bool findWay = true;
                                                        goal_x = a.x;
                                                        goal_y = a.y;
                                                        int local_x = a.x;
                                                        int local_y = a.y;
                                                        int local_x_predecessor = a.x_predecessor;
                                                        int local_y_predecessor = a.y_predecessor;
                                                        while (findWay)
                                                        {
                                                            if (firstPosition.x == local_x_predecessor && firstPosition.y == local_y_predecessor)
                                                            {
                                                                findWay = false;
                                                            }
                                                            a14.x = (int)converter(sf::Vector2f{(float)local_x, (float)local_y}, 0, 0).x;
                                                            a14.y = (int)converter(sf::Vector2f{(float)local_x_predecessor, (float)local_y_predecessor}, 0, 0).y;
                                                            sf::Vector2f currentVector = converter(sf::Vector2f{(float)local_x, (float)local_y}, 0, 0);
                                                            sf::Vector2f predecessorVector = converter(sf::Vector2f{(float)local_x_predecessor, (float)local_y_predecessor}, 0, 0);
                                                            int nicenessOfTheMovement = 5;
                                                            float x_difference = (currentVector.x - predecessorVector.x) / nicenessOfTheMovement;
                                                            float y_difference = (currentVector.y - predecessorVector.y) / nicenessOfTheMovement;
                                                            for (int p = 0; p < nicenessOfTheMovement; p++)
                                                            {
                                                                a14.x = (int)x_difference;
                                                                a14.y = (int)y_difference;
                                                                localMovingList.push_front(a14);
                                                            }

                                                            for (Location l15 : locationsList)
                                                            {
                                                                if (l15.x == local_x && l15.y == local_y && l15.owner != currentPlayer)
                                                                {
                                                                    conquerCity = true;
                                                                    conquerCity_x = l15.x;
                                                                    conquerCity_y = l15.y;
                                                                    break;
                                                                }
                                                            }

                                                            for (AvailableFields a12 : availableList2)
                                                            {
                                                                if (local_x_predecessor == a12.x && local_y_predecessor == a12.y)
                                                                {
                                                                    local_x = a12.x;
                                                                    local_y = a12.y;
                                                                    local_x_predecessor = a12.x_predecessor;
                                                                    local_y_predecessor = a12.y_predecessor;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                            if (empty)
                                            {
                                                sf::Vector2f newPosition = converter(sf::Vector2f{std::get<0>(position), std::get<1>(position)}, x_correction, y_correction);
                                                std::list<Unit>::iterator b = unitList.begin();
                                                std::list<Location>::iterator i2 = locationsList.begin();
                                                while (b != unitList.end())
                                                {
                                                    Unit &update_x = *b;
                                                    if (update_x.x_Coordinate == firstPosition.x && update_x.y_Coordinate == firstPosition.y)
                                                    {
                                                        update_x.movePoints = updateHP;
                                                        update_x.setString(update_x.movePoints, update_x.attacks, update_x.healthPoints);
                                                        update_x.x_Coordinate = (int)std::get<0>(position);
                                                        update_x.y_Coordinate = (int)std::get<1>(position);
                                                        *b = update_x;
                                                        unitList.splice(unitList.end(), unitList, b);
                                                        firstClick = false;
                                                        break;
                                                    }
                                                    ++b;
                                                }
                                            }
                                        }
                                    }

                                    movementList.clear();
                                    attackText.clear();
                                    waitTime = 20;
                                }
                                else
                                {

                                    movementList.clear();
                                    availableList.clear();
                                    availableList2.clear();
                                    attackList.clear();
                                    openList.clear();
                                    for (Unit x : unitList)
                                    {
                                        if (x.x_Coordinate == std::get<0>(position) && x.y_Coordinate == std::get<1>(position) && x.owner == currentPlayer)
                                        {
                                            firstClick = true;
                                            waitTime = 20;
                                            firstPosition = sf::Vector2f{std::get<0>(position), std::get<1>(position)};
                                            firstUnit = x;

                                            availableF.movingPoints = x.movePoints;
                                            availableF.x = x.x_Coordinate;
                                            availableF.y = x.y_Coordinate;
                                            availableF.x_predecessor = 0;
                                            availableF.y_predecessor = 0;
                                            availableList.push_back(availableF);
                                            availableList2.push_back(availableF);
                                            attackF.movingPoints = 1;
                                            if (x.type == 2)
                                            {
                                                attackF.movingPoints = 3;
                                            }

                                            attackF.x = x.x_Coordinate;
                                            attackF.y = x.y_Coordinate;
                                            attackF.x_predecessor = 0;
                                            attackF.y_predecessor = 0;
                                            attackList.push_back(attackF);

                                            openF.x = x.x_Coordinate;
                                            openF.y = x.y_Coordinate;
                                            openF.movingPoints = x.movePoints;
                                            openList.push_back(openF);

                                            auto movementResults = m.movement(openList, availableList, worldArray, unitList, columns, currentPlayer, true);
                                            openList.clear();
                                            openList.push_back(openF);
                                            auto movementResults2 = m.movement(openList, availableList2, worldArray, unitList, columns, currentPlayer, false);
                                            openList.clear();
                                            openF.x = x.x_Coordinate;
                                            openF.y = x.y_Coordinate;
                                            openF.movingPoints = 1;
                                            if (x.type == 2)
                                            {
                                                openF.movingPoints = 3;
                                            }
                                            openList.push_back(openF);
                                            auto attackResults = m.movementAttack(openList, attackList, worldArray, unitList, columns);
                                            attackList = std::get<1>(attackResults);

                                            availableList = std::get<1>(movementResults);
                                            availableList2 = std::get<1>(movementResults2);
                                            for (AvailableFields a : availableList)
                                            {
                                                Unit moveField;
                                                sf::Vector2f newPosition = converter(sf::Vector2f{(float)a.x, (float)a.y}, x_correction, y_correction);
                                                moveField.x_Coordinate = a.x;
                                                moveField.y_Coordinate = a.y;
                                                moveField.setPosition(newPosition);
                                                moveField.setTexture(&moveTexture);
                                                moveField.setTextureRect(sf::IntRect{0, 0, 145, 145});
                                                movementList.push_back(moveField);
                                            }

                                            attackText.clear();
                                            if (x.attacks > 0)
                                            {
                                                for (Unit u15 : unitList)
                                                {
                                                    for (AvailableFields a : attackList)
                                                    {
                                                        if (u15.x_Coordinate == a.x && u15.y_Coordinate == a.y && currentPlayer != u15.owner)
                                                        {
                                                            sf::Text attackTextElement;
                                                            attackTextElement.setFont(font);
                                                            attackTextElement.setCharacterSize(40);
                                                            std::string attackTextString = "";

                                                            int defenseBonus = 0;
                                                            if (worldArray[u15.y_Coordinate * columns + u15.x_Coordinate] == 3)
                                                            {
                                                                defenseBonus = 1;
                                                            }
                                                            for (Location l8 : locationsList)
                                                            {
                                                                if (l8.x == u15.x_Coordinate && l8.y == u15.y_Coordinate)
                                                                {
                                                                    defenseBonus = 1;
                                                                    break;
                                                                }
                                                            }

                                                            if ((unitStats[(u15.type - 1) * 20 + (x.type - 1) * 2]) > 0)
                                                            {
                                                                attackTextString += std::to_string(unitStats[(u15.type - 1) * 20 + (x.type - 1) * 2]);
                                                            }
                                                            else
                                                            {
                                                                attackTextString += "0";
                                                            }
                                                            if (x.type == 2 && (a.movingPoints < 2))
                                                            {
                                                                attackTextString = "0";
                                                            }
                                                            attackTextString += ":";
                                                            if ((unitStats[(x.type - 1) * 20 + (u15.type - 1) * 2] - defenseBonus) > 0)
                                                            {
                                                                attackTextString += std::to_string(unitStats[(x.type - 1) * 20 + (u15.type - 1) * 2] - defenseBonus);
                                                            }
                                                            else
                                                            {
                                                                attackTextString += "0";
                                                            }
                                                            attackTextElement.setString(attackTextString);
                                                            sf::Vector2f textPosition = converter(sf::Vector2f{(float)a.x, (float)a.y}, x_correction_local, y_correction_local);
                                                            attackTextElement.setPosition(sf::Vector2f{textPosition.x + 20, textPosition.y + 20});
                                                            attackText.push_back(attackTextElement);
                                                            break;
                                                        }
                                                    }
                                                }
                                            }

                                            break;
                                        }
                                    }
                                }

                                if (buyUnit.getGlobalBounds().contains((sf::Vector2f)localPosition) && incomeOfPlayers[currentPlayer - 1] >= 10)
                                {
                                    firstClick = true;
                                    buy = true;
                                    buyUnitType = 1;
                                }
                                if (buyUnit2.getGlobalBounds().contains((sf::Vector2f)localPosition) && incomeOfPlayers[currentPlayer - 1] >= 20)
                                {
                                    firstClick = true;
                                    buy = true;
                                    buyUnitType = 2;
                                }
                                if (buyUnit3.getGlobalBounds().contains((sf::Vector2f)localPosition) && incomeOfPlayers[currentPlayer - 1] >= 20)
                                {
                                    firstClick = true;
                                    buy = true;
                                    buyUnitType = 3;
                                }
                                if (buyUnit4.getGlobalBounds().contains((sf::Vector2f)localPosition) && incomeOfPlayers[currentPlayer - 1] >= 40)
                                {
                                    firstClick = true;
                                    buy = true;
                                    buyUnitType = 4;
                                }
                                update = true;
                            }
                        }
                        int touchedUnit2 = touchedUnit;
                        touchedUnit = 0;
                        if (buyUnit.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            buyUnit6.setTextureRect(sf::IntRect{25, 250, 1000, 200});
                            touchedUnit = 1;
                        }
                        if (buyUnit2.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            buyUnit6.setTextureRect(sf::IntRect{25, 450, 1000, 200});
                            touchedUnit = 2;
                        }
                        if (buyUnit3.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            buyUnit6.setTextureRect(sf::IntRect{25, 655, 1000, 200});
                            touchedUnit = 3;
                        }
                        if (buyUnit4.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            buyUnit6.setTextureRect(sf::IntRect{25, 855, 1000, 180});
                            touchedUnit = 4;
                        }
                        if (touchedUnit2 != touchedUnit)
                        {
                            update = true;
                        }

                        if (inGameMenu11.location.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            fightDocumentation = true;
                            update = true;
                        }

                        if (!(inGameMenu10.location.getGlobalBounds().contains((sf::Vector2f)localPosition) && fightDocumentation == true))
                        {
                            fightDocumentation = false;
                            update = true;
                        }
                    }
                    else
                    {
                        update = true;
                        std::list<Unit>::iterator unitListIterator7 = unitList.begin();
                        while (unitListIterator7 != unitList.end())
                        {
                            Unit &unitListElement7 = *unitListIterator7;
                            unitListElement7.unitPicture.setOutlineColor(sf::Color::Transparent);
                            unitListElement7.unit.setOutlineColor(sf::Color::Transparent);
                            if (unitListElement7.x_Coordinate == goal_x && unitListElement7.y_Coordinate == goal_y)
                            {
                                std::list<AvailableFields>::iterator availableFieldsIterator7 = localMovingList.begin();
                                AvailableFields &availableFieldsElement7 = *availableFieldsIterator7;
                                unitListElement7.text.setPosition(unitListElement7.text.getPosition().x + availableFieldsElement7.x, unitListElement7.text.getPosition().y + availableFieldsElement7.y);
                                unitListElement7.unit.setPosition(unitListElement7.unit.getPosition().x + availableFieldsElement7.x, unitListElement7.unit.getPosition().y + availableFieldsElement7.y);
                                unitListElement7.unitPicture.setPosition(unitListElement7.unitPicture.getPosition().x + availableFieldsElement7.x, unitListElement7.unitPicture.getPosition().y + availableFieldsElement7.y);
                                sf::Vector2f cityFind = converter(sf::Vector2f{(float)conquerCity_x, (float)conquerCity_y}, x_correction, y_correction);
                                if (unitListElement7.unit.getPosition().x == cityFind.x && unitListElement7.unit.getPosition().y == cityFind.y && conquerCity)
                                {
                                    conquerCity = false;
                                    std::list<Location>::iterator locationListIterator7 = locationsList.begin();
                                    while (locationListIterator7 != locationsList.end())
                                    {
                                        Location &locationListElement7 = *locationListIterator7;
                                        if (locationListElement7.x == conquerCity_x && locationListElement7.y == conquerCity_y)
                                        {
                                            locationListElement7.owner = unitListElement7.owner;
                                            locationListElement7.setFillColor(sf::Color(playerColors[(unitListElement7.owner - 1) * 4], playerColors[(unitListElement7.owner - 1) * 4 + 1], playerColors[(unitListElement7.owner - 1) * 4 + 2], playerColors[(unitListElement7.owner - 1) * 4 + 3]));
                                        }
                                        *locationListIterator7 = locationListElement7;
                                        ++locationListIterator7;
                                    }
                                }
                                *availableFieldsIterator7 = availableFieldsElement7;
                                if (localMovingList.size() != 1)
                                {
                                    localMovingList.erase(availableFieldsIterator7);
                                }
                                else
                                {
                                    localMovingList.clear();
                                }

                                break;
                            }
                            *unitListIterator7 = unitListElement7;
                            ++unitListIterator7;
                        }
                    }
                }
                if (status == 3 && (waitTime == 0 || begin))
                {

                    if (begin)
                    {
                        loadGameVector.clear();
                        std::vector<std::string> allSaveGames = get_all_files_names_within_folder("../../save_game");
                        begin = false;
                        int counter = 0;
                        bool saveGameExist = false;
                        Menu sg;
                        for (std::string saveGame : allSaveGames)
                        {
                            saveGameExist = true;
                            sg.setFont(font);
                            sg.location.setPosition(sf::Vector2f{45 + 700, (float)270 + 50 * counter});
                            sg.text.setPosition(sf::Vector2f{50 + 700, (float)280 + 50 * counter});
                            sg.setString(saveGame);
                            sg.name = saveGame;
                            sg.location.setTexture(loadGameTexture);
                            sg.location.setScale((float)0.70, (float)0.45);
                            loadGameVector.push_back(sg);
                            counter++;
                        }
                        if (saveGameExist)
                        {
                            sg.location.setPosition(sf::Vector2f{45 + 700, (float)270 + 50 * counter});
                            sg.location.setTexture(loadGameTexture);
                            sg.location.setScale((float)0.70, (float)0.45);
                            loadGameVector.push_back(sg);
                        }
                    }
                    else
                    {
                        if (backButton.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            backButton.text.setFillColor(sf::Color(212, 175, 55));
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                            {
                                status = 0;
                                waitTime = 20;
                            }
                            update = true;
                        }
                        else
                        {
                            if (backButton.text.getFillColor() != sf::Color::White)
                            {
                                backButton.text.setFillColor(sf::Color::White);
                                update = true;
                            }
                        }
                        std::vector<Menu>::iterator vecI = loadGameVector.begin();
                        std::vector<Menu>::iterator vecI2 = loadGameVector.begin();

                        int listElement;
                        int i = 0;
                        bool secoundLine = false;
                        while (vecI != loadGameVector.end())
                        {
                            Menu &menuElement = *vecI;
                            if (secoundLine)
                            {
                                menuElement.location.setColor(sf::Color(212, 175, 55));
                                secoundLine = false;
                            }
                            if (menuElement.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                            {

                                menuElement.text.setFillColor(sf::Color(212, 175, 55));

                                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                {
                                    while (vecI2 != loadGameVector.end())
                                    {
                                        Menu &menuElement2 = *vecI2;
                                        menuElement2.location.setColor(sf::Color::White);
                                        *vecI2 = menuElement2;
                                        ++vecI2;
                                    }
                                    fClick = true;
                                    selectedSaveGame = menuElement.name;

                                    menuElement.location.setColor(sf::Color(212, 175, 55));
                                    waitTime = 20;
                                    secoundLine = true;
                                }

                                update = true;
                                listElement = i;
                            }
                            else
                            {
                                if (menuElement.text.getFillColor() == sf::Color(212, 175, 55))
                                {
                                    menuElement.text.setFillColor(sf::Color::White);
                                    update = true;
                                }
                            }
                            *vecI = menuElement;
                            ++vecI;
                            ++i;
                        }
                        if (deleteGame.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            deleteGame.text.setFillColor(sf::Color(212, 175, 55));
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fClick)
                            {
                                deleteSaveGame(selectedSaveGame);
                                if (selectedSaveGame == continueGame)
                                {
                                    continueGame = "";
                                    loadArray.writeInformationFile("../../" + save_game + "/informationFile.txt", continueGame);
                                }
                                fClick = false;
                                waitTime = 20;
                                begin = true;
                            }
                            update = true;
                        }
                        else
                        {
                            if (deleteGame.text.getFillColor() != sf::Color::White)
                            {
                                deleteGame.text.setFillColor(sf::Color::White);
                                update = true;
                            }
                        }

                        if (loadGame.text.getGlobalBounds().contains((sf::Vector2f)localPosition))
                        {
                            loadGame.text.setFillColor(sf::Color(212, 175, 55));
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fClick)
                            {
                                status = 1;
                                waitTime = 0;
                                continueGame = selectedSaveGame;
                                fClick = false;
                                begin = true;
                            }
                            update = true;
                        }
                        else
                        {
                            if (loadGame.text.getFillColor() != sf::Color::White)
                            {
                                loadGame.text.setFillColor(sf::Color::White);
                                update = true;
                            }
                        }
                    }
                }
                if (status == 1 && waitTime == 0)
                {
                    std::string emptyString = "";
                    if (continueGame == emptyString)
                    {
                        status = 0;
                    }
                    else
                    {
                        x_correction = 0;
                        y_correction = 0;
                        view.reset(sf::FloatRect(0, 0, 1920, 1080 - 63));
                        view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                        endGame = false;
                        status = 5;
                        LoadGame lG;
                        inGameMenu10.setString("");
                        auto saveGame = lG.loadSaveGame("../../save_game/" + continueGame, font, renderTexture, unitsTexture, locationsTexture, worldTexture);
                        worldArray = std::get<0>(saveGame);
                        playerColors = std::get<1>(saveGame);
                        unitStats = std::get<2>(saveGame);
                        rows = std::get<3>(saveGame);
                        columns = std::get<4>(saveGame);
                        numberOfPlayers = std::get<5>(saveGame);
                        currentPlayer = std::get<6>(saveGame);
                        incomeOfPlayers = std::get<7>(saveGame);
                        input = continueGame;

                        locationsList = lG.locationsList;
                        unitList = lG.unitList;
                        mapList = lG.mapList;
                        mapListValue = lG.mapListValue;
                        roadList = lG.roadList;

                        nextPlayer.setFillColor(sf::Color(playerColors[(currentPlayer - 1) * 4], playerColors[(currentPlayer - 1) * 4 + 1], playerColors[(currentPlayer - 1) * 4 + 2], playerColors[(currentPlayer - 1) * 4 + 3]));
                        ;
                        for (Unit &x : unitList)
                        {
                            std::string s = x.text.getString();
                        }

                        renderTexture.display();
                    }
                    update = true;
                }

                if (oldStatus != status && status == 0)
                {
                    std::list<Menu>::iterator menuIterator2 = menuList.begin();
                    while (menuIterator2 != menuList.end())
                    {
                        Menu &menuElement2 = *menuIterator2;
                        update = true;
                        menuElement2.text.setFillColor(sf::Color::White);
                        *menuIterator2 = menuElement2;
                        ++menuIterator2;
                    }
                }

                if (waitTime > 0)
                {
                    waitTime -= 1;
                }
                elapsed -= updateTime;
                //update = true;
            }
            else
            {
                update = true;
                elapsed -= updateTime;
            }
        }

        // only redraw if there has been an update!
        if (update)
        {
            window.clear(sf::Color::White);
            if (status == 0)
            { //start menu
                window.draw(startBackground);
                for (Menu x : menuList)
                {
                    window.draw(x.location);
                    window.draw(x.text);
                }
            }
            else if (status == 2)
            {
                window.draw(createGameBackground);
                window.draw(backButton.text);
                window.draw(numPlayer.text);
                window.draw(numPlayer1.text);
                window.draw(numPlayer2.text);
                window.draw(numPlayer3.text);
                window.draw(numPlayer4.text);
                window.draw(numPlayer5.text);
                if (numberOfPlayers < 3)
                {
                    window.draw(numPlayer6.text);
                }
                if (numberOfPlayers < 6)
                {
                    window.draw(numPlayer7.text);
                }
                window.draw(numPlayer8.text);
            }
            else if (status == 3)
            {
                window.draw(loadGameBackground);
                window.draw(backButton.text);
                window.draw(loadGame.text);
                window.draw(deleteGame.location);
                window.draw(deleteGame.text);
                for (Menu x : loadGameVector)
                {
                    window.draw(x.location);
                    window.draw(x.text);
                }
            }
            else if (status == 4)
            {
                window.draw(creditBackground);
                for (Menu x : credits)
                {
                    window.draw(x.text);
                }
                window.draw(backButton.text);
            }
            else
            {
                window.setView(view);
                window.draw(sf::Sprite(renderTexture.getTexture()));
                window.setView(window.getDefaultView());
                for (Unit x : movementList)
                {
                    window.draw(x.unit);
                }

                window.setView(view);
                for (Location x : locationsList)
                {
                    window.draw(x.location);
                    window.draw(x.text);
                }

                window.setView(window.getDefaultView());
                for (Unit x : unitList)
                {
                    window.draw(x.unit);
                    window.draw(x.unitPicture);
                    window.draw(x.text);
                }
                window.setView(view);
                for (sf::Text x : attackText)
                {
                    window.draw(x);
                }
                window.setView(window.getDefaultView());
                if (endGame)
                {
                    flag.setColor(sf::Color(playerColors[(intFirstPlayer - 1) * 4], playerColors[(intFirstPlayer - 1) * 4 + 1], playerColors[(intFirstPlayer - 1) * 4 + 2], playerColors[(intFirstPlayer - 1) * 4 + 3]));
                    stick.setPosition(sf::Vector2f{window_size - 373 - stick.getGlobalBounds().width + 34, 0});
                    window.draw(stick);
                    stick.setPosition(sf::Vector2f{window_size + 373, 0});
                    window.draw(stick);
                    flag.setPosition(sf::Vector2f{window_size - 400 - 260, 0});
                    window.draw(flag);
                    flag.setPosition(sf::Vector2f{window_size + 400, 0});
                    window.draw(flag);
                    window.draw(inGameMenu8.text);
                }
                else
                {
                    window.draw(buyUnit5);
                    window.draw(nextPlayer);
                    window.draw(buyUnit);
                    window.draw(buyUnit2);
                    window.draw(buyUnit3);
                    window.draw(buyUnit4);
                    window.draw(inGameMenu9.text);

                    if (inGameMenu10.text.getString().getSize() != 0 && fightDocumentation == false)
                    {
                        window.draw(inGameMenu11.location);
                    }
                    if (fightDocumentation == true && inGameMenu10.text.getString().getSize() != 0)
                    {
                        window.draw(inGameMenu10.location);
                        window.draw(inGameMenu10.text);
                    }
                    if (touchedUnit == 1)
                    {
                        window.draw(buyUnit6);
                    }
                    else if (touchedUnit == 2)
                    {
                        window.draw(buyUnit6);
                    }
                    else if (touchedUnit == 3)
                    {
                        window.draw(buyUnit6);
                    }
                    else if (touchedUnit == 4)
                    {
                        window.draw(buyUnit6);
                    }
                }

                if (openMenu && !endGame)
                {
                    window.draw(inGameMenu.location);
                    window.draw(inGameMenu.text);
                    if (menuStatus == 0)
                    {
                        window.draw(inGameMenu1.location);
                        window.draw(inGameMenu1.text);
                        window.draw(inGameMenu2.location);
                        window.draw(inGameMenu2.text);
                        window.draw(inGameMenu3.location);
                        window.draw(inGameMenu3.text);
                        window.draw(inGameMenu4.location);
                        window.draw(inGameMenu4.text);
                        inGameMenu5.setString("Quit");
                        window.draw(inGameMenu5.location);
                        window.draw(inGameMenu5.text);
                    }
                    else
                    {
                        inGameMenu5.setString("Back");
                        window.draw(inGameMenu5.location);
                        window.draw(inGameMenu5.text);
                        window.draw(inGameMenu6.location);
                        window.draw(inGameMenu6.text);
                        window.draw(inGameMenu7.text);
                    }
                }
            }
            window.display();
            update = false;
        }
    }

    return 0;
}