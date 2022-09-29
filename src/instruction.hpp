#ifndef INSTRUCTION_HPP_
#define INSTRUCTION_HPP_

#include "emulator.hpp"

void init_instructions(void);
typedef void instruction_func_t(Emulator *);
extern vector<instruction_func_t *> instructions;

#endif // INSTRUCTION_HPP_
