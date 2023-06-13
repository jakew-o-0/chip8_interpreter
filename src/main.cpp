#include "./src/chip8/chip8.h"
#include "./src/gui_screen/chip8_sdl_win.h"
#include <SDL2/SDL_timer.h>


int main(void) {
	chip8 ch8_instance = chip8("./resorces/chip8-test-suite.ch8");
	sdl_window_data window_data = init_sdl_win();
	

	while(1) {
		int starting_tick = SDL_GetTicks();

		poll_sdl_events(window_data, ch8_instance);
		ch8_instance.cycle_cpu(0);

		if(ch8_instance.screen_render_flag == 0xF){
			ch8_instance.screen_render_flag = 0x00;
			draw_sdl_window(window_data, ch8_instance);
		}

		if(ch8_instance.screen_render_flag == 0x0C) {
			ch8_instance.screen_render_flag = 0x00;
			clear_sdl_window(window_data);
		}

		if((1000 / 60) > SDL_GetTicks() == starting_tick)
			SDL_Delay(1000 / 60 - (SDL_GetTicks() - starting_tick));
	}
}
