#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "emulator.hpp"
#include "instructions.hpp"

using namespace std;


void init_emu(Emulator* emu, size_t size, uint32_t eip, uint32_t esp)
{
    emu->memory = vector<uint8_t>(size);
    emu->registers = vector<uint32_t>(REGISTER_COUNT, 0);
    emu->eip = eip;
    emu->registers.at(ESP) = esp;
}

void dump_registers(Emulator *emu)
{
    for (int32_t i = 0; i < REGISTER_COUNT; ++i)
    {
        cout << registers_name[i] << " = " << emu->registers.at(i) << endl;
    }

    cout << "EIP = " << emu->eip;
}

uint32_t get_code8(Emulator *emu, int32_t index)
{
    return emu->memory[emu->eip + index];
}

int32_t get_sign_code8(Emulator *emu, int32_t index)
{
    return (int8_t)emu->memory[emu->eip + index];
}

uint32_t get_code32(Emulator *emu, int32_t index)
{
    uint32_t ret = 0;
    for (int32_t i = 0; i < 4; ++i)
    {
        ret |= get_code8(emu, index + i) << (i * 8);
    }
    return ret;
}

int32_t get_sign_code32(Emulator *emu, int32_t index) {
    return (int32_t)get_code32(emu, index);
}



int main(int argc, char *argv[])
{
    cout.setf(ios::hex, ios::basefield);

    uint32_t MEMORY_SIZE = 1024 * 1024;
    Emulator emu = Emulator();
    Emulator * emu_ptr = &emu;
    init_emu(emu_ptr, MEMORY_SIZE, 0x7c00, 0x7c00);

    if (argc != 2)
    {
        cout << "usage: px86 filename" << endl;
        return EXIT_FAILURE;
    }

    uint8_t x;
    ifstream infile;
    infile.open(argv[1], ios::binary | ios::in);
    for (int i = 0x7c00; i < 0x7c00 + 0x0200; ++i) {
        infile.read((char *)(&emu_ptr->memory.at(i)), sizeof(emu_ptr->memory.at(i)));
    }
    infile.close();

    while (emu_ptr->eip < MEMORY_SIZE) {
        uint8_t code = get_code8(emu_ptr, 0);

        cout << "EIP = " << emu_ptr->eip << ", Code = " << static_cast<uint16_t>(code) << endl;

        if (instructions.at(code) == NULL) {
            cout << "Not Implemented: " << code << endl;
            break;
        }

        instructions.at(code)(emu_ptr);

        if (emu_ptr->eip == 0x0000) {
            cout << "End of program" << endl;
            break;
        }
    }

    dump_registers(emu_ptr);

    return EXIT_SUCCESS;
}
