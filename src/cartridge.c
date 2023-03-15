#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"
#include "cartridge.h"

//copy cartridge to memory
void loadROM(unsigned char ROM[]) {
    for (int i = 0; i < 0x4000; i++) {
        write_mem(0x4020 , ROM[i+0x10]);
    }
}
