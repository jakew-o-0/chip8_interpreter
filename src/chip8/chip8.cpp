#include "chip8.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>





void chip8::load_game(std::string path) {
	// open file in binary mode
	std::ifstream file;
	file.open(path.c_str(), std::ios::binary | std::ios::in);
	assert(file.is_open());

	// find size of the rom for the buffer
	file.seekg(1, std::ios::end);
	int size = file.tellg();
	file.seekg(1, std::ios::beg);

	// create the buffer
	std::vector<char> buff;
	buff.resize(size);

	file.read(buff.data(), size);
	file.close();

	for(int i = 0; i < size; i++) {
		ram[i + 512] = static_cast<uint8_t>(buff[i]);
	}
}







void chip8::initalise() {
	pc = 0x0200;
	stack_ptr = 0;
	index_ptr = 0;
	delay_reg = 0;
	sound_reg = 0;

	uint8_t fontset[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0,
		0x20, 0x60, 0x20, 0x20, 0x70,
		0xF0, 0x10, 0xF0, 0x80, 0xF0,
		0xF0, 0x10, 0xF0, 0x10, 0xF0,
		0x90, 0x90, 0xF0, 0x10, 0x10,
		0xF0, 0x80, 0xF0, 0x10, 0xF0,
		0xF0, 0x80, 0xF0, 0x90, 0xF0,
		0xF0, 0x10, 0x20, 0x40, 0x40,
		0xF0, 0x90, 0xF0, 0x90, 0xF0,
		0xF0, 0x90, 0xF0, 0x10, 0xF0,
		0xF0, 0x90, 0xF0, 0x90, 0x90,
		0xE0, 0x90, 0xE0, 0x90, 0xE0,
		0xF0, 0x80, 0x80, 0x80, 0xF0,
		0xE0, 0x90, 0x90, 0x90, 0xE0,
		0xF0, 0x80, 0xF0, 0x80, 0xF0,
		0xF0, 0x80, 0xF0, 0x80, 0x80 
	};

	// initalise ram to 0 and load fontset
	for(uint16_t i = 0; i < 0xFFF; i++) {
		ram[i] = 0;
		if((i >= 0x050) && (i <= 0x0A0)){
			ram[i] =  fontset[i - 0x050];
		}
	}

	// clear registers
	for(int i = 0; i < 16; i++) {
		registers[i] = 0;
		stack[i] = 0;
	}

	// clear screen
	for(int i = 0; i < 32; i++) {
		for(int j = 0; j < 64; j++) {
			virtual_screen[i][j] = 0;
		}
	}
}










// grab the next instruction from memory
// by combining the next two bytes
// increment the pc by 2
// decrement the sound/delay registers
//
void chip8::fetch() {
	opcode = (ram[pc] << 8) | ram[pc + 1];	
	pc += 2;

	reg_x = registers[(opcode & 0x0F00) >> 8];
	reg_y = registers[(opcode & 0x00F0) >> 4];
	nn = opcode & 0x00FF;
	nnn = opcode & 0x0FFF;


	if(delay_reg > 0) 
		delay_reg--;

	if(sound_reg > 0)
		sound_reg--;
}





void chip8::decode() {

	// for commands that dont vary
	switch (opcode) {
		case 0x00E0:	clear_screen();		break;
		case 0x00EE:	exit_subroutine();	break;
		default:	break;
	}


	uint8_t nibble_1 = (opcode & 0xF000) >> 12;
	uint8_t nibble_2 = (opcode & 0x000F);

	switch (nibble_1) {
		case 0x1:	pc = nnn;					break;
		case 0x2:	call_subroutine();			break;
		case 0x3:	x_NEqual_n();				break;
		case 0x4:	x_Equal_n();				break;
		case 0x5:	x_Equal_y();				break;
		case 0x6:	registers[reg_x] = nn;		break;
		case 0x7:	registers[reg_x] += nn;		break;
		case 0x9:	x_NEqual_y();				break;
		case 0xA:	index_ptr = opcode & nnn;	break;
		case 0xB:	pc = registers[0] + nnn;	break;
		case 0xC:	registers[reg_x] = (std::rand() % 255) & nn;	break;
		case 0xD:	draw();						break;

		case 0x8: {
			switch (nibble_2) {
				case 0x0:	registers[reg_x] = registers[reg_y];	break;
				case 0x1:	bitwise_OR();			break;
				case 0x2:	bitwise_AND();			break;
				case 0x3:	bitwise_XOR();			break;
				case 0x4:	increment_by_Y();		break;
				case 0x5:	decrement_by_Y();		break;
				case 0x6:	shift_right();			break;
				case 0x7:	subtract_y_by_x();		break;
				case 0xE:	shift_left();			break;
				default:	break;
			}
		}

		case 0xE: {
			switch (nn) {
				case 0x9E:	key_npressed();	break;
				case 0xa1:	key_pressed();	break;
				default:	break;
			}
		}

		case 0xF:
		{
			switch (nibble_2) {
				case 0x7:	registers[reg_x] = delay_reg;	break;
				case 0xA:	await_keyPress();		break;
			}

			switch (nn) {
				case 0x15:	delay_reg = registers[reg_x];	break;
				case 0x18:	sound_reg = registers[reg_x];	break;
				case 0x1E:	index_ptr += registers[reg_x];	break;
				case 0x29:	load_font();	break;
				case 0x33:	bcd();			break;
				case 0x55:	save();			break;
				case 0x65:	load();			break;
			}
		}

		default:	break;
	}
}

void chip8::debug(){

}
