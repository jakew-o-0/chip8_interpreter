#pragma once

#include <cstdint>
#include <string>

class chip8 {
	private:
		uint8_t ram[4096];
		uint8_t registers[16];
		uint8_t sound_reg;
		uint8_t delay_reg;

		uint16_t stack[16];
		uint16_t stack_ptr;
		uint16_t index_ptr;
		uint16_t pc;

		uint16_t opcode;

		uint8_t reg_x;
		uint8_t reg_y;
		uint8_t	nn;
		uint16_t nnn;

	public:
		uint8_t screen_render_flag;
		uint8_t virtual_screen[32][64];
		uint8_t current_key;




	public:
		chip8(std::string path) {
			initalise();
			load_game(path);
		}

		void cycle_cpu(bool debug_on) {
			fetch();
			decode();

			if(debug_on)
				debug();
		}


	private:
		void initalise();
		void load_game(std::string path);

		void fetch();
		void decode();
		void debug();

		// opcode instructions
		void clear_screen();	
		void exit_subroutine();
		void jump();				
		void call_subroutine();	
		void x_NEqual_n();			
		void x_Equal_n();			
		void x_Equal_y();			
		void set_X_to_NN();		
		void increment_by_NN();	
		void x_NEqual_y();			
		void set_I();				
		void jump_with_offset();		
		void set_X_to_Rand();		
		void draw();				
		void set_X_to_Y();		
		void bitwise_OR();		
		void bitwise_AND();	
		void bitwise_XOR();	
		void increment_by_Y();	
		void decrement_by_Y();	
		void shift_right();	
		void subtract_y_by_x();
		void shift_left();		
		void key_npressed();	
		void key_pressed();	
		void set_X_to_delay();	
		void await_keyPress();
		void set_delay();		
		void set_sound();		
		void increment_I();	
		void load_font();		
		void bcd();			
		void save();			
		void load();			
};
