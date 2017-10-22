#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

// Named registers
#define ESP 0
#define EBP 1
#define EIP 2
#define EAX 3
#define EBX 4
#define ECX 5
#define EDX 6
#define ESI 7

void init_reg();
void free_reg();
void write_reg(uint16_t value, unsigned char number);
uint16_t read_reg(unsigned char number);

#endif
