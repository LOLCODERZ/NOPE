enum Instruction {
    // ===== Manipulation =====
    Push = 0x00,
    // Merge,
    // Split,
    Append,
    // Load,
    Interrupt,
    
// ======== Operations =========
    Add,
    Subtract,
    Multiply,
    Divide,

// === Multi-byte operations ===
    Add16,
    Add32,
    Add64,

// ======== Comparisons ========
    Equal,
    NotEqual,
    GreaterThan,
    LessThan,
    GreaterThanOrEqual,
    LessThanOrEqual,

// ========== Jumping ==========
    Jump,
    JumpEqual,
    JumpNotEqual
};