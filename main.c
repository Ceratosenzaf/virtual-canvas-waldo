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

int linear_map(int val, int src_min, int src_max, int dst_min, int dst_max) {
  return (int)(dst_min + (float)(dst_max - dst_min) * (val - src_min) /
                             (src_max - src_min));
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

  SDL_Renderer* renderer =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    return print_error_and_return("Error creating renderer", win);
  }

  int render_w, render_h;
  if (SDL_GetRendererOutputSize(renderer, &render_w, &render_h) != 0) {
    return print_error_and_return("Error reading renderer size", win);
  }

  SDL_Texture* img = IMG_LoadTexture(renderer, "image.jpg");
  if (!img) {
    return print_error_and_return("Error loading image", win);
  }

  int img_w, img_h;
  if (SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h) != 0) {
    return print_error_and_return("Error reading image info", win);
  }

  int win_b_t, win_b_l, win_b_b, win_b_r;
  if (SDL_GetWindowBordersSize(win, &win_b_t, &win_b_l, &win_b_b, &win_b_r) !=
      0) {
    return print_error_and_return("Error getting window borders", win);
  }

  SDL_Rect usable;
  if (SDL_GetDisplayUsableBounds(DISPLAY, &usable) != 0) {
    return print_error_and_return("Error getting display bounds", win);
  }

  float scale_x = (float)img_w / usable.w;
  float scale_y = (float)img_h / usable.h;

  int running = 1;
  SDL_Event event;
  while (running) {
    int win_x, win_y;
    SDL_GetWindowPosition(win, &win_x, &win_y);

    int rel_x = linear_map(win_x - usable.x - win_b_l, 0,
                           usable.w - win_b_l - win_b_r, 0, usable.w);
    int rel_y = linear_map(win_y - usable.y - win_b_t, 0,
                           usable.h - win_b_t - win_b_b, 0, usable.h);

    SDL_Rect shown_img = {(int)(rel_x * scale_x), (int)(rel_y * scale_y),
                          (int)(win_w * scale_x), (int)(win_h * scale_y)};
    SDL_Rect full_img = {0, 0, win_w, win_h};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, img, &shown_img, &full_img);
    SDL_RenderPresent(renderer);

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