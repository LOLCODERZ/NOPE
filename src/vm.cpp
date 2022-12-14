#include "vm.h"

bool VM::execute_instruction(Instruction instruction, uint8_t arg0) {
    switch (instruction) {
        case Instruction::Push: {
            // Check if the stack is full
            if (this->stack_pointer >= STACK_SIZE) {
                throw "ERROR: Stack overflow";
                return false;
            }
            // Register the data to the stack layout
            this->stack_layout.push_back(DataLayout {
                this->stack_pointer, 
                this->stack_pointer + 1,
                1                           // 1 Byte
            });
            // Push the argument onto the stack
            this->stack[this->stack_pointer++] = arg0;
        } break;

        case Instruction::Pop: {
            // Check if the stack is empty
            if (this->stack_pointer == 0) {
                throw "ERROR: Stack underflow";
                return false;
            }
            // Pop the top element from the stack
            auto last_layout = this->stack_layout.back();
            this->stack_pointer = last_layout.m_low_address;
            this->stack_layout.pop_back();
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

        // case Instruction::Add16: {
        //     // Get the two operands from the stack
        //     uint16_t left_side = this->stack[--this->stack_pointer];
        //     uint16_t right_side = this->stack[--this->stack_pointer];
        //     // Add them together and push the result onto the stack
        //     this->stack[this->stack_pointer++] = left_side + right_side;

        //     // Check under and overflow
        //     if (this->stack[this->stack_pointer - 1] < left_side || this->stack[this->stack_pointer - 1] < right_side) {
        //         return false;
        //     }
        // }

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

        case Instruction::Merge: {
            auto last_layout = this->stack_layout.back();
            stack_layout.pop_back();
            auto new_last_layout = this->stack_layout.back();
            if (last_layout.m_data_size != new_last_layout.m_data_size) {
                throw "ERROR: Could not merge, the data types are different";
            }
            new_last_layout.m_high_address = last_layout.m_high_address;
            new_last_layout.m_data_size *= 2;
        } break;

        // case Instruction::Append: {
        //     // Check if there is enough space on the stack
        //     if (this->stack_pointer + 1 >= STACK_SIZE) {
        //         throw "ERROR: Stack overflow";
        //         return false;
        //     }
        //     // Increment the size of the stack to make space for the new element
        //     this->stack_pointer++;
        //     // Append the element to the end of the stack
        //     this->stack[this->stack_pointer - 1] = arg0;
        // } break;
        
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