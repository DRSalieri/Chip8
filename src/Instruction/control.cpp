#include "chip8.h"

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
/// @brief JP V0, addr - Jump to location nnn + V0.
void Chip8::OP_Bnnn()
{
    uint16_t address = opcode & 0x0FFFu;

    pc = registers[0] + address;
}