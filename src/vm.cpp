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


// ================ Operations ================

        case Instruction::Add: {
            // Get the two operands from the stack: This goes for all following operation cases
            uint8_t left_side = this->stack_pop();
            uint8_t right_side = this->stack_pop();
            // Add them together and push the result onto the stack
            this->stack_push(left_side + right_side);
            this->address_map[this->stack_pointer - 1] = left_side + right_side;
            
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
            this->address_map[this->stack_pointer - 1] = left_side - right_side;

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
            this->address_map[this->stack_pointer - 1] = left_side * right_side;

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
            this->address_map[this->stack_pointer - 1] = left_side / right_side;

            // Check for overflow
            if (left_side % right_side != 0) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;


// ================ Multi-byte operations ================

        case Instruction::Add16: {
            // Interpret 8 bit operands to 16 bit
            uint16_t left_side = this->stack_pop();
            uint16_t right_side = this->stack_pop();
            // Add them together and push the result onto the stack as two 8 bit operands
            this->stack_push((left_side + right_side) & 0xFF);
            this->address_map[this->stack_pointer - 2] = (left_side + right_side) & 0xFF;
            this->stack_push((left_side + right_side) >> 8);
            this->address_map[this->stack_pointer - 1] = (left_side + right_side) >> 8;

            // Check for overflow
            if (left_side > 0xFFFF - right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;

        case Instruction::Add32: {
            // Interpret 8 bit operands to 32 bit
            uint32_t left_side = this->stack_pop();
            uint32_t right_side = this->stack_pop();
            // Add them together and push the result onto the stack as four 8 bit operands
            this->stack_push((left_side + right_side) & 0xFF);
            this->stack_push((left_side + right_side) >> 8);
            this->stack_push((left_side + right_side) >> 16);
            this->stack_push((left_side + right_side) >> 24);

            // Check for overflow
            if (left_side > 0xFFFFFFFF - right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;

        case Instruction::Add64: {
            // Interpret 8 bit operands to 64 bit
            uint64_t left_side = this->stack_pop();
            uint64_t right_side = this->stack_pop();
            // Add them together and push the result onto the stack as eight 8 bit operands
            this->stack_push((left_side + right_side) & 0xFF);
            this->stack_push((left_side + right_side) >> 8);
            this->stack_push((left_side + right_side) >> 16);
            this->stack_push((left_side + right_side) >> 24);
            this->stack_push((left_side + right_side) >> 32);
            this->stack_push((left_side + right_side) >> 40);
            this->stack_push((left_side + right_side) >> 48);
            this->stack_push((left_side + right_side) >> 56);

            // Check for overflow
            if (left_side > 0xFFFFFFFFFFFFFFFF - right_side) {
                this->overflow = true;
                throw "ERROR: Stack overflow";
            }
        } break;


// ================ Compare instructions ================

        case Instruction::Equal: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the two operands are equal
            if (left_side == right_side) {
                return true;
            } 
        } break;

        case Instruction::NotEqual: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the two operands are not equal
            if (left_side != right_side) {
                return true;
            }
        } break;

        case Instruction::GreaterThan: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the left operand is greater than the right operand
            if (left_side > right_side) {
                return true;
            }
        } break;

        case Instruction::LessThan: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the left operand is less than the right operand
            if (left_side < right_side) {
                return true;
            }
        } break;

        case Instruction::GreaterThanOrEqual: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the left operand is greater than or equal to the right operand
            if (left_side >= right_side) {
                return true;
            }
        } break;

        case Instruction::LessThanOrEqual: {
            uint8_t left_side = this->stack[--this->stack_pointer];
            uint8_t right_side = this->stack[this->stack_pointer];
            // Check if the left operand is less than or equal to the right operand
            if (left_side <= right_side) {
                return true;
            }
        } break;


// ================ Jump instructions ================

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