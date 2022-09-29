#include <cstdint>
#include <string>
#include <vector>

#include "instruction.hpp"
#include "emulator.hpp"
#include "emulator_function.hpp"

vector<instruction_func_t *> instructions = vector<instruction_func_t *>(0x0100);

static void mov_r32_imm32(Emulator *emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xb8;
    uint32_t value = get_code32(emu, 1);
    emu->registers.at(reg) = value;
    emu->eip += 5;
}

void short_jump(Emulator *emu)
{
    int8_t diff = get_sign_code8(emu, 1);
    emu->eip += diff + 2;
}

void near_jump(Emulator *emu)
{
    int32_t diff = get_sign_code32(emu, 1);
    emu->eip += (diff + 5);
}

void init_instructions(void) {
    for (int i = 0; i < 8; ++i) {
        instructions.at(0xb8 + i) = mov_r32_imm32;
    }
    instructions.at(0xe9) = near_jump;
    instructions.at(0xeb) = short_jump;
}
