# Assembly Intro

An introduction to assembly language for C programmers, focusing reimplementing simple C standard library functions in assembly.

## Table of Contents

- [What is Assembly Language?](#what-is-assembly-language)
- [What Are Registers?](#what-are-registers)
- [Common x86-64 Registers](#common-x86-64-registers)
- [Memory Addressing in Assembly](#memory-addressing-in-assembly)
- [Common Instructions](#common-instructions)
- [The `ft_strlen` Function Explained Simply](#the-ft_strlen-function-explained-simply)
- [Detailed Explanation of `xor rax, rax`](#detailed-explanation-of-xor-rax-rax)

## What is Assembly Language?

Assembly language is a low-level programming language that communicates directly with our computer's processor. Unlike C, where we might write:

```c
x = x + 1;
```

in assembly, we must specify exactly which memory locations to use and how to manipulate them.

## What Are Registers?

Registers are tiny, super-fast variables built directly into the CPU. Unlike RAM:

- **Extremely limited:** We only have a handful available.
- **Extremely fast:** Much faster than accessing memory.
- **Specific purposes:** Each has its own conventions.

> **Analogy:** We can think of registers as the CPU's personal workspace—the small desk where it keeps values it's actively working with.

## Common x86-64 Registers

Modern 64-bit processors have several registers. The most important ones for our code:

| Register | Name/Role         | Typical Use                       |
| -------- | ----------------- | --------------------------------- |
| **RAX**  | Accumulator       | Stores function return values     |
| **RDI**  | Destination Index | First function argument           |
| **RSI**  | Source Index      | Second function argument          |
| **RDX**  | Data Register     | Third function argument or math   |
| **RCX**  | Counter Register  | Fourth function argument or loops |

## Memory Addressing in Assembly

The notation `[rdi + rax]` means:

> "The memory location at address (`rdi` + `rax`)"

This is similar to accessing `s[length]` in C.

## Common Instructions

- `xor reg, reg` — Sets a register to zero (faster than `mov reg, 0`)
- `cmp` — Compares two values
- `je` — "Jump if equal" (conditional jump)
- `inc` — Increment a value by 1
- `jmp` — Unconditional jump
- `ret` — Return from a function

## The `ft_strlen` Function Explained

The function [`ft_strlen`](ft_strlen.s) counts the characters in a string, just like C's `strlen()`:

1. The function receives a string pointer in the **RDI** register.
2. It sets **RAX** to 0 (our counter).
3. It enters a loop:

- Checks if the current character at `[RDI + RAX]` is zero (end of string).
- If zero, exits the loop.
- If not, increments **RAX** and checks the next character.

4. When done, the count is in **RAX** (the return value).

## Detailed Explanation of `xor rax, rax`

This instruction is a clever way to set the **RAX** register to zero, using bitwise operations.

### What is XOR?

**XOR** (exclusive OR) is a bitwise operation:

Here's how XOR works at the bit level:

| Bit A | Bit B | A XOR B |
| ----- | ----- | ------- |
| 0     | 0     | 0       |
| 0     | 1     | 1       |
| 1     | 0     | 1       |
| 1     | 1     | 0       |

- If the bits are **different**, the result is **1**.
- If the bits are **the same**, the result is **0**.

### Why XOR a Register with Itself?

When we XOR any value with itself, we always get zero:

- Every bit position is comparing identical bits.
- Same bits always produce 0 in XOR.

**Example:**

```
    10101010
XOR 10101010
    --
    00000000
```

### Why Use XOR Instead of MOV?

- **Smaller instruction size:** Encodes to fewer bytes.
- **Performance:** Modern CPUs optimize this pattern.
- **Historical efficiency:** On older CPUs, this was faster than loading a constant.

## Sources
### Further Reading & Resources

- [Assembly Language in 100 Seconds (Fireship)](https://www.youtube.com/watch?v=4gwYkEK0gOk)
