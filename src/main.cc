#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

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
};

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

void mov_r32_imm32(Emulator *emu)
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

int main(int argc, char *argv[])
{
    cout.setf(ios::hex, ios::basefield);

    uint32_t MEMORY_SIZE = 1024 * 1024;
    Emulator emu = Emulator();
    Emulator * emu_ptr = &emu;
    init_emu(emu_ptr, MEMORY_SIZE, 0x0000, 0x7c00);

    if (argc != 2)
    {
        cout << "usage: px86 filename" << endl;
        return EXIT_FAILURE;
    }

    uint8_t x;
    ifstream infile;
    infile.open(argv[1], ios::binary | ios::in);
    for (int i = 0; i < 0x0200; ++i) {
        infile.read((char *)(&emu_ptr->memory.at(i)), sizeof(emu_ptr->memory.at(i)));
    }
    infile.close();

    typedef void instruction_func_t(Emulator *);
    vector<instruction_func_t *> instructions = vector<instruction_func_t *>(0x0100);
    for (int i = 0; i < 8; ++i) {
        instructions.at(0xb8 + i) = mov_r32_imm32;
    }
    instructions.at(0xeb) = short_jump;

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
