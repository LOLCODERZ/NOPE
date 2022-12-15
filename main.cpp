#include "test.h"
#include "vm.h"
#include "fstream"

int main() {
    VM vm = VM();

    /*
     * Sample program [c]:
     *
     * int i = 0;
     * while (i != 2) {
     *      i += 1;
     * }
     *
     * Sample program [Assembly]:
     *
     * # int i = 0;
     * 0    -   PUSH 0   // Value of i
     * 1    -   PUSH 0   // Address of i
     * 2    -   STORE    // Store i to memory
     *
     * # i != 10
     * 3    -   PUSH 0   // Address of i
     * 4    -   LOAD     // Load i
     * 5    -   PUSH 2  // Push 10 (the value being compared)
     * 6    -   PUSH 16   // Push location to jump to
     * 7    -   JUMPEQ   // Jump to the location if i == 10
     *
     * # i += 1
     * 8    -   PUSH 0   // Address of i
     * 9    -   LOAD     // Load i
     * 10   -   PUSH 1   // Push 1 (to value being added to i)
     * 11   -   ADD      // Add 1 to i
     * 12   -   PUSH 0   // Address of i
     * 13   -   STORE    // Update the value of i
     *
     * # while
     * 14   -   PUSH 3   // Push the address of the start of the loop
     * 15   -   JUMP     // Go back to the beginning of the loop
     */

    // int i = 0:
    /*vm.add(Instruction::Push, 0);
    vm.add(Instruction::Push, 0);
    vm.add(Instruction::Store, 0);
    // i != 10
    vm.add(Instruction::Push, 0);
    vm.add(Instruction::Load, 0);
    vm.add(Instruction::Push, 10);
    vm.add(Instruction::Push, 100);
    vm.add(Instruction::JumpEqual, 0);
    // i += 1
    vm.add(Instruction::Push, 0);
    vm.add(Instruction::Load, 0);
    vm.add(Instruction::Push, 1);
    vm.add(Instruction::Add, 0);
    vm.add(Instruction::Push, 0);
    vm.add(Instruction::Store, 0);
    // while
    vm.add(Instruction::Push, 3);
    vm.add(Instruction::Jump, 0);
    // End of program
    vm.add(Instruction::Interrupt, 0);*/

    vm.parse("/home/remy/apc-proj/NOPE/res/main.bytecat");

    vm.execute(false);

    return 0;
}
