#include "vm.h"

// TODO: Support 1+ byte data type
//     : Use a Layout Table (stack layout)
//     : Add Multi-byte instruction (add16, add32, add64)
// TODO (in case of layout table): Add append instruction
//                               : To extend the size of a data type or an array
//                               : No issue with memory since the element should be on top of the stack
// TODO: Find a way to access variables (being somewhere else in the stack):
//     : Pop everything until we access the correct variable [Slow 😢]
//     : Store variables in 2nd stack [Still slow]
//     : Store variables on the stack with a Map linking the variables with their address (ptr)
//              > In case of early free (keep it in mind [memory]) and free it once on top of the stack (normal free)
//              > To avoid having empty space in the memory
//              > Pretty fast (cannot really be considered a stack anymore though) [definitely avoid LinkedList in this case]
// TODO: Add more Operation instruction
// TODO: Support for negative numbers
// TODO: Add instruction for comparison
// TODO: Add instruction for jumping
// TODO: FFI functions => printing
// TODO: Think about includes
//     : Either process it in the compiler (can be ignored because we are skipping the middle-end part [Checking + Error Processing])
//     : Or process it in the compiler and only using certain FFI functions if the correct file in included
//              > Maybe using namespace resolution (map with namespace and function)
// TODO: Full debug mode (with input scanning to go to the next node or breakpoint) and add breakpoint
// TODO: Intermediate Language from the instruction
//     : Example Syntax =>
//          PUSH 0
//          STORE x01
//          $A LOAD x01
//          PUSH 5
//          JMPEQ $B
//          LOAD x01
//          PUSH 1
//          ADD
//          SET x01
//          JMP $A
//          $B INTERRUPT
//     : Instruction Explanations =>
//          PUSH [arg0]: Push [arg0] to the stack
//          STORE [arg0]: Store the value on top of the stack to the memory at address [arg0]
//          LOAD [arg0]: Push the value at address [arg0] to the stack (duplicate it)
//          JUMPEQ [arg0]: Jump to [arg0] if the last two values on the stack are equal (consume the last two values)
//          ADD: Add the last two values on the stack and push the results (consume the last two values)
//          SET [arg0]: Update the value at address [arg0] to last value on the stack
//          JMP [arg0]: Jump to [arg0]
//          INTERRUPT: Interrupts/stops the program
//     : Compiler =>
//          Split at spaces
//          If it starts with:
//              'x' => Hexadecimal
//              'b' => Binary
//              '
// Optional: Add Short Operations (Only in the intermediate language's compiler)
//         : IADD [arg0] => Immediate Add
//                       => PUSH arg0
//                       => ADD
//         : ISTORE [arg0] [arg1] => Immediate Store
//                         => PUSH arg1
//                         => STORE arg0
//         : SPUSH [arg0] => String Push (Could be also a seperate instruction in the VM)
// Example on how to push a 32 bits number (0xAABBCCDD)
//      Option 1:
//          PUSH 0xAA
//          PUSH 0xBB
//          MERGE       # [16 bits] 0xAABB
//          PUSH 0xCC
//          PUSH 0xDD
//          MERGE       # [16 bits] 0xCCDD
//          MERGE       # [32 bits] 0xAABBCCDD
//      Option 2:
//          PUSH 0xAA
//          PUSH 0xBB
//          MERGE       # [16 bits] 0xAABB
//          PUSH 0xCC
//          MERGE       # [24 bits] 0xAABBCC
//          PUSH 0xDD
//          MERGE       # [32 bits] 0xAABBCCDD
//      Option 3:
//          PUSH 0xAA
//          APPEND 0xBB # [16 bits] 0xAABB
//          APPEND 0xCC # [24 bits] 0xAABBCC
//          APPEND 0xDD # [32 bits] 0xAABBCCDD
//      Reflection:
//          Probably better to use MERGE (and also maybe SPLIT) because we might want to merge data from the memory
//          Might also be useful for casting types (Maybe have a RESIZE instruction)
//          but think about what to do with the rest of the data and where the resize starts
// TODO: Think about what to do if we want to store an 16 bits value and update it with a 32 bits value
//     : Same with arrays (would it be possible to append it)
//     : Same with strings (technically arrays)
//     Idea: early free + cloning to the top of the stack (moving) and then appending it (since on top of the stack)
//     Potential issue, the moved variable will be freed to early since it is on top of the stack
// The stack will therefore only be considered as a stack when freeing variables
//     : Pop only possible from the top
//     : Read possible everywhere
//     : Write possible everywhere
//     : Push only possible to the top
// TODO: Support overflow
//      Option 1:
//          Automatically resize the data type
//      Option 2:
//          Have to possibility to ignore the overflow (ignore the carry) [Add, AddO => Add with overflow]

bool VM::execute_instruction(Instruction instruction, uint8_t arg0) {
    switch (instruction) {
        case Instruction::PushStack:
            this->stack[this->stack_pointer++] = arg0;
            break;
        case Instruction::Add: {
            uint8_t lhs = this->stack[--this->stack_pointer];
            uint8_t rhs = this->stack[--this->stack_pointer];
            this->stack[this->stack_pointer++] = lhs + rhs;
            // TODO: Check Overflow
        } break;
        case Instruction::Interrupt:
            return false;
    }
    
    return true;
}

void VM::debug() {
    std::cout << "----- Lolcode VM -----" << std::endl;
    std::cout << "Stack (" << this->stack_pointer << "): ";
    for (uintptr_t i = 0; i < this->stack_pointer; i++) {
        if (i % 8 == 0) std::cout << std::endl << "      ";
        std::cout << (int) this->stack[i] << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout << "Program Counter (" << this->program_counter << ")" << std::endl;
}