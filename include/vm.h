#ifndef NOPE_VM_H
#define NOPE_VM_H

#include <cstdint>
#include "constant.h"
#include "instructions.h"
#include "data_layout.h"
#include "data.h"
#include <iostream>
#include <vector>
#include <map>

class VM {
// Variables
public: 
    uint8_t stack[STACK_SIZE];
    // Layout of the stack (data size)
    std::vector<DataLayout> stack_layout {};
    // Map of variables (address, data)
    std::map<uintptr_t, Data> variables {};
    // Stack pointer that points to the top of the stack
    uintptr_t stack_pointer = 0;
    // Program counter that points to the next instruction
    uintptr_t program_counter = 0;
    // Program
    std::vector<std::pair<Instruction, uint8_t>> program {};
    // Overflow flag to check if calculation overflowed
    bool overflow_flag = false;

// Functions
public:
    void add(Instruction instruction, uint8_t arg0);
    void execute(bool debug);
    bool execute_instruction(Instruction instruction, uint8_t arg0);

    void parse(const std::string& str);

    void debug();

private:
    void stack_push(uint8_t value);
    void stack_push(const Data& data);
    Data stack_pop();
};

#endif // NOPE_VM_H