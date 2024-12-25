#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>

// Memory size
#define MEMORY_SIZE 256

// Number of iterations processor will operate
#define MAX_ITERATION 2500

// MEMORY ADDRESS
#define A 0xFC
#define B 0xFD
#define C 0xFE
#define D 0xFF

// OPCODE
#define LOAD 0x01 
#define ADDR 0x03
#define JUMP 0x07
#define HLT 0xFF

typedef struct {
    uint8_t registers[4]; // Registers array A, B, C, D, now in memory storage so do not use array
    uint8_t PC; // Program counter
    uint8_t flags; // Flags not in use now
    uint8_t memory[MEMORY_SIZE]; // Memory storage
} CPU;

void init_cpu(CPU* cpu); // Initialize cpu memory and registers and so on to equeal 0
void alu(CPU* cpu, uint8_t opcode); // ALU
void execute_instruction(CPU* cpu); // Opcode descriptor
void print_cpu_state(CPU* cpu); // Print hex dump
void load_program(CPU* cpu, uint8_t* program, size_t program_size); // Loads instructions to memory

void load_op(CPU* cpu); // Implement LOAD operation
void store_op(CPU* cpu); // Implement STORE operation
void jump_op(CPU* cpu); // Implement JUMP operation
void jump_z_op(CPU* cpu); // Implement JUMP_IF_ZERO operation
void out_op(CPU* cpu); // Implement OUT operation
#endif