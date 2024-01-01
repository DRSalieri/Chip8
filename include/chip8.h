#pragma once
#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>

// FontSet start address
const unsigned int FONTSET_START_ADDRESS = 0x50;
// ROM start address
const unsigned int START_ADDRESS = 0x200;

const unsigned int FONTSET_SIZE = 80;

const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_HEIGHT = 64;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip8
{
public:
	uint8_t     registers[16]{};
	uint8_t     memory[4096]{};
	uint16_t    index{};
	uint16_t    pc{};
	uint16_t    stack[16]{};
	uint8_t     sp{};
	uint8_t     delayTimer{};
	uint8_t     soundTimer{};
	uint8_t     keypad[16]{};
	uint32_t    video[64 * 32]{};
	uint16_t    opcode;

	std::default_random_engine 				randGen;
	std::uniform_int_distribution<uint8_t> 	randByte;

	// Function Pointer Table
	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1]

	Chip8();
	~Chip8();
	void LoadROM(char const* filename);

	// CPU
	void Cycle();

	void Table0()
	{
		((*this).*(table0[opcode & 0x000Fu]))();
	}
	void Table8()
	{
		((*this).*(table8[opcode & 0x000Fu]))();
	}
	void TableE()
	{
		((*this).*(tableE[opcode & 0x000Fu]))();
	}
	void TableF()
	{
		((*this).*(tableF[opcode & 0x00FFu]))();
	}
	void OP_NULL()
	{}

	#pragma region Instructions

	/// @brief CLS - clear the display
	void Chip8::OP_00E0()
	{
		memset(video, 0, sizeof(video));
	}
	/// @brief RET - return from a subroutine.
	void Chip8::OP_00EE()
	{
		--sp;
		pc = stack[sp];
	}
	/// @brief JP addr - Jump to Location nnn.
	void Chip8::OP_1nnn()
	{
		uint16_t address = opcode & 0x0FFFu;

		pc = address;
	}
	/// @brief CALL addr - Call subroutine at nnn.
	void Chip8::OP_2nnn()
	{
		uint16_t address = opcode & 0x0FFFu;

		stack[sp] = pc;
		++sp;
		pc = address;
	}
	/// @brief SE Vx, byte - Skip next instruction if Vx = kk.
	void Chip8::OP_3xkk()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t byte = opcode & 0x00FFu;

		if (registers[Vx] == byte)
		{
			pc += 2;
		}
	}
	/// @brief SNE Vx, byte - Skip next instruction if Vx != kk.
	void Chip8::OP_4xkk()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t byte = opcode & 0x00FFu;

		if (registers[Vx] != byte)
		{
			pc += 2;
		}
	}
	/// @brief SE Vx, Vy - Skip next instruction if Vx = Vy.
	void Chip8::OP_5xy0()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vx] == registers[Vy])
		{
			pc += 2;
		}
	}
	/// @brief SNE Vx, Vy - Skip next instruction if Vx != Vy.
	void Chip8::OP_9xy0()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vx] != registers[Vy])
		{
			pc += 2;
		}
	}

	/// @brief LD Vx, byte - Set Vx = kk.
	void Chip8::OP_6xkk()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t byte = opcode & 0x00FFu;

		registers[Vx] = byte;
	}
	/// @brief ADD Vx, byte - Set Vx = Vx + kk.
	void Chip8::OP_7xkk()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t byte = opcode & 0x00FFu;

		registers[Vx] += byte;
	}
	/// @brief LD Vx, Vy - Set Vx = Vy.
	void Chip8::OP_8xy0()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] = registers[Vy];
	}
	/// @brief OR Vx, Vy - Set Vx = Vx OR Vy.
	void Chip8::OP_8xy1()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] |= registers[Vy];
	}
	/// @brief AND Vx, Vy - Set Vx = Vx AND Vy.
	void Chip8::OP_8xy2()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] &= registers[Vy];
	}
	/// @brief XOR Vx, Vy - Set Vx = Vx XOR Vy.
	void Chip8::OP_8xy3()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] ^= registers[Vy];
	}
	/// @brief ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
	void Chip8::OP_8xy4()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		uint16_t sum = registers[Vx] + registers[Vy];

		if (sum > 255U)
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] = sum & 0xFFu;
	}
	/// @brief SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
	void Chip8::OP_8xy5()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vx] > registers[Vy])
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] -= registers[Vy];
	}
	/// @brief SHR Vx - Set Vx = Vx SHR 1, set VF = LSB o Vx.
	void Chip8::OP_8xy6()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		// Save LSB in VF
		registers[0xF] = (registers[Vx] & 0x1u);

		registers[Vx] >>= 1;
	}
	/// @brief SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
	void Chip8::OP_8xy7()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vy] > registers[Vx])
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] = registers[Vy] - registers[Vx];
	}
	/// @brief SHL Vx {, Vy} - Set Vx = Vx SHL 1, set VF = LSB of Vx.
	void Chip8::OP_8xyE()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		// Save MSB in VF
		registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

		registers[Vx] <<= 1;
	}
	/// @brief LD I, addr - Set I = nnn.
	void Chip8::OP_Annn()
	{
		uint16_t address = opcode & 0x0FFFu;

		index = address;
	}
	/// @brief JP V0, addr - Jump to location nnn + V0.
	void Chip8::OP_Bnnn()
	{
		uint16_t address = opcode & 0x0FFFu;

		pc = registers[0] + address;
	}
	/// @brief RND Vx, byte - set Vx = random byte AND kk.
	void Chip8::OP_Cxkk()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t byte = opcode & 0x00FFu;

		registers[Vx] = randByte(randGen) & byte;
	}
	/// @brief DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	void Chip8::OP_Dxyn()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;
		uint8_t height = opcode & 0x000Fu;

		// Wrap if going beyond screen boundaries
		uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
		uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

		registers[0xF] = 0;

		for (unsigned int row = 0; row < height; ++row)
		{
			uint8_t spriteByte = memory[index + row];

			for (unsigned int col = 0; col < 8; ++col)
			{
				uint8_t spritePixel = spriteByte & (0x80u >> col);
				uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

				// Sprite pixel is on
				if (spritePixel)
				{
					// Screen pixel also on - collision
					if (*screenPixel == 0xFFFFFFFF)
					{
						registers[0xF] = 1;
					}

					// Effectively XOR with the sprite pixel
					*screenPixel ^= 0xFFFFFFFF;
				}
			}
		}
	}
	/// @brief SKP Vx - Skip next instruction if key with the value of Vx is pressed.
	void Chip8::OP_Ex9E()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		uint8_t key = registers[Vx];

		if (keypad[key])
		{
			pc += 2;
		}
	}
	/// @brief SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
	void Chip8::OP_ExA1()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		uint8_t key = registers[Vx];

		if (!keypad[key])
		{
			pc += 2;
		}
	}
	/// @brief LD Vx, DT - Set Vx = delay timer value.
	void Chip8::OP_Fx07()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		registers[Vx] = delayTimer;
	}
	/// @brief LD Vx, K - Wait for a key press, store the value of the key in Vx.
	void Chip8::OP_Fx0A()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		if (keypad[0])
		{
			registers[Vx] = 0;
		}
		else if (keypad[1])
		{
			registers[Vx] = 1;
		}
		else if (keypad[2])
		{
			registers[Vx] = 2;
		}
		else if (keypad[3])
		{
			registers[Vx] = 3;
		}
		else if (keypad[4])
		{
			registers[Vx] = 4;
		}
		else if (keypad[5])
		{
			registers[Vx] = 5;
		}
		else if (keypad[6])
		{
			registers[Vx] = 6;
		}
		else if (keypad[7])
		{
			registers[Vx] = 7;
		}
		else if (keypad[8])
		{
			registers[Vx] = 8;
		}
		else if (keypad[9])
		{
			registers[Vx] = 9;
		}
		else if (keypad[10])
		{
			registers[Vx] = 10;
		}
		else if (keypad[11])
		{
			registers[Vx] = 11;
		}
		else if (keypad[12])
		{
			registers[Vx] = 12;
		}
		else if (keypad[13])
		{
			registers[Vx] = 13;
		}
		else if (keypad[14])
		{
			registers[Vx] = 14;
		}
		else if (keypad[15])
		{
			registers[Vx] = 15;
		}
		else
		{
			pc -= 2;
		}
	}
	/// @brief LD DT, Vx - Set delay timer = Vx.
	void Chip8::OP_Fx15()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		delayTimer = registers[Vx];
	}
	/// @brief Set sound timer = Vx.
	void Chip8::OP_Fx18()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		soundTimer = registers[Vx];
	}
	/// @brief ADD I, Vx - Set I = I + Vx.
	void Chip8::OP_Fx1E()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		index += registers[Vx];
	}
	/// @brief LD F, Vx - Set I = location of sprite for digit Vx.
	void Chip8::OP_Fx29()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t digit = registers[Vx];

		index = FONTSET_START_ADDRESS + (5 * digit);
	}
	/// @brief LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2.
	void Chip8::OP_Fx33()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t value = registers[Vx];

		// Ones-place
		memory[index + 2] = value % 10;
		value /= 10;

		// Tens-place
		memory[index + 1] = value % 10;
		value /= 10;

		// Hundreds-place
		memory[index] = value % 10;
	}
	/// @brief LD [I], Vx - Store registers V0 through Vx in memory starting at location I.
	void Chip8::OP_Fx55()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		for (uint8_t i = 0; i <= Vx; ++i)
		{
			memory[index + i] = registers[i];
		}
	}
	/// @brief LD Vx, [I] - Read registers V0 through Vx from memory starting at location I.
	void Chip8::OP_Fx65()
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		for (uint8_t i = 0; i <= Vx; ++i)
		{
			registers[i] = memory[index + i];
		}
	}

	#pragma endregion
};