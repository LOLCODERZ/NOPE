#define STACK_SIZE 1024

#include <cstdint>
#include "instructions.h"
#include <iostream>

class VM {
public: 
    uint8_t stack[STACK_SIZE];
    uintptr_t stack_pointer;
    uintptr_t program_counter;

    bool execute_instruction(Instruction instruction, uint8_t arg0);

    void debug();
};