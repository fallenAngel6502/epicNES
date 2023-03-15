#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"

//2KB of  internal memory
uint8_t internal_RAM[0x800] = {0};

//cartridge space in memory
uint8_t cart_mem[0xBFE0] = {0};

//read from memory
uint8_t read_mem(uint16_t addr){
    if(addr <= 0x800){
        return internal_RAM[addr];
    }else if(addr >= 0x4020){
        return cart_mem[addr - 0x4020];
    }
}

//write to memory
void write_mem(uint16_t addr, uint8_t data){
    if(addr <= 0x800){
        internal_RAM[addr] = data;
    }else if(addr >= 0x4020){
        cart_mem[addr - 0x4020] = data;
    }
}

/*--------------------------------------------------------
Addressing functions
--------------------------------------------------------*/
//immediate addressing
uint16_t imm(uint16_t PC){
    return PC + 1;
}

//absolute addressing
uint16_t absolute(uint16_t PC){
    uint8_t low = read_mem(PC + 1);
    uint8_t high = read_mem(PC + 2);
    high <<= 8;
    return high + low;
}

//absolute x-indexed addressing
uint16_t abs_x(uint16_t PC, uint8_t X){
    uint8_t low = read_mem(PC + 1);
    uint8_t high = read_mem(PC + 2);
    high <<= 8;
    return high + low + X;
}

//absolute y-indexed addressing
uint16_t abs_y(uint16_t PC, uint8_t Y){
    uint8_t low = read_mem(PC + 1);
    uint8_t high = read_mem(PC + 2);
    high <<= 8;
    return high + low + Y;
}

//indirect x-indexed addressing
uint16_t ind_x(uint16_t PC, uint8_t X){
    uint8_t imm0 = read_mem(PC + 1);
    uint8_t low = read_mem(imm0 + X);
    uint8_t high = read_mem(imm0 + X + 1);
    high <<= 8;
    return high + low;
}

//indirect y-indexed addressing
uint16_t ind_y(uint16_t PC, uint8_t Y){
    uint8_t imm0 = read_mem(PC + 1);
    uint8_t low = read_mem(imm0);
    uint8_t high = read_mem(imm0 + 1);
    high <<= 8;
    return high + low + Y;
}

//zero page addressing
uint16_t zero_pg(uint16_t PC){
    return (uint16_t) read_mem(PC + 1);
}

//zero page x-indexed addressing
uint16_t zero_pg_x(uint16_t PC, uint8_t X){
    return (uint16_t) read_mem(PC + 1) + X;
}

//zero page y-indexed addressing
uint16_t zero_pg_y(uint16_t PC, uint8_t Y){
    return (uint16_t) read_mem(PC + 1) + Y;
}
