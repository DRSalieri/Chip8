#include "chip8.h"

/// @brief LD I, addr - Set I = nnn.
void Chip8::OP_Annn()
{
    uint16_t address = opcode & 0x0FFFu;

    index = address;
}
/// @brief LD Vx, DT - Set Vx = delay timer value.
void Chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delayTimer;
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