#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

// Named registers
#define ESP 0
#define EBP 1
#define EIP 2
#define ERT 3 // Stores the address of the return value of previous operation

void init_reg();
void free_reg();
void write_reg(uint16_t value, unsigned char number);
uint16_t read_reg(unsigned char number);

#endif
