// use sdl2 to load a font texture then display it in a window as a timer
// using font.ttf
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// ------------------------------------------------------------
// Global Setup
// ------------------------------------------------------------
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Surface *surface = NULL;
TTF_Font *font = NULL;
SDL_Color color = {0, 0, 0, 255};
SDL_Rect rect = {0, 0, 0, 0};
SDL_Event event;

// ------------------------------------------------------------
// Function Prototypes
// ------------------------------------------------------------
bool init();
void close();
void render(int count);

// ------------------------------------------------------------
// Main Function
// ------------------------------------------------------------

int WinMain(int argc, char *argv[])
{
    if (!init())
    {
        std::cout << "Failed to initialize!" << std::endl;
    }
    else
    {
        bool quit = false;
        int count = 0;
        while (!quit)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
            render(count);
            //wait 1 second
            SDL_Delay(1000);
            count++;
        }
    }
    close();
    return 0;
}

// ------------------------------------------------------------
// Function Definitions
// ------------------------------------------------------------

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                    success = false;
                }
                if (TTF_Init() == -1)
                {
                    std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }
    return success;
}

void close()
{
    SDL_DestroyTexture(texture);
    texture = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    TTF_CloseFont(font);
    font = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void render(int count)
{
    const char* time = std::to_string(count).c_str();
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    font = TTF_OpenFont("assets/font.ttf", 28);
    surface = TTF_RenderText_Solid(font, time, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.x = (640 - rect.w) / 2;
    rect.y = (480 - rect.h) / 2;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}





