#define STACK_SIZE 1024

#include <cstdint>
#include "instructions.h"
#include <iostream>

class VM {
// Variables
public: 
    uint8_t stack[STACK_SIZE];
    // Stack pointer that points to the top of the stack
    uintptr_t stack_pointer;
    // Program counter that points to the next instruction
    uintptr_t program_counter;

// Functions
public:
    bool execute_instruction(Instruction instruction, uint8_t arg0);

    // Pop function to handle the stack
    uint8_t pop();

    void debug();
};