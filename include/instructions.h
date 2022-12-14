enum Instruction {
    Push = 0x00,
    
    Add,
    Subtract,
    Multiply,
    Divide,
    
    Add16,
    // Add32,
    // Add64,

    Jump,
    JumpEqual,
    JumpNotEqual,
    // Merge,
    // Split,
    Append,
    // Load,
    Interrupt
};