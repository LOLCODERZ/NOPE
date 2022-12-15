#include "vm.h"

bool VM::execute_instruction(Instruction instruction, uint8_t arg0) {
    switch (instruction) {
        case Instruction::Push:
            this->stack_push(arg0);
            break;

        case Instruction::Pop:
            this->stack_pop();
            break;

        case Instruction::Add: {
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            Data sum = left_hand_side + right_hand_side;
            this->overflow_flag = sum.m_overflow;
            this->stack_push(sum);
        }
            break;

        case Instruction::Subtract: {
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            Data sum = left_hand_side - right_hand_side;
            this->overflow_flag = sum.m_overflow;
            this->stack_push(sum);
        }
            break;

        case Instruction::Multiply: {
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            Data sum = left_hand_side * right_hand_side;
            this->overflow_flag = sum.m_overflow;
            this->stack_push(sum);
        }
            break;

        case Instruction::Divide: {
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            Data sum = left_hand_side / right_hand_side;
            this->overflow_flag = sum.m_overflow;
            this->stack_push(sum);
        }
            break;

        case Instruction::Jump: {
            // Jump to the specified address
            Data address = this->stack_pop();
            this->program_counter = address.as_uintptr();
        }
            break;

        case Instruction::JumpEqual: {
            Data address = this->stack_pop();
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            // Check if the two operands are equal
            if (left_hand_side == right_hand_side) {
                this->program_counter = address.as_uintptr();
            }
        }
            break;

        case Instruction::JumpNotEqual: {
            Data address = this->stack_pop();
            Data left_hand_side = this->stack_pop();
            Data right_hand_side = this->stack_pop();
            // Check if the two operands are equal
            if (left_hand_side != right_hand_side) {
                this->program_counter = address.as_uintptr();
            }
        }
            break;

        case Instruction::Merge: {
            auto last_layout = this->stack_layout.back();
            stack_layout.pop_back();
            auto new_last_layout = this->stack_layout.back();
            if (last_layout.m_data_size != new_last_layout.m_data_size) {
                throw std::runtime_error("ERROR: Could not merge, the data types are different");
            }
            new_last_layout.m_high_address = last_layout.m_high_address;
            new_last_layout.m_data_size *= 2;
        }
            break;

        case Instruction::Append: {
            auto last_layout = this->stack_layout.back();
            stack_layout.pop_back();
            auto& new_last_layout = this->stack_layout.back();
            if (last_layout.m_data_size != new_last_layout.m_data_size) {
                throw std::runtime_error("ERROR: Could not append, the data types are different");
            }
            new_last_layout.m_high_address = last_layout.m_high_address;
        } break;

        case Instruction::Store: {
            Data address = this->stack_pop();
            Data value = this->stack_pop();

            auto it = this->variables.find(address.as_uintptr());
            if (it != this->variables.end()) {
                it->second = value;
            } else {
                this->variables.insert(std::make_pair(address.as_uintptr(), value));
            }

        }
            break;

        case Instruction::Load: {
            Data address = this->stack_pop();
            auto address_uint = address.as_uintptr();

            for (auto &variable: this->variables) {
                if (variable.first == address_uint) {
                    this->stack_push(variable.second);
                    return true;
                }
            }
            throw std::runtime_error("ERROR: Could not find variable!");
        }

        case Instruction::Out: {
            Data data = this->stack_pop();
            uintptr_t data_size = data.m_layout.m_high_address - data.m_layout.m_low_address;
            for (uintptr_t i = 0; i < data_size; i++) {
                std::cout << (char) data.m_data[i];
            }
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
    std::cout << "Layout (" << this->stack_layout.size() << "): " << std::endl;
    for (auto &layout : this->stack_layout) {
        std::cout << layout.to_string() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Variables (" << this->variables.size() << "): " << std::endl;
    for (auto &variable : this->variables) {
        std::cout << variable.first << " => " << variable.second.m_layout.to_string() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Program Counter (" << this->program_counter << ")" << std::endl;
    std::cout << std::endl << std::endl;
}

void VM::stack_push(uint8_t value) {
    if (this->stack_pointer >= STACK_SIZE) {
        throw std::overflow_error("Stack overflow :(");
    }

    this->stack_layout.emplace_back(
        this->stack_pointer,
        this->stack_pointer + 1,
        1                           // 1 Byte
    );
    this->stack[this->stack_pointer++] = value;
}

void VM::stack_push(const Data& data) {
    auto data_size = data.m_layout.m_high_address - data.m_layout.m_low_address;
    if (this->stack_pointer + data_size >= STACK_SIZE) {
        throw std::overflow_error("Stack overflow :(");
    }

    auto low_address = this->stack_pointer;
    for (uintptr_t i = 0; i < data_size; i++) {
        this->stack[this->stack_pointer++] = data.m_data[i];
    }
    auto high_address = this->stack_pointer;

    this->stack_layout.emplace_back(low_address, high_address, data.m_layout.m_data_size);
}

Data VM::stack_pop() {
    if (this->stack_layout.empty()) {
        throw std::underflow_error("Stack underflow :(");
    }
    // Pop the layout
    auto layout = this->stack_layout.back();
    this->stack_layout.pop_back();

    auto data = Data {this->stack, layout};
    this->stack_pointer = layout.m_low_address;

    return data;
}

void VM::add(Instruction instruction, uint8_t arg0) {
    this->program.emplace_back(instruction, arg0);
}

void VM::execute(bool debug) {
    while (this->program_counter < this->program.size()) {
        auto instruction = this->program[this->program_counter++];
        if (!this->execute_instruction(instruction.first, instruction.second)) {
            break;
        }
        if (debug) {
            this->debug();
        }
    }
}