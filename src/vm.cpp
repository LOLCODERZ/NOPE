#include "vm.h"

void VM::stack_push(uint8_t value) {
    // Check if the stack is full
    if (this->stack_pointer >= STACK_SIZE) {
        this->overflow = true;
        throw "ERROR: Stack overflow";
    }
    // Push the value onto the stack
    this->stack[this->stack_pointer++] = value;
}

uint8_t VM::stack_pop() {
    // Check if the stack is empty
    if (this->stack_pointer == 0) {
        this->underflow = true;
        throw "ERROR: Stack underflow";
    }
    // Return the top element of the stack
    return this->stack[--this->stack_pointer];
}

bool VM::execute_instruction(Instruction instruction, uint8_t arg0) {
    switch (instruction) {
        case Instruction::Push: {
            // Check if the stack is full
            if (this->stack_pointer >= STACK_SIZE) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
            // Push the argument onto the stack
            this->stack_push(arg0);
            } break;

        case Instruction::Add: {
            // Get the two operands from the stack: This goes for all following operation cases
            uint8_t left_side = this->stack_pop();
            uint8_t right_side = this->stack_pop();
            // Add them together and push the result onto the stack
            this->stack_push(left_side + right_side);
            
            // Check for overflow
            if (left_side > 0xFF - right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;


        case Instruction::Subtract: {
            uint8_t left_side = this->stack_pop();
            uint8_t right_side = this->stack_pop();
            // Subtract and push the result onto the stack
            this->stack_push(left_side - right_side);

            // Check overflow
            if (left_side < right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;

        case Instruction::Multiply: {
            uint8_t left_side = this->stack_pop();
            uint8_t right_side = this->stack_pop();
            // Multiply and push the result onto the stack
            this->stack_push(left_side * right_side);

            // Check for overflow
            if (left_side > 0xFF / right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;

        case Instruction::Divide: {
            uint8_t left_side = this->stack_pop();
            uint8_t right_side = this->stack_pop();
            // Divide and push the result onto the stack
            this->stack_push(left_side / right_side);

            // Check for overflow
            if (left_side % right_side != 0) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;

        case Instruction::Add16: {
            // Get the two operands from the stack
            uint16_t left_side = this->stack[--this->stack_pointer];
            uint16_t right_side = this->stack[--this->stack_pointer];
            // Add them together and push the result onto the stack
            this->stack[this->stack_pointer++] = left_side + right_side;

            // Check under and overflow
            if (this->stack[this->stack_pointer - 1] < left_side || this->stack[this->stack_pointer - 1] < right_side) {
                return false;
            }
        } break;

        case Instruction::Jump: {
            // Jump to the specified address
            this->program_counter = arg0;
        } break;

        case Instruction::JumpEqual: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the two operands are equal
            if (left_side == right_side) {
                this->program_counter = arg0;
            }
        } break;

        case Instruction::JumpNotEqual: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the two operands are not equal
            if (left_side != right_side) {
                this->program_counter = arg0;
            }
        } break;

        case Instruction::Append: {
            // Check if there is enough space on the stack
            if (this->stack_pointer + 1 >= STACK_SIZE) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
            // Increment the size of the stack to make space for the new element
            this->stack_pointer++;
            // Append the element to the end of the stack
            this->stack[this->stack_pointer - 1] = arg0;
        } break;
        
        case Instruction::Interrupt: {
            return false;
        }
    }
    
    return true;
}


void VM::debug() {
    std::cout << "----- Lolcode VM -----" << std::endl;
    std::cout << "Stack (" << this->stack_pointer << "): ";

    // Print stack contents
    for (uintptr_t i = 0; i < this->stack_pointer; i++) {
        if (i % 8 == 0) std::cout << std::endl << "      ";
        std::cout << (int) this->stack[i] << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout << "Program Counter (" << this->program_counter << ")" << std::endl;
}