#include <memory.h>
#include <registers.h>
#include <stdint.h>
#include <stdio.h>

static uint16_t start = 0x1000, end = 0x2FFF;
static int token = -1;

// Initializes the stack
void init_stack(){
    token = reserve_mem(start, end);
    write_reg(start, ESP);
}

// Pushes a word into the stack
void push(uint16_t word){
    uint16_t sp = read_reg(ESP);
    if(sp == end){
        printf("\n[Fatal] Stack overflow!");
        return;
    }
    pwrite_word(word, sp, token);
    write_reg(sp+1, ESP);
}

// Pushes an integer(4 bytes) into the stack
void pushi(int32_t val){
    push(val >> 16);
    push(val & 0xFFFF);
}

// Pushes a long(8 bytes) into the stack
void pushl(int64_t val){
    pushi(val >> 32);
    pushi(val & 0xFFFFFFFF);
}

// Pops the next word from the stack
uint16_t pop(){
    uint16_t sp = read_reg(ESP);
    if(sp == start){
        printf("\n[Fatal] Stack underflow!");
        return 0;
    }
    write_reg(sp - 2, ESP);
    return read_word(sp);
}

// Pops the next integer(4 bytes) from the stack
int32_t popi(){
    int32_t ret = pop();
    return (ret << 16) | pop();
}

// Pops the next long(8 bytes) from the stack
int64_t popl(){
    int64_t ret = popi();
    return (ret << 32 ) | popi();
}
