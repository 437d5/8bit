# 8 bit processor

## Memory
Memory is a 256 byte array which store instructions and operands.
The last 4 byte of memory contain A, B, C and D registers. 
The size of every memory cell is 8 bit and registers are also 8 bit.
Memory cells `0-251` store program code.
Memory cells `252-255` store A, B, C, D respectively.

## Assembly language

Now my processor can proceed only 4 instructions 

- ```LOAD <register> <value>``` -- register is memory location of register and value
is some constant. Example: ```LOAD A 5``` equals to 
```LOAD 0xFC 0x05```. OPCODE = 0x01

- ```ADDX <register> <value>```. Example ```ADDX A 0x05``` equals to A = A + 5.
OPCODE = 0x02

- ```ADDR <register> <register>``` -- add two register's values. Example
```ADDR A B``` equals to ```ADDR 0xFC 0xFD``` (A = A + B). OPCODE = 0x03

- ```JUMP <address>``` -- jumps to address in memory. OPCODE 0x07

- ```HLT``` -- stops the emulator. OPCODE 0xFF

### Program example

Fibonacci sequence
```
LOAD, A, 0x00, // LOAD A 0 
LOAD, B, 0x01, // LOAD B 1
LOAD, C, 0x00, // LOAD C 0 LOOP
ADDR, C, A, // ADDR C A
LOAD, A, 0x00, // LOAD A 0
ADDR, A, B, // ADDR A B
ADDR, B, C, // ADDR B C
JUMP, 0x06 // JUMP LOOP
```

## Globals

`MAX_ITERATION` -- Number of iterations processor will do before shutting down.

`MEMORY_SIZE` -- The number of memory addresses available.

## Some improvements

1. Increase the number of operations available.
2. Write python script to ease programm writing.
3. Add flags.
4. Refactor operation names and their work.
5. Rework memory to be dynamically dependent on `MEMORY_SIZE` variable.
