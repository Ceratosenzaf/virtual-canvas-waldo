#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

const int DISPLAY = 0;
const float DISPLAY_RATIO = 0.08;

int print_error_and_return(char message[], SDL_Window* win) {
  printf("%s: %s\n", message, SDL_GetError());
  if (win) SDL_DestroyWindow(win);
  SDL_Quit();
  return 1;
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return print_error_and_return("SDL init failed", NULL);
  }

  SDL_DisplayMode dm;
  if (SDL_GetCurrentDisplayMode(DISPLAY, &dm) != 0) {
    return print_error_and_return("Error reading display info", NULL);
  }

  int win_w = dm.w * DISPLAY_RATIO;
  int win_h = win_w;
  SDL_Window* win =
      SDL_CreateWindow("Waldo?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       win_w, win_h, SDL_WINDOW_SHOWN);
  if (!win) {
    return print_error_and_return("Error opening window", NULL);
  }

  int running = 1;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }
  }

  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}