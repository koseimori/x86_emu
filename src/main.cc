#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

using namespace std;

enum Register
{
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTER_COUNT,
};

vector<string> registers_name = {
    "EAX",
    "ECX",
    "EDX",
    "EBX",
    "ESP",
    "EBP",
    "ESI",
    "EDI",
};

class Emulator
{
public:
    vector<uint32_t> registers;
    uint32_t eflags;
    vector<uint8_t> memory;
    uint32_t eip;
    Emulator(size_t size, uint32_t eip, uint32_t esp);
    void dump_registers();
    uint32_t get_code8(int32_t index);
    int32_t get_sign_code8(int32_t index);
    uint32_t get_code32(int32_t index);
    void mov_r32_imm32();
    void short_jump();
};

Emulator::Emulator(size_t size, uint32_t eip, uint32_t esp)
{
    this->memory = vector<uint8_t>(size, 0);
    this->registers = vector<uint32_t>(REGISTER_COUNT, 0);
    this->eip = eip;
    this->registers.at(ESP) = esp;
}

void Emulator::dump_registers()
{
    for (int32_t i = 0; i < REGISTER_COUNT; ++i)
    {
        cout << registers_name[i] << " = " << registers.at(i) << endl;
    }

    cout << "EIP = " << eip;
}

uint32_t Emulator::get_code8(int32_t index)
{
    return memory[eip + index];
}

int32_t Emulator::get_sign_code8(int32_t index)
{
    return (int8_t)memory[eip + index];
}

uint32_t Emulator::get_code32(int32_t index)
{
    uint32_t ret = 0;
    for (int32_t i = 0; i < 4; ++i)
    {
        ret |= get_code8(index + i) << (i * 8);
    }
    return ret;
}

void Emulator::mov_r32_imm32()
{
    uint8_t reg = get_code8(0) - 0xB8;
    uint32_t value = get_code32(1);
    registers.at(reg) = value;
    eip += 5;
}

void Emulator::short_jump()
{
    int8_t diff = get_sign_code8(1);
    eip += diff + 2;
}

int main(int argc, char *argv[])
{
    uint32_t MEMORY_SIZE = (1024 * 1024);
    Emulator emu = Emulator(MEMORY_SIZE, 0x0000, 0x7c00);

    if (argc != 2)
    {
        cout << "usage: px86 filename" << endl;
        return EXIT_FAILURE;
    }
}
