#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sdl2wrapper.h"
#include "utils.h"

static SDL_Surface* load_bmp(const char* filename)
{
   SDL_Surface* surface = SDL_LoadBMP(filename);
   if (surface == NULL)
   {
      fprintf(stderr, "Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
      return NULL;
   }

   return surface;
}

static SDL_Surface* load_png_jpg(Sdl2* sdl2, const char* filename)
{
   SDL_Surface* surface = IMG_Load(filename);
   if (surface == NULL)
   {
      fprintf(stderr, "Unable to load image %s! SDL Error: %s\n", filename, IMG_GetError());
      return NULL;
   }

   // Convert surface to screen format
   SDL_Surface* optimizedSurface = SDL_ConvertSurface(surface, sdl2->screenSurface->format, 0);
   if (optimizedSurface == NULL)
   {
      fprintf(stderr, "Unable to optimize image %s! SDL Error: %s\n", filename, SDL_GetError());
   }

   // Get rid of old loaded surface
   SDL_FreeSurface(surface);

   return optimizedSurface;
}

SDL_Surface* load_image(Sdl2* sdl2, const char* filename)
{
   if (strstr(filename, ".bmp") != NULL)
   {
      return load_bmp(filename);
   }

   if (strstr(filename, ".png") != NULL)
   {
      return load_png_jpg(sdl2, filename);
   }

   if (strstr(filename, ".jpg") != NULL)
   {
      return load_png_jpg(sdl2, filename);
   }

   return NULL;
}

Sdl2* init_sdl2(int width, int height, const char* title)
{
   Sdl2* sdl2 = malloc(sizeof(Sdl2));
   memset(sdl2, 0, sizeof(Sdl2));

   sdl2->width = width;
   sdl2->height = height;

   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      return NULL;
   }

   // Create window
   sdl2->window = SDL_CreateWindow(title, 
                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, SDL_WINDOW_SHOWN);
   if (sdl2->window == NULL)
   {
      fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return NULL;
   }

   // Initialize PNG loading
   int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
   int result = IMG_Init(imgFlags);
   if ((result & imgFlags) != imgFlags)
   {
      fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
      return NULL;
   }

   // Initialize SDL_ttf
   if (TTF_Init() == -1)
   {
      fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
      return NULL;
   }

   // Get window surface
   sdl2->screenSurface = SDL_GetWindowSurface(sdl2->window);

   // Open the font
   sdl2->captionFont = TTF_OpenFont(CAPTIONFONT, 140);
   if (sdl2->captionFont == NULL)
   {
      fprintf(stderr, "Failed to load caption font! SDL_ttf Error: %s\n", TTF_GetError());
      return NULL;
   }

   sdl2->frameTime = SDL_GetTicks();
   sdl2->backgroundColor = COLOR_BG;
   SDL_ShowCursor(SDL_FALSE);

   return sdl2;
}

void cleanup_sdl2(Sdl2* sdl2)
{
   SDL_ShowCursor(SDL_TRUE);

   if (sdl2->window != NULL)
   {
      SDL_DestroyWindow(sdl2->window);
      sdl2->window = NULL;
   }

   if (sdl2->captionFont != NULL)
   {
      TTF_CloseFont(sdl2->captionFont);
      sdl2->captionFont = NULL;
   }

   // Quit SDL subsystems
   TTF_Quit();
   IMG_Quit();
   SDL_Quit();
   
   free(sdl2);
}

bool handle_events(Sdl2* sdl2)
{
   bool quit = false;
   SDL_Event e;
   while (SDL_PollEvent(&e))
   {
      if (e.type == SDL_QUIT)
      {
         quit = true;
      }
      else if (e.type == SDL_MOUSEMOTION)
      {
         // Get the mouse offsets
         sdl2->mouse.mouse_x = e.motion.x;
         sdl2->mouse.mouse_y = e.motion.y;
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN)
      {
         sdl2->mouse.mouse_x = e.button.x;
         sdl2->mouse.mouse_y = e.button.y;

         if (e.button.button == SDL_BUTTON_LEFT)
         {
            sdl2->mouse.button_left = true;
         }

         if (e.button.button == SDL_BUTTON_RIGHT)
         {
            sdl2->mouse.button_right = true;
         }
      }
      else if (e.type == SDL_MOUSEBUTTONUP)
      {
         if (e.button.button == SDL_BUTTON_LEFT)
         {
            sdl2->mouse.button_left = false;
         }

         if (e.button.button == SDL_BUTTON_RIGHT)
         {
            sdl2->mouse.button_right = false;
         }
      }
      else if (e.type == SDL_KEYDOWN)
      {
         for (int c = 0; c < 26; ++c)
         {
            if (e.key.keysym.sym == c + 'a')
            {
               sdl2->keys.letters[c] = true;
            }
         }

         for (int c = 0; c < 10; ++c)
         {
            if (e.key.keysym.sym == c + '0')
            {
               sdl2->keys.numbers[c] = true;
            }
         }

         switch (e.key.keysym.sym)
         {
            case SDLK_SPACE:
               sdl2->keys.space = true;
               break;
            case SDLK_UP:
               sdl2->keys.up = true;
               break;
            case SDLK_DOWN:
               sdl2->keys.down = true;
               break;
            case SDLK_LEFT:
               sdl2->keys.left = true;
               break;
            case SDLK_RIGHT:
               sdl2->keys.right = true;
               break;
         }
      }
      else if (e.type == SDL_KEYUP)
      {
         for (int c = 0; c < 26; ++c)
         {
            if (e.key.keysym.sym == c + 'a')
            {
               sdl2->keys.letters[c] = true;
            }
         }

         for (int c = 0; c < 10; ++c)
         {
            if (e.key.keysym.sym == c + '0')
            {
               sdl2->keys.numbers[c] = true;
            }
         }

         switch (e.key.keysym.sym)
         {
            case SDLK_SPACE:
               sdl2->keys.space = false;
               break;
            case SDLK_UP:
               sdl2->keys.up = false;
               break;
            case SDLK_DOWN:
               sdl2->keys.down = false;
               break;
            case SDLK_LEFT:
               sdl2->keys.left = false;
               break;
            case SDLK_RIGHT:
               sdl2->keys.right = false;
               break;
         }
      }
   }

   return !quit;
}

int frame = 0;

void update_and_delay_sdl2(Sdl2* sdl2)
{
   frame++;
   SDL_UpdateWindowSurface(sdl2->window);

   long newFrameTime = SDL_GetTicks();
   float delayTime = (1000.0 / FRAMES_PER_SECOND) - newFrameTime + sdl2->frameTime;
      if ((frame % 100) == 0)
      {
         printf("Frame %d : %.4fms\n", frame, delayTime);
      }
   if (delayTime > 0.0)
   {

      SDL_Delay(delayTime);
   }

   sdl2->frameTime = SDL_GetTicks();
}

void clear_sdl2(Sdl2* sdl2)
{
   // Fill the surface white
   SDL_FillRect(sdl2->screenSurface, NULL, sdl2->backgroundColor);
}
