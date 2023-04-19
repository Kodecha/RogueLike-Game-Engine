#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>

using namespace std;

// widows settings


// global constants
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100;
const int MAX_ROOMS = 40;
const int MAX_ROOM_SIZE = 10;
const int MIN_ROOM_SIZE = 3;
const int FOV = 15;

// global variables
bool gameRunning = false;
// map
char map[MAP_WIDTH][MAP_HEIGHT];
// player
int playerX = 0;
int playerY = 0;
int playerHP = 100;
int playerHunger = 100;
int playerMaxHP = 100;
int playerMaxHunger = 100;
int playerAttack = 10;
int playerDefense = 10;
int playerGold = 0;

// FUNCTION PROTOTYPES
//-----------------------------------------------------
void init();
void loadMap();
void updateGame();
void drawGame();
void inputHandle();

//FUNCTION DEFINITIONS
//-----------------------------------------------------
// initialization function  
void init() {
    loadMap();
    playerX = 3;
    playerY = 3;
    gameRunning = true;
}

// load the map
void loadMap()
{
    // open the map file
    ifstream mapFile("map.txt");
    // check if the file was opened
    if (mapFile.is_open())
    {
        // read the map file
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                mapFile >> map[x][y];
            }
        }
        // close the file
        mapFile.close();
    }
}

// draw the game
void drawGame() {
// clear the screen
system("cls");    
// top border
cout << "+";
for (int i = 0; i < FOV*2+1; i++)
{
    cout << "-";

}
cout << "+";
cout << endl;

/* loop through every tile in the field of view 
* applying ray casting to check if walls are blocking the view of tiles
* then displaying the map centered on the player in the center of the screen
* map screen will be 20x20 border excluded */

for (int y = playerY - FOV; y <= playerY + FOV; y++)
{
    // draw the left border
    cout << "|";
    for (int x = playerX - FOV; x <= playerX + FOV; x++)
    {
        // calculate the distance between the player and the current tile
        double dist = sqrt(pow(x - playerX, 2) + pow(y - playerY, 2));
        // using ray casting, check if a wall is blocked and a hidden tile is visible   
        bool visible = true;
        for (int i = 0; i < 100; i++)
        {
            // calculate the current tile
            int currentX = playerX + (x - playerX) * i / 100;
            int currentY = playerY + (y - playerY) * i / 100;

            // check if the current tile is a wall
            if (map[currentX][currentY] == '#')
            {
                // check if the current tile is closer than the current tile
                if (dist > sqrt(pow(currentX - playerX, 2) + pow(currentY - playerY, 2)))
                {
                    // the wall is blocking the tile
                    visible = false;
                }
            }
        }

        // check if the tile is visible
        if (visible)
        {
            // check if the tile is in the field of view
            if (dist <= FOV)
            {
                // check if the tile is the player
                if (x == playerX && y == playerY)
                {
                    // draw the player
                    cout << "@";
                }
                else
                {
                    // draw the tile
                    cout << map[x][y];
                }
            }
            else
            {
                // draw a hidden tile
                cout << " ";
            }
        }
        else
        {
            // draw a hidden tile
            cout << " ";
        }
    }
    // draw the right border
    cout << "|";
    // draw a new line
    cout << endl;
    } 
    // bottom border
    cout << "+";
    for (int i = 0; i < FOV * 2 + 1; i++)
    {
        cout << "-";

    }
    cout << "+";
    cout << endl;
    // draw the player stats
    cout << "HP: " << playerHP << "/" << playerMaxHP << endl;   
    cout << "Hunger: " << playerHunger << "/" << playerMaxHunger << endl;
}  

// update the game
void updateGame()
{
    // update the player
    // update the hunger
    playerHunger--;
    // check if the player is dead
    if (playerHP <= 0)
    {
        // end the game
        gameRunning = false;
    }
}

// input handle
void inputHandle() {
    // get the input, on move check for # wall, check for monster x and y and initiate onPlayerAttack()
    //take input without enter
    char input;
    input = _getch();
    
    switch (input)
    {
    case 'w':
        // check if the player can move up
        if (map[playerX][playerY - 1] != '#')
        {
            // move the player up
            playerY--;

        }
        break;
    case 'a':
        // check if the player can move left
        if (map[playerX - 1][playerY] != '#')
        {
            // move the player left
            playerX--;

        }
        break;
    case 's':
        // check if the player can move down
        if (map[playerX][playerY + 1] != '#')
        {
            // move the player down
            playerY++;

        }
        break;
    case 'd':
        // check if the player can move right
        if (map[playerX + 1][playerY] != '#')
        {
            // move the player right
            playerX++;

        }
        break;
    case 'q':
        // end the game
        gameRunning = false;
        break;
    }
}
// main function
int main()
{
    // initialize the game
    init();
    // game loop
    while (gameRunning)
    {
        // draw the game
        drawGame();
        // update the game
        updateGame();
        // input handle
        inputHandle();
    }
    // end the game
    return 0;
}









//RogueLike Game by: Brendan Kunderas
//Date: 3/17/2023

/*This is a roguelike game that is based on the game "Dungeon Crawl Stone Soup". 
The game is a turn based game where the player moves around a dungeon and fights monsters. 
The player can pick up items and use them to fight monsters. 
The player can also use items to heal themselves. 
The game is over when the player dies or when the player reaches the end of the dungeon.
The game will have a top down view of the dungeon and will use ASCII characters to represent the player and monsters.
The game will use a turn based system where the player will move and then the monsters will move.


The game will look like this when it is finished:



___________________________________________
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|                                         |
|_________________________________________|
| Health: 10/230       |  STR:            |     
|                      |  DEX:            | 
| Hunger: 4000/4000    |  CON:            | 
|                      |  EXP:            |
| $1000                |  LVL:            |
|______________________|__________________|
The game is written in C++ and uses the NO external libraries.
The source code will be written modularly within the main.cpp file.



The map will use an algorithm to generate the map and will generate special rooms that will have special items and monsters based on txt map files.

Progression:
The game will use a turn based system where the player will move and then the monsters will move.

*/

