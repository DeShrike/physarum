#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sdl2wrapper.h"
#include "time.h"
#include "utils.h"

#define WIDTH 800
#define HEIGHT 600

#define ISDOWN(c) ((c >= 'a' && c <= 'z') ? sdl2->keys.letters[c - 'a'] :  \
                  ((c >= '0' && c <= '9') ? sdl2->keys.numbers[c - '0'] : false))

Sdl2* sdl2 = NULL;
float angle = 0.0f;

Uint32 rgba_to_uint32(int r, int g, int b, int a)
{
   return (a << 24) | (r << 16) | (g << 8) | b;
}

void uint32_to_rgba(Uint32 color, int* r, int* g, int* b, int* a)
{
   *a = (color & 0xFF000000) >> 24;
   *r = (color & 0x00FF0000) >> 16;
   *g = (color & 0x0000FF00) >> 8;
   *b = (color & 0x000000FF);
}

// Function to apply alpha blending
Uint32 alpha_blend(Uint32 background, Uint32 color)
{
   int ra, rr, rg, rb;
   int ca, cr, cg, cb;
   int ba, br, bg, bb;

   uint32_to_rgba(background, &br, &bg, &bb, &ba);
   uint32_to_rgba(color, &cr, &cg, &cb, &ca);
   float alpha = ca / 255.0;

   rr = (unsigned char)(alpha * cr + (1 - alpha) * br);
   rg = (unsigned char)(alpha * cg + (1 - alpha) * bg);
   rb = (unsigned char)(alpha * cb + (1 - alpha) * bb);
   ra = 255; // Set the alpha value to 255 for the result

   return rgba_to_uint32(rr, rg, rb, ra);
}

void put_pixel_and_blend(SDL_Surface *surface, int x, int y, Uint32 color)
{
   if (x < 0 || y < 0)
   {
      return;
   }

   if (x >= surface->w || y >= surface->h)
   {
      return;
   }

   int byteperpixel = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * byteperpixel;

   if ((color & 0xFF000000) == 0xFF000000)
   {
      *(Uint32 *)p = color;
   }
   else
   {
      Uint32 background = *(Uint32 *)p;
      *(Uint32 *)p = alpha_blend(background, color);
   }
}

void put_pixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
   if (x < 0 || y < 0)
   {
      return;
   }

   if (x >= surface->w || y >= surface->h)
   {
      return;
   }

   int byteperpixel = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * byteperpixel;
   *(Uint32 *)p = color;
}

void SDL_DrawLine(SDL_Surface* surface, int pX1, int pY1, int pX2, int pY2, Uint32 color )
{
   int lenX = pX2 - pX1;
   int lenY = pY2 - pY1;
   float vectorLen = sqrtf(lenX * lenX + lenY * lenY);
   if (vectorLen == 0)
   {
      return;
   }
   
   float inclineX = lenX / vectorLen;
   float inclineY = lenY / vectorLen;
   float x = (int)pX1;
   float y = (int)pY1;
   /*
   bool locked = SDL_MUSTLOCK(surface);
   if (locked)
   {
      SDL_LockSurface(surface);
   }*/

   for (int i = 0; i < (int)vectorLen; ++i)
   {
      put_pixel(surface, (int)x, (int)y, color);
      //SDL_PutPixel(surface, (int)x, (int)y, color);
      x += inclineX;
      y += inclineY;
   }

   /*
   if (locked)
   {
      SDL_UnlockSurface(surface);
   }*/
}

void draw_logo(SDL_Surface* logoSurface, SDL_Surface* imgSurface)
{
   SDL_Rect logoRect = {
      .x = (sdl2->screenSurface->w - logoSurface->w) / 2,
      .y = (sdl2->screenSurface->h - logoSurface->h) / 2,
      .w = logoSurface->w,
      .h = logoSurface->h
   };

   float scale = map(sin(angle), -1, 1, 0.1, 0.3);
   angle += 0.1;
   
   SDL_Rect srcRect = {
      .x = 0,
      .y = 0,
      .w = imgSurface->w,
      .h = imgSurface->h
   };

   SDL_Rect dstRect = {
      .x = (sdl2->screenSurface->w - floor(imgSurface->w * scale)) / 2,
      .y = 20,
      .w = floor(imgSurface->w * scale),
      .h = floor(imgSurface->h * scale)
   };

   SDL_BlitScaled(imgSurface, &srcRect, sdl2->screenSurface, &dstRect);
   SDL_BlitSurface(logoSurface, NULL, sdl2->screenSurface, &logoRect);
}

int main(int argc, char* args[])
{
   srand(time(NULL));

   sdl2 = init_sdl2(WIDTH, HEIGHT, "Slime Mold");
   if (sdl2 == NULL)
   {
      fprintf(stderr, "Failed to initialize SDL2 !\n");
      return EXIT_FAILURE;
   }

   SDL_Surface* logoSurface;
   SDL_Surface* imgSurface;

   logoSurface = TTF_RenderText_Blended(sdl2->captionFont, "Slime", (SDL_Color){ 255, 255, 0});
   imgSurface = load_image(sdl2, "./images/logo.png");

   SDL_Delay(100);

   while (true)
   {
      Uint32 currTickTime = SDL_GetTicks();
      float deltaTime = (currTickTime - sdl2->prevTickTime) / 1000.0f;
      sdl2->prevTickTime = currTickTime;

      if (handle_events(sdl2) == false)
      {
         break;
      }

      if (ISDOWN('q') || sdl2->keys.space)
      {
         break;
      }

      clear_sdl2(sdl2);

      draw_logo(logoSurface, imgSurface);

      int y = 0;
      Uint32 colors[] = { COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN, COLOR_YELLOW, COLOR_PURPLE, COLOR_ORANGE, COLOR_WHITE, COLOR_BLACK };
      int colorcount = sizeof(colors) / sizeof(Uint32);
      int thickness = (HEIGHT - 100) / colorcount;
      for (int x = 50; x < WIDTH - 50; ++x)
      {
         for (int c = 0; c < colorcount; ++c)
         {
            y = 50 + (thickness * c);
            for (int t = 0; t < thickness; ++t)
            {
               put_pixel_and_blend(sdl2->screenSurface, x, y + t, colors[c] & 0xB0FFFFFF);
            }
         }
      }

      SDL_DrawLine(sdl2->screenSurface, 0, 0, WIDTH - 1, HEIGHT - 1, COLOR_WHITE);
      SDL_DrawLine(sdl2->screenSurface, WIDTH - 1, 0, 0, HEIGHT - 1, COLOR_WHITE);

      update_and_delay_sdl2(sdl2);
   }

   if (logoSurface != NULL)
   {
      SDL_FreeSurface(logoSurface);
      logoSurface = NULL;
   }

   if (imgSurface != NULL)
   {
      SDL_FreeSurface(imgSurface);
      imgSurface = NULL;
   }

   cleanup_sdl2(sdl2);

   return EXIT_SUCCESS;
}
