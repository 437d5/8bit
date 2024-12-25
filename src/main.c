#include <complex.h>
#include <time.h>
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
            uint8_t reg = MEMORY_SIZE - 1 - cpu->memory[cpu->PC++];
            uint8_t result = cpu->memory[reg] + cpu->memory[cpu->PC++];
            cpu->memory[reg] = result;
            break;
        }
        case 0x03: { // ADDR
            uint8_t reg_1 = cpu->memory[cpu->PC++];
            uint8_t reg_2 = cpu->memory[cpu->PC++];
            uint8_t result = cpu->memory[reg_1] + cpu->memory[reg_2];
            cpu->memory[reg_1] = result;
            break;
        }
        // case 0x04: { // SUB
        //     uint8_t reg_1 = cpu->memory[cpu->PC++];
        //     uint8_t reg_2 = cpu->memory[cpu->PC++];
        //     uint8_t result = cpu->registers[reg_1] - cpu->registers[reg_2];
        //     cpu->registers[reg_1] = result;
        //     break;  
        // }
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
        // case 0x05: // STORE: reg, addr
        //     store_op(cpu);
        //     break;
        case 0x07: // JUMP: addr
            jump_op(cpu);
            break;
        // case 0x08: // JUMP IF ZERO: reg, addr
        //     jump_z_op(cpu);
        //     break;
        // case 0x09: // OUT: reg
        //     out_op(cpu);
        //     break;
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
    
    cpu->memory[dest] = value;
}

void jump_op(CPU *cpu) {
    uint8_t dest = cpu->memory[cpu->PC++];
    printf("JUMP A=%02X B=%02X C=%02X\n", cpu->memory[A], cpu->memory[B], cpu->memory[C]); 
    cpu->PC = dest;
}

void run_emulator(CPU* cpu) {
    int counter = 0;
    while (1) {
        if (counter == MAX_ITERATION) {
            break;
        } else { 
            execute_instruction(cpu);
            // print_cpu_state(cpu);
            counter++;
        }
    }
}

int main() {
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    CPU cpu;
    init_cpu(&cpu);

    uint8_t program[] = {
       LOAD, A, 0x00, // LOAD A 0 
       LOAD, B, 0x01, // LOAD B 1
       LOAD, C, 0x00, // LOAD C 0 LOOP
       ADDR, C, A, // ADDR C A
       LOAD, A, 0x00, // LOAD A 0
       ADDR, A, B, // ADDR A B
       ADDR, B, C, // ADDR B C
       JUMP, 0x06 // JUMP LOOP
    };

    load_program(&cpu, program, sizeof(program));
    run_emulator(&cpu);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    long seconds = end.tv_sec - start.tv_sec; // Секунды
    long nanoseconds = end.tv_nsec - start.tv_nsec; // Наносекунды
    double elapsed = seconds + nanoseconds * 1e-9; // Время в секундах
    printf("TIME: %.9f\n", elapsed);
    
    return 0;
}