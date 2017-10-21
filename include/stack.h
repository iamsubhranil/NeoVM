#ifndef STACK_H
#define STACK_H

#include <stdint.h>

void init_stack();

void push(uint16_t word);
void pushi(int32_t value);
void pushl(int64_t value);

uint16_t pop();
int32_t popi();
int64_t popl();

#endif
