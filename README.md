# StackMachine
A project to explore the implementation of a virtual stack machine

## Project Goals
This project is intended to learn the how a stack machine works, 
with the goal of becoming robust enough to be the target of a compiler 
for a BASIC-like language. This project is focused on simplicity 
over performance, so it may differ somewhat from the "text book" 
definition of how a stack machine operates. For example, some instructions
peek at the second item on the stack without popping the item on top first.

## Stack Machine Features
The stack machine is comprised of a few fundamental components.
    - data stack - this is the primary stack for performing computations
    - return address stack - allows for control flow/branching
    - random access memory - an array that can be written/read to in any order (random access), array indices serve as memory addresses.
    - program memory - programs are represented as an array of strings, each entry containing an instruction, array indices serve a program memory addresses.
    - program counter - this tracks the address of the instruction in program memory that is being currently executed
    - ALU - arithmetic logic unit, in this implementation it is basically a branching table, responsible for receiving instructions and executing them.
    - debugging and verbose modes trace instruction execution and data movement.

## Instruction Set
  ### Stack manipulation
    - push <value> - push an item on to the data stack.
    - pop  - removes the top item from the stack.
    - pushr - pops the top item from the data stack and pushed it on to the return address stack.
    - popr -  pops the top item from the return address stack and pushes it onto the data stack.
    - show -  prints the value of the top itme on the data stack to console
    
 ### Memory Manipulation
   - load <addr> - gets the data at provided memory address and places it ontop of the data stack.
   - store <addr> - takes the item on the top of the data stack and saves it in the location pointed to by the provided memory address.

  ### Arithmetic
  All of these operations consume the top two values of the stack, resulting in those two values being replaced with the result of the operation.
  
    - add - adds the top two values of the data stack, pushing their sum on to the top of the data stack.
    - sub - subtracts the top value of the stack from the second value, pushing the result on to the top of the data stack.
    - mult - multiplies the top two values of the stack, pushing the result on to the top of the data stack.
    - div  - divides the second value on the stack by the top value on the stack, pushing the result on to the top of the data stack.
    - mod  - same as div except it peforms modulo division.

  ### Bitwise operations
  Similar to arithmetic operations, these instructions consume the top two items of the stack, with the result being pushed on to the data stack.
  
    - bwand - bitwise AND
    - bwor  - bitwise OR
    - bwxor - bitwise XOR
  
  ### Comparisons
  These operations compare the top two values of the stack, leaving them in place and pushing the result of the comparison on to the data stack.
  
    - cmpeq - compares for equality, pushes 1 on to the data stack if the values are equal, and 0 if they are not equal.
    - cmpl - less than comparison (<) pushes 1 on to the stack if the top value is less than the second value, other wise pushes 0 on to the data stack.
    - cmpg - greater than comparison (>) pushes 1 on to the data stack if the top value is greater than the second value on the stack, other wise pushes a 0 to the data stack.
    
 ### Branching and Control Flow
 These operations are what allow us to build up higher level constructs.
 
    - jmpnz <addr> - Jump if not zero, checks the top value of the data stack, and if it is not zero jumps to address provided.
    - jmpz <addr> - Jump if zero, checks the top value of the data stack, and if it IS a zero jumps to address provided.
    - call - treats the value on the top of the data stack as a jump address, performing a jump to that location.
    - retf - return from subroutine, this retrieves the return address from the return address stack to allow the continuation of program execution after a call or jump instruction.
    - halt - end program execution
    - nop - no operations - comes in handy more than you'd think.
    
 ### Instructions of Convenience
 Most of these instructions were borrowed from the Forth instruction set as described in [1]
 
  - swap - switches the placement of the top two values on the stack.
  - dup - pushes a copy of the top item on the data stack on to the data stack.
  - over - pushes a copy of the second item on the data stack on to the top of the data stack.
  
  
 ## Sample stack machine assemvly language programs
   Count down from 7
  ```
    push 7
    show
    push 1
    sub
    jmpnz 1
    halt
  ```
  Perform an action depending on the result of a comparison
  ```
    push 5
    push 3
    cmpq eq
    jmpnz 7
    cmpl
    jmpnz 10
    halt
    push 13
    show
    retf
    push 21
    show
    retf
  ```
