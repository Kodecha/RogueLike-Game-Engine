#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//------------------------------------------------------------
// Global Setup
//------------------------------------------------------------

// constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 640;
const int MAX_DISPLAY_WIDTH = 800;
const int MAP_DISPLAY_HEIGHT = 608;
const int MAX_HUNGER = 4000;
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100; 
const int FOV = 8;
const int TILE_SIZE = 32;
const int MAX_MONSTERS = 100;
const int MAX_ITEMS = 100;
const SDL_Rect HUD_BORDER = {MAX_DISPLAY_WIDTH - 188, MAP_DISPLAY_HEIGHT - 124, 192, 128};
const SDL_Rect HUD_RECT = {MAX_DISPLAY_WIDTH - 190, MAP_DISPLAY_HEIGHT - 126, 192, 128};
const SDL_Rect statsRectBorder = {MAX_DISPLAY_WIDTH - 196, MAX_DISPLAY_WIDTH - 130, 196, 130};
const SDL_Rect statsRect = {MAX_DISPLAY_WIDTH - 192, MAX_DISPLAY_WIDTH - 128, 192, 128};

//TileSet variables

SDL_Texture* tilesetTexture;
SDL_Texture* vignette;
SDL_Texture* playerTexture;
SDL_Texture* mobTexture;
SDL_Texture* fontTexture;
TTF_Font* font;

// gloabl variables
bool gameRunning = true;
char map[MAP_WIDTH][MAP_HEIGHT];
int movenum = 0;
int enemyCount = 0;

// initialize the global game variables
int playerX = 10;
int playerY = 10;
int playerHP = 100;
int playerMaxHP = 100;
int playerHunger = 4000;

int ratX = 2;
int ratY = 18;

// function prototypes
void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT]);
void drawStats(SDL_Renderer* renderer);
void inputHandle();
void handleAI();
void gameReset();

//------------------------------------------------------------
// Main Function
//------------------------------------------------------------

int WinMain( int argc, char *argv[] )
{

    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( "Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("assets/font.ttf", 12);;    
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDL_Surface* tilesetSurface = IMG_Load("assets/tileset.png");
    // create texture from the surface
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, tilesetSurface);
    // load the player image to a surface
    SDL_Surface* playerSurface = IMG_Load("assets/player.png");

    // create player texture from the surface
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

    // load rat image to a surface
    SDL_Surface* ratSurface = IMG_Load("assets/rat.png");

    // create rat texture from the surface
    mobTexture = SDL_CreateTextureFromSurface(renderer, ratSurface);

    // load vignette image to a surface
    SDL_Surface* vignetteSurface = IMG_Load("assets/vignette.png");

    // create vignette texture from the surface
    vignette = SDL_CreateTextureFromSurface(renderer, vignetteSurface);

    // load the map from a text file
    std::ifstream mapFile("map.txt");
    if (mapFile.is_open())
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                mapFile >> map[x][y];
            }
        }
    }
    
    // main game loop
    while (gameRunning)
    {
        // handle input
        inputHandle();
        //hand ai
        handleAI();
        // draw the game
        drawGame(renderer, map);
        // draw the stats
        drawStats(renderer);
        // update the screen
        SDL_RenderPresent(renderer);
        // wait 16ms
        SDL_Delay(16);
    }
    // clean up
    SDL_DestroyTexture(tilesetTexture);
    SDL_FreeSurface(tilesetSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;

}

//------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------

void handleAI()
{

}

void drawGame(SDL_Renderer* renderer, char map[MAP_WIDTH][MAP_HEIGHT])
{   
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // draw the map
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // calculate the screen coordinates of the tile
            int screenX = x * TILE_SIZE - playerX * TILE_SIZE + MAX_DISPLAY_WIDTH / 2;
            int screenY = y * TILE_SIZE - playerY * TILE_SIZE + MAP_DISPLAY_HEIGHT / 2;
            // use raytracing to determine if a # wall tile is blocking the player's view of the tile
            bool visible = true;
            for (int i = 0; i < FOV; i++)
            {
                int rayX = playerX + (x - playerX) * i / FOV;
                int rayY = playerY + (y - playerY) * i / FOV;
                if (map[rayX][rayY] == '#')
                {
                    visible = false;
                }
            }

            // if the tile is visible and within the player's FOV circle, draw it
            if (visible && sqrt(pow(x - playerX, 2) + pow(y - playerY, 2)) < FOV)
            {
                //TODO: TILE HANDLING NEEDS TO BE MORE MODULAR AND ABLE TO ADD MORE TEXTURES  and  event types
                SDL_Rect tilesetRect = {0, 0, TILE_SIZE, TILE_SIZE};
                if (map[x][y] == '#')
                {
                    tilesetRect.x = 0;
                    tilesetRect.y = 0;
                }
                else if (map[x][y] == '.')
                {
                    tilesetRect.x = TILE_SIZE;
                    tilesetRect.y = 0;
                }
                else if (map[x][y] == 'X')
                {
                    tilesetRect.x = TILE_SIZE * 2;
                    tilesetRect.y = 0;
                }
                else if (map[x][y] == ',')
                {
                    tilesetRect.x = TILE_SIZE * 3;
                    tilesetRect.y = 0;
                }
                // set the tile's screen rectangle
                SDL_Rect tileRect = {screenX, screenY, TILE_SIZE, TILE_SIZE};
                // render the tile
                SDL_RenderCopy(renderer, tilesetTexture, &tilesetRect, &tileRect);
            }
        }
    }
    // draw the player from the player texture
    SDL_Rect playerRect = {MAX_DISPLAY_WIDTH / 2, MAP_DISPLAY_HEIGHT / 2, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    // draw the rat from the rat texture
    SDL_Rect ratRect = {ratX * TILE_SIZE - playerX * TILE_SIZE + MAX_DISPLAY_WIDTH / 2, ratY * TILE_SIZE - playerY * TILE_SIZE + MAP_DISPLAY_HEIGHT / 2, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, mobTexture, NULL, &ratRect);

    // draw the vignette.png to the center of the screen full size. move it to the right 32 pixels
    SDL_Rect vignetteRect = {0, 0, MAX_DISPLAY_WIDTH, MAP_DISPLAY_HEIGHT};
    SDL_RenderCopy(renderer, vignette, NULL, &vignetteRect);
}

void drawStats(SDL_Renderer* renderer)
{

}

void inputHandle() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        //WASD movement ESC to quit Collision with walls #
        switch (event.type) 
        {
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) 
                {
                    case SDLK_ESCAPE:
                        gameRunning = false;
                        break;
                    case SDLK_w:
                        if (map[playerX][playerY - 1] != '#' && map[playerX][playerY - 1] != ' ') 
                        {
                            playerY--;
                            movenum++;
                            playerHunger--;
                        if (playerX == ratX && playerY == ratY) 
                        {
                            ratY--;
                            if (map[ratX][ratY] == '#') //Wall collison for rat!
                            {
                                ratY++;
                                playerY++;
                                movenum--;
                            }
                            if (map[ratX][ratY] == 'X') //boil the rat
                            {
                                ratX = 3;
                                ratY = 19;
                            }
                        }
                        break;
                    case SDLK_a:
                        if (map[playerX - 1][playerY] != '#' && map[playerX][playerY - 1] != ' ') 
                        {
                            playerX--;
                            movenum++;
                            playerHunger--;
                        }
                        if (playerX == ratX && playerY == ratY ) 
                        {
                            ratX--;
                            if (map[ratX][ratY] == '#') 
                            {
                                ratX++;
                                playerX++;
                                movenum--;
                            }
                        }
                        break;
                    case SDLK_s:
                        if (map[playerX][playerY + 1] != '#' && map[playerX][playerY - 1] != ' ') 
                        {
                            playerY++;
                            movenum++;
                            playerHunger--;
                        }
                        if (playerX == ratX && playerY == ratY ) 
                        {
                            ratY++;
                            if (map[ratX][ratY] == '#') 
                            {
                                ratY--;
                                playerY--;
                                movenum--;
                            }
                        }
                        break;
                    case SDLK_d:
                        if (map[playerX + 1][playerY] != '#' && map[playerX][playerY - 1] != ' ') 
                        {
                            playerX++;
                            movenum++;
                        }
                        if (playerX == ratX && playerY == ratY ) 
                        {
                            ratX++;
                            if (map[ratX][ratY] == '#') 
                            {
                                ratX--;
                                playerX--;
                                movenum--;
                            }
                            if (map[ratX][ratY] == 'X') //boil the rat
                            {
                                ratX = 3;
                                ratY = 19;
                            }
                        }
                        break;
                    case SDLK_SPACE:
                        movenum++;
                        playerHunger--;
                        }
                        break;
                    case SDLK_e:
                        gameReset();
                        break;
             }
        }
        break;
    }
}

void gameReset() 
{
playerX = 2;
playerY = 17;
playerHP = 100;
playerHunger = 4000;

ratX = 2;
ratY = 18;

movenum = 0;
}
