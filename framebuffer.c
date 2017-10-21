#include <memory.h>
#include <framebuffer.h>
#include <stdio.h>

static int token = -1, pointer = 0;
static uint16_t base = 0x0000, end = 0x0FFF;

void init_fb(){
    token = reserve_mem(base, end);
}

void write_char(unsigned char value){
    if(pointer > (end-base))
        return;
    pwrite_byte(value, base + pointer, token);
    pointer++;
}

void flush_fb(){
    uint16_t temp = 0;
    for(temp = 0;temp < pointer;temp++){
        printf("%c", read_byte(base + temp));
    }
    pointer = 0;
}
