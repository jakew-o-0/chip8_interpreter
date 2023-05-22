#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include "../chip8/chip8.h"


struct sdl_window_data {
	SDL_Window* window;
	SDL_Renderer* window_renderer;
	SDL_Event event_stack;
};

sdl_window_data init_sdl_win();
void poll_sdl_events(sdl_window_data &window_data, chip8 &ch8_instance);
void draw_sdl_window(sdl_window_data &window_data, chip8 &ch8_instance);
void clear_sdl_window(sdl_window_data &window_data);
