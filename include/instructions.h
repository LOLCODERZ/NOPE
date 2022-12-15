enum Instruction {
    // Push a byte to the stack
    Push = 0x00,
    // Pop the last data from the stack
    Pop,
    // Add the last two data on the stack and push the result
    Add,
    // Subtract the last two data on the stack and push the result
    Subtract,
    // Multiply the last two data on the stack and push the result
    Multiply,
    // Divide the last two data on the stack and push the result
    Divide,
    // Jump to an instruction
    Jump,
    // Jump to an instruction if the last two data are equal
    JumpEqual,
    // Jump to an instruction if the last two data are not equal
    JumpNotEqual,
    // Merge the last two data (8 bits + 8 bits => 16 bits)
    Merge,
    // Append the last two data (8 bits + 8 bits => 8 bits x2)
    Append,
    // Maybe: Split,
    // Store the last data from the stack to the memory at address [arg0]
    Store,
    // Load the data at address [arg0] to the stack (copy)
    Load,
    // Print the last data to the console
    Out,
    // Stop the program
    Interrupt,
    // Clone the data
    Clone
};