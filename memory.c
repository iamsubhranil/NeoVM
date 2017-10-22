#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <memory.h>
/* 
 * 64KB total addressable memory, hence 16bit addresses.
 * Word length 16bit. Hence to access a word, 2 reads or writes have to be performed.
 * Integer : 32bit
 * Long : 64bit
 *
 */
#define MEM_SIZE 65536
static unsigned char* memory = NULL;
static uint16_t **reserved_locations= NULL, res_count = 0;

// Initializes 64K chunk of memory
int init_mem(){
    memory = (unsigned char *)malloc(MEM_SIZE);
    return memory != NULL;
}

// Frees the allocated memory
void free_mem(){
    free(memory);
}

// Checks if present access overlaps any reserved area
static inline int check_access(uint16_t location){
    if(location < 0 || location > MEM_SIZE)
        return 0;
    uint16_t i = 0;
    while(i < res_count){
        if(location >= reserved_locations[i][0] && location <= reserved_locations[i][1])
            return 0;
        i++;
    }
    return 1;
}

/*
 * This method reserves a portion of memory marked by `from - to` to make them unavailable to access by the usual write_* API.
 * Any other API calling this method will be returned a token, which it'll have to pass to the priviledged pwrite_* calls, to make
 * any successful write to the reserved locations. If the region requested to be reserved is already fully or partly reserved
 * by another API, this method will return -1, and the reservation will fail.
 */
int reserve_mem(uint16_t from, uint16_t to){
    if(!check_access(from) || !check_access(to))
        return -1;
    if(from >= to)
        return -1;
    res_count++;
    reserved_locations = (uint16_t **)realloc(reserved_locations, res_count*sizeof(uint16_t *));
    reserved_locations[res_count - 1] = (uint16_t *)malloc(sizeof(uint16_t)*2);
    reserved_locations[res_count - 1][0] = from;
    reserved_locations[res_count - 1][1] = to;
    while(from < to){
        memory[from] = 0;
        from++;
    }
    return res_count;
}

// Prints the reserved memory blocks
void print_resmem(){
    int i = 0;
    while(i < res_count){
        printf("\n[Token %d] 0x%4x - 0x%4x (%u bytes)", (i+1), reserved_locations[i][0], reserved_locations[i][1],
                reserved_locations[i][1] - reserved_locations[i][0]);
        i++;
    }
}

// Grants access to any reserved location if the appropiate token is provided
static inline int grant_access(uint16_t location, int token){
#ifdef DEBUG
    printf("\n[Debug] Checking access of location 0x%x for token %d..", location, token);
#endif
    if(token < 1 || token > res_count)
        return 0;
    if(location >= reserved_locations[token - 1][0] && location <= reserved_locations[token - 1][1])
        return 1;
#ifdef DEBUG
    printf("\n[Debug] Bad access [Allowed : %u - %u, Tried : %u]", reserved_locations[token - 1][0], reserved_locations[token - 1][1], location);
#endif
    return 0;
}

/*
 * Main method which performs the actual write. This method should never be called directly. Rather, appropiate
 * write_* should be called to write something to memory, which will take care of casting, fitting, and all other
 * aspects.
 */
static void write_mem(unsigned char value, uint16_t location, int token){
    if(token > 0 && !grant_access(location, token))
        return;
    else if(token < 1 && !check_access(location))
        return;
    memory[location] = value;
}

/*
 * Writes one byte into memory
 */
void write_byte(unsigned char by, uint16_t location){
    write_mem(by, location, -1);
}

/*
 * Writes a word(2 bytes) into memory.
 */
void write_word(uint16_t word, uint16_t location){
    write_mem(word >> 8, location, -1); // upper bits are stored in lower location
    write_mem(0x00FF & word, location + 1, -1); // lower order bits are stored in higher location
}

/*
 * Writes an integer(4 bytes) to memory
 */
void write_int(int32_t val, uint16_t location){
    write_word(val >> 16, location);
    write_word(val & 0xFFFF, location + 2);
}

/*
 * Writes a long(8 bytes) to memory
 */
void write_long(int64_t val, uint16_t location){
    write_int(val >> 32, location);
    write_int(val & 0xFFFFFFFF, location + 4);
}

// Helper to show the contents of memory
void print_mem(uint16_t from, uint16_t to){
    while(from <= to){
        printf("%x ", memory[from]);
        from++;
    }
}

/*
 * Base read method. This method should never be directly called.
 */
static unsigned char read_mem(uint16_t location){
    return memory[location];
}

/*
 * Reads one byte from memory.
 */
unsigned char read_byte(uint16_t location){
    return read_mem(location);
}

/*
 * Reads one word(2 bytes) from memory
 */
uint16_t read_word(uint16_t location){
    uint16_t ret = read_mem(location);
    return (ret << 8) | read_mem(location + 1);
}

/*
 * Reads an integer(4 bytes) from memory
 */
int32_t read_int(uint16_t location){
    int32_t ret = read_word(location);
    return (ret << 16) | read_word(location + 2);
}

/*
 * Reads a long(8 bytes) from memory
 */
int64_t read_long(uint16_t location){
    int64_t ret = read_int(location);
    return (ret << 32) | read_int(location + 4);
}


/*
 * Writes one byte into memory in priviledged mode
 */
void pwrite_byte(unsigned char by, uint16_t location, int token){
    write_mem(by, location, token);
}

/*
 * Writes a word(2 bytes) into memory in priviledged mode
 */
void pwrite_word(uint16_t word, uint16_t location, int token){
    write_mem(word >> 8, location, token); // upper bits are stored in lower location
    write_mem(0x00FF & word, location + 1, token); // lower order bits are stored in higher location
}

/*
 * Writes an integer(4 bytes) to memory in priviledged mode
 */
void pwrite_int(int32_t val, uint16_t location, int token){
    pwrite_word(val >> 16, location, token);
    pwrite_word(val & 0xFFFF, location + 2, token);
}

/*
 * Writes a long(8 bytes) to memory in priviledged mode
 */
void pwrite_long(int64_t val, uint16_t location, int token){
    pwrite_int(val >> 32, location, token);
    pwrite_int(val & 0xFFFFFFFF, location + 4, token);
}
