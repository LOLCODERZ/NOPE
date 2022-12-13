enum Instruction {
    Push = 0x00,
    Pop,
    Add,

    Add16,
    // Add32,
    // Add64,
    
    Subtract,
    Multiply,
    Divide,
    Jump,
    JumpEqual,
    JumpNotEqual,
    // Merge,
    // Split,
    // Append,
    // Load,
    Interrupt
};