#ifndef _SDL2WRAPPER_H_
#define _SDL2WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FRAMES_PER_SECOND 30

#define CAPTIONFONT "./fonts/Fontana.ttf"
//#define CAPTIONFONT "./fonts/Bronson.ttf"
//#define CAPTIONFONT "./fonts/Bogota.ttf"
//#define CAPTIONFONT "./fonts/Discoria.ttf"

#define COLOR_BLACK  0xFF000000
#define COLOR_RED    0xFFFF0000
#define COLOR_GREEN  0xFF00FF00
#define COLOR_BLUE   0xFF0000FF
#define COLOR_CYAN   0xFF00FFFF
#define COLOR_YELLOW 0xFFFFFF00
#define COLOR_PURPLE 0xFFA000FF
#define COLOR_ORANGE 0xFFFFA000
#define COLOR_WHITE  0xFFFFFFFF
#define COLOR_BG     0xFF181818

typedef struct {
   bool space;
   bool up;
   bool down;
   bool left;
   bool right;
   bool letters[26];
   bool numbers[10];
} Keys;

typedef struct {
   int mouse_x;
   int mouse_y;
   bool button_left;
   bool button_right;
} Mouse;

typedef struct {
   int width;
   int height;
   SDL_Window* window;
   SDL_Surface* screenSurface;
   TTF_Font* captionFont;

   Mouse mouse;
   Keys keys;

   Uint32 backgroundColor;

   long frameTime;
   Uint32 prevTickTime;
} Sdl2;

Sdl2* init_sdl2(int width, int height, const char* title);
void cleanup_sdl2(Sdl2* sdl2);
bool handle_events(Sdl2* sdl2);
void update_and_delay_sdl2(Sdl2* sdl2);
void clear_sdl2(Sdl2* sdl2);
//void set_color_sdl2(Sdl2* sdl2, Uint32 color);
SDL_Surface* load_image(Sdl2* sdl2, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
