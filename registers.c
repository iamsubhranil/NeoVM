#include <registers.h>
#include <stdlib.h>

static uint16_t *registers = NULL;
#define NUM_REG 8 // 8 registers for now

void init_reg(){
    registers = (uint16_t *)malloc(sizeof(uint16_t)*NUM_REG);
}

void free_reg(){
    free(registers);
}

void write_reg(uint16_t value, unsigned char loc){
    registers[loc] = value;
}

uint16_t read_reg(unsigned char loc){
    return registers[loc];
}
