#include <complex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/cpu.h"

void init_cpu(CPU* cpu) {
    for (int i = 0; i < 4; i++) {
        cpu->registers[i] = 0;
    }
    
    cpu->PC = 0;
    cpu->flags = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        cpu->memory[i] = 0;
    }
}

void alu(CPU* cpu, uint8_t opcode) {
    switch (opcode) {
        case 0x02: { // ADDX
            uint8_t result = cpu->registers[cpu->memory[cpu->PC++]] + cpu->memory[cpu->PC++];
            cpu->registers[cpu->memory[cpu->PC++]] = result;
            break;
        }
        case 0x03: { // ADDR
            uint8_t reg_1 = cpu->memory[cpu->PC++];
            uint8_t reg_2 = cpu->memory[cpu->PC++];
            uint8_t result = cpu->registers[reg_1]+cpu->registers[reg_2];
            cpu->registers[reg_1] = result;
            break;
        }
        case 0x04: { // SUB
            uint8_t reg_1 = cpu->memory[cpu->PC++];
            uint8_t reg_2 = cpu->memory[cpu->PC++];
            uint8_t result = cpu->registers[reg_1] - cpu->registers[reg_2];
            cpu->registers[reg_1] = result;
            break;  
        }
        default: 
            printf("Unknown opcode in ALU: 0x%02X\n", opcode);
            exit(1);
    };
}

void execute_instruction(CPU* cpu) {
    uint8_t opcode = cpu->memory[cpu->PC++];

    switch (opcode) {
        case 0x01:
            load_op(cpu);    
            break;
        case 0x02: // ADDX: reg + value
            alu(cpu, opcode);
            break;
        case 0x03: // ADDR: reg + reg
            alu(cpu, opcode);
            break;
        case 0x04: // SUB: reg - reg
            alu(cpu, opcode);
            break;
        case 0x05: // STORE: reg, addr
            store_op(cpu);
            break;
        case 0x07: // JUMP: addr
            jump_op(cpu);
            break;
        case 0x08: // JUMP IF ZERO: reg, addr
            jump_z_op(cpu);
            break;
        case 0x09: // OUT: reg
            out_op(cpu);
            break;
        case 0xFF: // HLT
            printf("Processor halted, exiting emulator.");
            exit(0);
        case 0x00: // NOP

        default:
            printf("Unknow opcode: 0x%02X\n", opcode);
            exit(1);
    };
}

void print_cpu_state(CPU* cpu) {
    printf("PC: %02X, Flags: %02X\n", cpu->PC, cpu->flags);
    printf("Registers: A = %02X, B = %02X, C = %02X, D = %02X\n", 
           cpu->registers[0], cpu->registers[1], cpu->registers[2], cpu->registers[3]);
    printf("Memory dump:\n");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%02X ", cpu->memory[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void load_program(CPU* cpu, uint8_t* program, size_t program_size) {
    for (size_t i = 0; i < program_size; i++) {
        cpu->memory[i] = program[i];
    }
}

void load_op(CPU* cpu) {
    uint8_t dest, value;
    dest = cpu->memory[cpu->PC++];
    value = cpu->memory[cpu->PC++];
    
    if (dest >= 0 && dest < 4) {
        cpu->registers[dest] = value;
    } else {
        printf("Invalid destination LOAD operation: 0x%02X\n", dest);
    }
}

void run_emulator(CPU* cpu) {
    while (1) {
        execute_instruction(cpu);
        print_cpu_state(cpu);
    }
}

int main() {
    CPU cpu;
    init_cpu(&cpu);

    uint8_t program[] = {
        0x00, 0x00, 0x05,
        0x00, 0x01, 0x01,
        0x01, 
        0xFF
    };

    load_program(&cpu, program, sizeof(program));
    run_emulator(&cpu);
    
    return 0;
}