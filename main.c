#include <memory.h>
#include <framebuffer.h>
#include <registers.h>
#include <stack.h>

#include <inttypes.h>
#include <stdio.h>

void test_word(uint16_t loc, int token){
    uint16_t word, loc1;
    printf("\n[Test] Word read-write test");
    printf("\n[Input] Enter a word(16 bit) to write : ");
    //printf("%s", SCNu16);
    scanf("%" SCNu16, &word);
    if(loc == 0){
        printf("[Input] Enter a location(0-65535) to write : ");
        scanf("%" SCNu16, &loc);
        token = -1;
    }
    printf("[Info] Writing %u to 0x%x", word, loc);
    if(token == -1)
        write_word(word, loc);
    else
        pwrite_word(word, loc, token);
    printf("\n[Info] Read %u from %x\n", read_word(loc), loc);
    if(read_word(loc) != word)
        printf("[Test] Failed!");
    else
        printf("[Test] Succeeded!");
}

void test_int(uint16_t loc, int token){
    int32_t word;
    printf("\n[Test] Integer read-write test");
    printf("\n[Input] Enter an integer(32 bit) to write : ");
    scanf("%" SCNd32, &word);
    if(loc == 0){
        printf("[Input] Enter a location(0-65535) to write : ");
        scanf("%" SCNu16, &loc);
        token = -1;
    }
    printf("[Info] Writing %d to 0x%x", word, loc);
    if(token == -1)
        write_int(word, loc);
    else
        pwrite_int(word, loc, token);
    printf("\n[Info] Read %d from %x\n", read_int(loc), loc);
    if(read_int(loc) != word)
        printf("[Test] Failed!");
    else
        printf("[Test] Succeeded!");
}

void test_long(uint16_t loc, int token){
    int64_t word;
    printf("\n[Test] Long read-write test");
    printf("\n[Input] Enter a long(64 bit) to write : ");
    scanf("%" SCNd64, &word);
    if(loc == 0){
        printf("[Input] Enter a location(0-65535) to write : ");
        scanf("%" SCNu16, &loc);
        token = -1;
    }
    printf("[Info] Writing %ld to 0x%x", word, loc);
    if(token == -1)
        write_long(word, loc);
    else
        pwrite_long(word, loc, token);
    printf("\n[Info] Read %ld from %x\n", read_long(loc), loc);
    if(read_long(loc) != word)
        printf("[Test] Failed!");
    else
        printf("[Test] Succeeded!");
}

void test_reserve(){
    uint16_t start, size;
    printf("\n[Test] Memory reservation test");
    printf("\n[Input] Enter the start address to reserve : ");
    scanf("%" SCNu16, &start);
    printf("[Input] Enter size of the reserved block in bytes : ");
    scanf("%" SCNu16, &size);
    printf("[Info] Reserving block of size %" PRId16 " bytes starting from 0x%x..", size, start);
    int token = reserve_mem(start, start+size);
    if(token == -1){
        printf("\n[Error] Reservation failed!");
    }
    else{
        printf("\n[Info] Reservation succeeded, token received %d!", token);
        printf("\n[Info] Testing standard read-writes..");
        if(size > 1)
            test_word(start, token);
        if(size > 3)
            test_int(start, token);
        if(size > 7)
            test_long(start, token);
    }
    printf("\n[Test] Memory reservation test complete!");
}

void test_fb(){
    int num = 0;
    printf("\n[Test] Framebuffer test");
    printf("\n[Input] Enter number of characters : ");
    scanf("%d", &num);
    while(num > 0){
        char c;
        printf("[Input] Enter character %d : ", num);
        scanf(" %c", &c);
        write_char(c);
        num--;
    }
    printf("[Info] Flushing framebuffer\n");
    flush_fb();
    printf("\n[Test] Framebuffer test complete");
}

void test_registers(){
    printf("\n[Test] Register test");
    uint16_t word, reg;
    printf("\n[Input] Enter a word to store : ");
    scanf("%" SCNu16, &word);
    printf("[Input] Enter a register number to store(0-7) : ");
    scanf("%" SCNu8, &reg);
    printf("[Info] Writing %" PRIu16 " to register %" PRIu8, word, reg);
    write_reg(word, reg);
    printf("\n[Info] Reading from register %" PRIu8, reg);
    printf("\n[Info] Read %" PRIu16 " from register %"PRIu8, read_reg(reg), reg);
    printf("\n[Test] ");
    if(read_reg(reg) != word)
        printf("Failed");
    else
        printf("Succeeded!");
}

void test_stack(){
    printf("\n[Test] Stack test");
    uint16_t word;
    int32_t vali;
    int64_t vall;
    printf("\n[Input] Enter a word to push into stack : ");
    scanf("%" SCNu16, &word);
    printf("[Info] Pushing the 0x%2x into the stack..", word);
    push(word);
    printf("\n[Input] Enter an integer to push into stack : ");
    scanf("%" SCNd32, &vali);
    printf("[Info] Pushing 0x%4x into the stack", vali);
    pushi(vali);
    printf("\n[Input] Enter a long to push into stack : ");
    scanf("%" SCNd64, &vall);
    printf("[Info] Pushing 0x%8x into the stack\n[Info] Stack now : ", vall);
    pushl(vall);
    print_mem(0x1000, 0x1000 + 13);
    printf("\n[Info] Popped long : %" PRId64, popl());
    printf("\n[Info] Popped integer : %" PRId32, popi());
    printf("\n[Info] Popped word : %" PRIu16, pop());
    printf("\n[Test] Stack test complete");
}

int main(){
    init_mem();
    init_fb();
    init_reg();
    init_stack();
    
    printf("[Info] Reserved memory map\n");
    print_resmem();
    printf("\n");

    test_word(0, -1);
    test_int(0, -1);
    test_long(0, -1);
    test_reserve();
    test_fb();
    test_registers();
    test_stack();

    printf("\n");
    free_mem();
    free_reg();
    return 0;
}
