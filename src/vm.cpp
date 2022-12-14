#include "vm.h"

bool VM::execute_instruction(Instruction instruction, uint8_t arg0) {
    switch (instruction) {
        case Instruction::Push: {
            // Check if the stack is full
            if (this->stack_pointer >= STACK_SIZE) {
                throw "ERROR: Stack overflow";
                return false;
            }
            // Push the argument onto the stack
            this->stack[this->stack_pointer++] = arg0;
            } break;

        case Instruction::Add: {
            // Get the two operands from the stack: This goes for all following operation cases
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[--this->stack_pointer];
            // Add them together and push the result onto the stack
            this->stack[this->stack_pointer++] = left_side + right_side;
            
            // Check under and overflow
            if (this->stack[this->stack_pointer - 1] < left_side || this->stack[this->stack_pointer - 1] < right_side) {
                return false;
            }
        } break;


        case Instruction::Subtract: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[--this->stack_pointer];
            // Subtract and push the result onto the stack
            this->stack[this->stack_pointer++] = left_side - right_side;

            // Check under and overflow
            if (this->stack[this->stack_pointer - 1] > left_side || this->stack[this->stack_pointer - 1] > right_side) {
                return false;
            }
        } break;

        case Instruction::Multiply: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[--this->stack_pointer];
            // Multiply and push the result onto the stack
            this->stack[this->stack_pointer++] = left_side * right_side;

            // Check for under and overflow
            if (this->stack[this->stack_pointer - 1] < left_side || this->stack[this->stack_pointer - 1] < right_side) {
                return false;
            }
        } break;

        case Instruction::Divide: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[--this->stack_pointer];
            // Divide and push the result onto the stack
            this->stack[this->stack_pointer++] = left_side / right_side;

            // Check under and overflow
            if (this->stack[this->stack_pointer - 1] > left_side || this->stack[this->stack_pointer - 1] < right_side) {
                return false;
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
            uint8_t lhs = this->stack[--this->stack_pointer];
            uint8_t rhs = this->stack[this->stack_pointer];
            // Check if the two operands are equal
            if (lhs == rhs) {
                this->program_counter = arg0;
            }
        } break;

        case Instruction::JumpNotEqual: {
            uint8_t lhs = this->stack[--this->stack_pointer];
            uint8_t rhs = this->stack[this->stack_pointer];
            // Check if the two operands are not equal
            if (lhs != rhs) {
                this->program_counter = arg0;
            }
        } break;

        case Instruction::Append: {
            // Check if there is enough space on the stack
            if (this->stack_pointer + 1 >= STACK_SIZE) {
                throw "ERROR: Stack overflow";
                return false;
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

uint8_t VM::pop() {
    // Check if the stack is empty
    if (this->stack_pointer == 0) {
        throw "ERROR: Stack underflow";
        return 0;
    }
    // Return the top element of the stack
    return this->stack[--this->stack_pointer];
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