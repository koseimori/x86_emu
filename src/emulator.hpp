#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include <iostream>
#include <vector>

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

#endif // EMULATOR_HPP_
