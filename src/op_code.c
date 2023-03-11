#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"
#include "op_code.h"

/*--------------------------------------------------------
OP code functions
--------------------------------------------------------*/

//determines state of SR flags N, Z, C
void SR_flags(uint16_t val, char flags[], uint8_t* SR){
    
    char N[] = "N";
    char Z[] = "Z";
    char C[] = "C";
 
    //N
    if(strstr(flags, N)){
        if(((val & 0x80)>>7)== 1){
            set_flag('N', SR);
        }else{
            clear_flag('N', SR);
        }
    }
    //Z
    if(strstr(flags, Z)){
        if(val == 0){
            set_flag('Z', SR);
        }else{
            clear_flag('Z', SR);
        }
    }
    //C
    if(strstr(flags, C)){
        if(val > 0xFF){
            set_flag('C', SR);
        }else{
            clear_flag('C', SR);
        }
    }
}

//set a flag in SR
void set_flag(char flag, uint8_t* SR){
    switch(flag){
        case 'N':
            *SR |= 0x80;
        break;
        case 'Z':
            *SR |= 0x02;
        break;
        case 'C':
            *SR |= 0x01;
        break;
        case 'I':
            *SR |= 0x04;
        break;
        case 'V':
            *SR |= 0x40;
        break;
        case 'B':
            *SR |= 0x10;
        break;
    }
}

//clear a flag in SR
void clear_flag(char flag, uint8_t* SR){
    switch(flag){
        case 'N':
            *SR &= 0x7F;
        break;
        case 'Z':
            *SR &= 0xFD;
        break;
        case 'C':
            *SR &= 0xFE;
        break;
        case 'I':
            *SR &= 0xFB;
        break;
        case 'V':
            *SR &= 0xBF;
        break;
    }
}

//get a flag in SR
uint8_t get_flag(char flag, uint8_t* SR){
	uint8_t result;
    switch(flag){
        case 'N':
            result = (*SR & 0x80) >> 7;
        break;
        case 'Z':
            result = (*SR & 0x02) >> 1;
        break;
        case 'C':
            result = (*SR & 0x01);
        break;
        case 'I':
            result = (*SR & 0x04) >> 2;
        break;
        case 'V':
            result = (*SR & 0x40) >> 6;
        break;
    }
	return result;
}

//logical and memory value with accumulator
void AND(uint16_t addr, uint8_t* A, uint8_t* SR){
    uint8_t val = *A & read_mem(addr);
    *A = val;
    SR_flags(val, "NZ", SR);
}

//logical or memory value with accumulator
void ORA(uint16_t addr, uint8_t* A){
    *A |= read_mem(addr);
}

//logical exclusive or memory value with accumulator
void EOR(uint16_t addr, uint8_t* A, uint8_t* SR){
    uint8_t val = *A ^ read_mem(addr);
    *A = val;
    SR_flags(val, "NZ", SR);
}

//adds value in memory to accumulator with carry
void ADC(uint16_t addr, uint8_t* A, uint8_t* SR){
    uint16_t val = read_mem(addr);
    uint16_t sum = *A + val + get_flag('C', SR);
    if((val & 0x80) == (*A & 0x80)){
        if((val & 0x80) != (sum &0x80)){
            set_flag('V', SR);
        }else{
            clear_flag('V', SR);
        }
    }
    *A = sum;
    SR_flags(sum, "NZC", SR);
}

//test bits in memory with accumulator
void BIT(uint16_t addr, uint8_t* A, uint8_t* SR){
    uint8_t val = read_mem(addr);
    *SR &= (val & 0xC0);
    SR_flags((val & *A), "Z", SR);
}

//branch if condition
void branch(char cond, uint8_t* SR, uint16_t* PC){
    bool branch = false;
    switch(cond){
        case 'C':
            if((*SR & 0x01) == 0){
                branch = true;
            }
        break;
        case 'S':
            if((*SR & 0x01) == 0x01){
                branch = true;
            }
        break;
        case 'Q':
            if((*SR & 0x02) == 0x02){
                branch = true;
            }
        break;
        case 'E':
            if((*SR & 0x02) == 0){
                branch = true;
            }
        break;
        case 'I':
            if((*SR & 0x80) == 0x80){
                branch = true;
            }
        break;
        case 'L':
            if((*SR & 0x80) == 0){
                branch = true;
            }
        break;
        case 'V':
            if((*SR & 0x40) == 0x40){
                branch = true;
            }
        break;
        case 'U':
            if((*SR & 0x40) == 0){
                branch = true;
            }
        break;
    }
    if(branch){
        JMP(PC);
    }
}

//subtract without saving but update flags
void compare(uint16_t addr, uint8_t* reg, uint8_t* SR){
    uint8_t val = *reg - read_mem(addr);
    SR_flags(val, "NZC", SR); 
}

//decrements value in memory or register
void decrement(uint16_t addr, uint8_t* reg){
    if(addr == 0){
        *reg -= 1;
    }else if(addr == 1){
        *reg -= 1;
    }else{
        uint8_t val = read_mem(addr);
        val -= 1;
        write_mem(addr, val);
    }
}

//increments value in memory or register
void increment(uint16_t addr, uint8_t* reg){
    if(addr == 0){
        *reg += 1;
    }else if(addr == 1){
        *reg += 1;
    }else{
        uint8_t val = read_mem(addr);
        val += 1;
        write_mem(addr, val);
    }
}

//jump to new location
void JMP(uint16_t* PC){
    uint16_t low = read_mem(*PC + 1);
    uint16_t high = read_mem(*PC + 2);
    *PC = ((high << 4) + low);
}

//jump to new location and save return address
void JSR(uint16_t* PC, uint8_t* SP){
    uint16_t retaddr = *PC + 2;
    uint8_t rhigh = retaddr >> 8;
    uint8_t rlow = retaddr & 0xFF;
    push(rhigh, SP);
    push(rlow, SP);
    uint16_t low = read_mem(*PC + 1);
    uint16_t high = read_mem(*PC + 2);
    *PC = ((high << 4) + low);
}

//Load from memory into register
void load(uint16_t addr, uint8_t* reg, uint8_t* SR){
    uint8_t val = read_mem(addr);
    *reg = val;
    SR_flags(val, "NZ", SR);
    
}

//push value to the stack
void push(uint8_t val, uint8_t* SP){
    write_mem(*SP, val);
    *SP += 1;
}

//pull value from stack
void pull(uint8_t* reg, uint8_t* SP){
   *SP -= 1;
   uint16_t SPaddr = (uint16_t) SP;
   *reg = read_mem(SPaddr);
}

//shift value right or left
void shift(uint16_t addr, char dir, uint8_t* A, uint8_t* SR){
    uint8_t val;
    if(addr == 0){
        val = *A;
        if(dir == 'L'){
            val <<= 1;
        }else if(dir == 'R'){
            val >>= 1;
        }
        *A = val;
    }else{
        val = read_mem(addr);
        if(dir == 'L'){
            val <<= 1;
        }else if(dir == 'R'){
            val >>= 1;
        }
        write_mem(addr, val);
    }
    SR_flags(val, "NZC", SR);
}

//rotate value right or left
void rotate(uint16_t addr, char dir, uint8_t* A, uint8_t* SR){
    uint8_t val;
    uint8_t c = get_flag('C', SR);
    if(addr == 0){
        val = *A;
        if(dir == 'L'){
            val <<= 1;
            if((val & 0x80) != c){
                if(c == 0){
                    val &= c;
                }else{
                    val |= c;
                }
            }
        }else if(dir == 'R'){
            val >>= 1;
        }
        *A = val;
    }else{
        val = read_mem(addr);
        if(dir == 'L'){
            val <<= 1;
            if((val & 0x80) != c){
                if(c == 0){
                    val &= c;
                }else{
                    val |= c;
                }
            }
        }else if(dir == 'R'){
            val >>= 1;
        }
        write_mem(addr, val);
    }
    SR_flags(val, "NZC", SR);
}

//return from subroutine
void RTS(uint16_t* PC, uint8_t* SP){
   uint8_t low = 0;
   uint8_t high = 0;
   pull(&low, SP);
   pull(&high, SP);
   *PC = high;
   *PC <<= 8;
   *PC += low;
}

//subtracts value in memory from accumulator with carry
void SBC(uint16_t addr, uint8_t* A, uint8_t* SR){
    uint16_t val = read_mem(addr);
    uint16_t diff = *A - val - get_flag('C', SR);
    if((val & 0x80) == (*A & 0x80)){
        if((val & 0x80) != (diff &0x80)){
            set_flag('V', SR);
        }else{
            clear_flag('V', SR);
        }
    }
    *A = diff;
    SR_flags(diff, "NZC", SR);
}

//store value in register to memory
void store(uint16_t addr, uint8_t* reg){
    uint8_t data = *reg;
    write_mem(addr, data);
}

//transfer value from one register to another
void transfer(uint8_t* reg0, uint8_t* reg1, uint8_t* SR){
    uint8_t val = *reg1;
    *reg0 = val;
    SR_flags(val, "NZ", SR);
}

void BRK(){
    //TODO
}

void RTI();