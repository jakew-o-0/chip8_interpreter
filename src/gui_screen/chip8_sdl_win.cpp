#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <cstdlib>

#include <vector>

#include "./src/gui_screen/chip8_sdl_win.h"
#include "./src/chip8/chip8.h"



sdl_window_data init_sdl_win() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL has failed to initalise");
		exit(-1);
	}

	sdl_window_data window_data;

	window_data.window = SDL_CreateWindow("chip8 Emulator",
											SDL_WINDOWPOS_CENTERED,
											SDL_WINDOWPOS_CENTERED,
											660,
											340,
											SDL_WINDOW_SHOWN);

	if(window_data.window == nullptr) {
		printf("window fialed to initalise");
		exit(-1);
	}


	window_data.window_renderer = SDL_CreateRenderer(window_data.window,
													-1,
													SDL_RENDERER_ACCELERATED);

	if(window_data.window_renderer== nullptr) {
		printf("window renderer fialed to initalise");
		exit(-1);
	}

	return window_data;
}





void poll_sdl_events(sdl_window_data &window_data, chip8 &ch8_instance) {
	while (SDL_PollEvent(&window_data.event_stack)) {

		switch (window_data.event_stack.type) {
			case SDL_QUIT:
				SDL_DestroyRenderer(window_data.window_renderer);
				SDL_DestroyWindow(window_data.window);
				SDL_Quit();
				exit(0);


			case SDL_KEYUP:
				switch (window_data.event_stack.key.keysym.sym) 
				{
					case SDLK_1:
						ch8_instance.current_key = 0x01; break;
					case SDLK_2:
						ch8_instance.current_key = 0x02; break;
					case SDLK_3:
						ch8_instance.current_key = 0x03; break;
					case SDLK_4:
						ch8_instance.current_key = 0x0C; break;
					case SDLK_q:
						ch8_instance.current_key = 0x04; break;
					case SDLK_w:
						ch8_instance.current_key = 0x05; break;
					case SDLK_e:
						ch8_instance.current_key = 0x06; break;
					case SDLK_r:
						ch8_instance.current_key = 0x0D; break;
					case SDLK_a:
						ch8_instance.current_key = 0x07; break;
					case SDLK_s:
						ch8_instance.current_key = 0x08; break;
					case SDLK_d:
						ch8_instance.current_key = 0x09; break;
					case SDLK_f:
						ch8_instance.current_key = 0x0E; break;
					case SDLK_z:
						ch8_instance.current_key = 0x0A; break;
					case SDLK_x:
						ch8_instance.current_key = 0x00; break;
					case SDLK_c:
						ch8_instance.current_key = 0x0B; break;
					case SDLK_v:
						ch8_instance.current_key = 0x0F; break;
					default: 
						ch8_instance.current_key = 0xF0;
						break;
				}
		}
	}
}




void draw_sdl_window(sdl_window_data &window_data, chip8 &ch8_instance) {
	SDL_RenderClear(window_data.window_renderer);
	std::vector<SDL_Rect> pixels;

	// itterates through the SCREEN abstraction, 
	// where its a one a pixel will be added to the list with the position of i and j,
	// then multiplied by 10 as the x and y co-ordernates, 
	// +10 is to add padding around the screen
	for(int i = 0; i < 32; i++)              
	{                                        
		for(int j = 0; j < 64; j++)
		{
			if(ch8_instance.virtual_screen[i][j] == 1)
			{
				SDL_Rect tmp;
				tmp.h = 10;
				tmp.w = 10;
				tmp.x = j*10+10;
				tmp.y = i*10+10;

				pixels.push_back(tmp);
			}
		}
	}

	SDL_Rect pixel_arr[pixels.size()];
	std::copy(pixels.begin(), pixels.end(), pixel_arr);

	SDL_SetRenderDrawColor(window_data.window_renderer, 225, 225, 225, 225);
	SDL_RenderFillRects(window_data.window_renderer, pixel_arr, pixels.size());
	SDL_RenderDrawRects(window_data.window_renderer, pixel_arr, pixels.size());

	SDL_SetRenderDrawColor(window_data.window_renderer, 0, 0, 0, 225);
	SDL_RenderPresent(window_data.window_renderer);
}




void clear_sdl_window(sdl_window_data &window_data) {
	SDL_RenderClear(window_data.window_renderer);
	SDL_RenderPresent(window_data.window_renderer);
}
