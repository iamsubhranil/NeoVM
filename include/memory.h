#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// Base methods
int init_mem();
void free_mem();
int reserve_mem(uint16_t from, uint16_t to);
void print_resmem();

// Normal write methods
void write_byte(unsigned char by, uint16_t location);
void write_word(uint16_t word, uint16_t location);
void write_int(int32_t value, uint16_t location);
void write_long(int64_t value, uint16_t location);

// Priviledged write methods
void pwrite_byte(unsigned char by, uint16_t location, int token);
void pwrite_word(uint16_t word, uint16_t location, int token);
void pwrite_int(int32_t value, uint16_t location, int token);
void pwrite_long(int64_t value, uint16_t location, int token);

// Read methods
unsigned char read_byte(uint16_t location);
uint16_t read_word(uint16_t location);
int32_t read_int(uint16_t location);
int64_t read_long(uint16_t location);

// Helper print
void print_mem(uint16_t from, uint16_t to);
#endif
