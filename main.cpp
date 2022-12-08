#include "test.h"
#include "vm.h"

int main() {
    VM vm = VM();

    vm.execute_instruction(Instruction::PushStack, 5);
    vm.debug();
    vm.execute_instruction(Instruction::PushStack, 2);
    vm.debug();
    vm.execute_instruction(Instruction::Add, 0);
    vm.debug();

    print_hello();
    return 0;
}
