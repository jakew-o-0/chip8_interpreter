#include "chip8.h"
#include <cstdint>


void chip8::clear_screen() {
	screen_render_flag = 0x0C;
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			virtual_screen[i][j] = 0x0;
		}
	}
}


void chip8::exit_subroutine(){
	stack_ptr--;					      //decrement stack pointer
	pc = stack[stack_ptr];				//pc asigned to the retrun location
	stack[stack_ptr] = 0x0000;			//unused stack position is cleared
}


void chip8::call_subroutine(){
	stack[stack_ptr] = pc;         // add pc to stack and incement sp
	stack_ptr++;
	pc = nnn;   // jump to location NNN by assigning pc NNN
}


void chip8::x_NEqual_n(){
	// if true, the next instruction is skipped
	if (registers[reg_x] == nn ){  
		pc += 2;
	}
}


void chip8::x_Equal_n(){
	if (registers[reg_x] != nn) {  
		pc += 2;
	}
}

void chip8::x_Equal_y(){
	if (registers[reg_x] == registers[reg_y]) {  
		pc += 2;
	}
}


void chip8::x_NEqual_y(){
	if (registers[reg_x] != registers[reg_y]) {  
		pc += 2;
	}
}


void chip8::set_X_to_Rand(){
	registers[reg_x] = (std::rand() % 255) & nn;
}


void chip8::draw(){
	screen_render_flag = 0xF;
	uint8_t height =  opcode & 0x000F;
	uint8_t x_pos = registers[reg_x];
	uint8_t y_pos = registers[reg_y];

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < 8; j++) {
			x_pos = (x_pos + i) % 32;
			y_pos = (y_pos + i) % 64;
			uint8_t bit = (ram[index_ptr] & (1 << (8 - j)) >> (8 - j));
			virtual_screen[x_pos][y_pos] ^= bit;
		}
	}
}



void chip8::bitwise_OR(){
	registers[reg_x] |= registers[reg_y];
	registers[0xF] = 0;
}


void chip8::bitwise_AND(){
	registers[reg_x] &= registers[reg_y];
	registers[0xF] = 0;
}


void chip8::bitwise_XOR(){
	registers[reg_x] |= registers[reg_y];
	registers[0xF] = 0;
}


void chip8::increment_by_Y(){
	uint8_t prev_regx = registers[reg_x];
	registers[reg_x] += registers[reg_y];
	registers[0xF] = 1;

	if(prev_regx > registers[reg_x]) {	// checks for overflow
		registers[0xF] = 1;
	}
}


void chip8::decrement_by_Y(){
	uint8_t prev_regx = registers[reg_x];
	registers[reg_x] -= registers[reg_y];
	registers[0xF] = 0;

	if(prev_regx < registers[reg_x]) {	// checks for underflow
		registers[0xF] = 1;
	}
}


void chip8::shift_right(){
	registers[reg_x] = registers[reg_y];
	uint8_t smallest_bit = registers[reg_x] & 0x01;
	registers[reg_x] >>= 1;
	registers[0xF] = smallest_bit;
}


void chip8::subtract_y_by_x(){
	uint8_t prev_regx = registers[reg_x];
	registers[reg_x] =- registers[reg_y];

	if(prev_regx < registers[reg_x]) {	// checks for underflow
		registers[0xF] = 1;
	}
}


void chip8::shift_left(){
	registers[reg_x] = registers[reg_y];
	uint8_t largest_bit = (registers[reg_x] & (1 << 7)) >> 7;
	registers[reg_x] <<= 1;
	registers[0xF] = largest_bit;
}


void chip8::key_npressed(){
}


void chip8::key_pressed(){
}


void chip8::await_keyPress(){
}


void chip8::load_font(){
	switch (registers[reg_x]) 
	{
		case 0x00:  index_ptr = 80; break;  // 0
		case 0x01:  index_ptr = 85; break;  // 1
		case 0x02:  index_ptr = 90; break;  // 2
		case 0x03:  index_ptr = 95; break;  // 3
		case 0x04:  index_ptr = 100; break; // 4
		case 0x05:  index_ptr = 105; break; // 5
		case 0x06:  index_ptr = 110; break; // 6
		case 0x07:  index_ptr = 115; break; // 7
		case 0x08:  index_ptr = 120; break; // 8
		case 0x09:  index_ptr = 125; break; // 9
		case 0x0A:  index_ptr = 130; break; // A
		case 0x0B:  index_ptr = 135; break; // B
		case 0x0C:  index_ptr = 140; break; // C
		case 0x0D:  index_ptr = 145; break; // D
		case 0x0E:  index_ptr = 150; break; // E
		case 0x0F:  index_ptr = 155; break; // F

		default: break;
	}
}


void chip8::bcd(){
}


void chip8::save(){
	for(int i = 0; i <= reg_x; i++) {
		ram[index_ptr + i] = registers[i];
	}
}


void chip8::load(){
	for(int i = 0; i <= reg_x; i++) {
		registers[i] = ram[index_ptr + i];
	}
}
