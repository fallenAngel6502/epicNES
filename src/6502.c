#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"

//registers
uint8_t A = 0;
uint8_t X = 0;
uint8_t Y = 0;
//program counter
uint16_t PC = 0;
//stack pointer
uint8_t SP = 0x1;
//status register
uint8_t SR = 0x20;


void SR_flags(uint16_t val, char flags[]);

//op codes
void ADC(uint16_t addr);
void AND(uint16_t addr);
void BIT(uint16_t addr);
void BRK();
//BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS
void branch(char cond);
//CLC, CLD, CLI, CLV
void clear_flag(char flag);
//CMP, CPX, CPY
void compare(uint16_t addr, uint8_t* reg);
//DEC, DEX, DEY
void decrement(uint16_t addr);
void EOR(uint16_t addr);
//INC, INX, INY
void increment(uint16_t addr);
void JMP();
void JSR();
//LDA, LDX LDY
void load(uint16_t addr, uint8_t* reg);
//ASL, LSR
void shift(uint16_t addr, char dir);
void NOP();
void ORA(uint16_t addr);
//PHA, PHP
void push(uint8_t val);
//PLA, PLP
void pull(uint8_t* reg);
//ROL, ROR
void rotate(uint16_t addr, char dir);
void RTI();
void RTS();
void SBC(uint16_t addr);
//SEC, SED, SEI
void set_flag(char flag);
//STA, STX, STY
void store(uint16_t addr, uint8_t* reg);
//TAX, TAY, TSX, TXA, TXS, TYA
void transfer(uint8_t* reg0, uint8_t* reg1);

//decodes and executes op code
void cpu_cycle(uint8_t op);

int main(){
    return 0;
}

/*--------------------------------------------------------
OP code functions
--------------------------------------------------------*/

//determines state of SR flags N, Z, C
void SR_flags(uint16_t val, char flags[]){
    
    char N[] = "N";
    char Z[] = "Z";
    char C[] = "C";
 
    //N
    if(strstr(flags, N)){
        if(((val & 0x80)>>7)== 1){
            set_flag('N');
        }else{
            clear_flag('N');
        }
    }
    //Z
    if(strstr(flags, Z)){
        if(val == 0){
            set_flag('Z');
        }else{
            clear_flag('Z');
        }
    }
    //C
    if(strstr(flags, C)){
        if(val > 0xFF){
            set_flag('C');
        }else{
            clear_flag('C');
        }
    }
}

//set a flag in SR
void set_flag(char flag){
    switch(flag){
        case 'N':
            SR |= 0x80;
        break;
        case 'Z':
            SR |= 0x02;
        break;
        case 'C':
            SR |= 0x01;
        break;
        case 'I':
            SR |= 0x04;
        break;
        case 'V':
            SR |= 0x40;
        break;
        case 'B':
            SR |= 0x10;
        break;
    }
}

//clear a flag in SR
void clear_flag(char flag){
    switch(flag){
        case 'N':
            SR &= 0x7F;
        break;
        case 'Z':
            SR &= 0xFD;
        break;
        case 'C':
            SR &= 0xFE;
        break;
        case 'I':
            SR &= 0xFB;
        break;
        case 'V':
            SR &= 0xBF;
        break;
    }
}

//logical and memory value with accumulator
void AND(uint16_t addr){
    uint8_t val = A & read_mem(addr);
    A = val;
    SR_flags(val, "NZ");
}

//logical or memory value with accumulator
void ORA(uint16_t addr){
    A |= read_mem(addr);
}

//logical exclusive or memory value with accumulator
void EOR(uint16_t addr){
    uint8_t val = A ^ read_mem(addr);
    A = val;
    SR_flags(val, "NZ");
}

//adds value in memory to accumulator with carry
void ADC(uint16_t addr){
    uint16_t val = read_mem(addr);
    uint16_t sum = A + val + (SR &0x01);
    if((val & 0x80) == (A & 0x80)){
        if((val & 0x80) != (sum &0x80)){
            set_flag('V');
        }else{
            clear_flag('V');
        }
    }
    A = sum;
    SR_flags(sum, "NZC");
}

//test bits in memory with accumulator
void BIT(uint16_t addr){
    uint8_t val = read_mem(addr);
    SR &= (val & 0xC0);
    SR_flags((val & A), "Z");
}

//branch if condition
void branch(char cond){
    bool branch = false;
    switch(cond){
        case 'C':
            if((SR & 0x01) == 0){
                branch = true;
            }
        break;
        case 'S':
            if((SR & 0x01) == 0x01){
                branch = true;
            }
        break;
        case 'Q':
            if((SR & 0x02) == 0x02){
                branch = true;
            }
        break;
        case 'E':
            if((SR & 0x02) == 0){
                branch = true;
            }
        break;
        case 'I':
            if((SR & 0x80) == 0x80){
                branch = true;
            }
        break;
        case 'L':
            if((SR & 0x80) == 0){
                branch = true;
            }
        break;
        case 'V':
            if((SR & 0x40) == 0x40){
                branch = true;
            }
        break;
        case 'U':
            if((SR & 0x40) == 0){
                branch = true;
            }
        break;
    }
    if(branch){
        JMP();
    }
}

//subtract without saving but update flags
void compare(uint16_t addr, uint8_t* reg){
    uint8_t val = *reg - read_mem(addr);
    SR_flags(val, "NZC"); 
}

//decrements value in memory or register
void decrement(uint16_t addr){
    if(addr == 0){
        X -= 1;
    }else if(addr == 1){
        Y -= 1;
    }else{
        uint8_t val = read_mem(addr);
        val -= 1;
        write_mem(addr, val);
    }
}

//increments value in memory or register
void increment(uint16_t addr){
    if(addr == 0){
        X += 1;
    }else if(addr == 1){
        Y += 1;
    }else{
        uint8_t val = read_mem(addr);
        val += 1;
        write_mem(addr, val);
    }
}

//jump to new location
void JMP(){
    uint16_t low = read_mem(PC + 1);
    uint16_t high = read_mem(PC + 2);
    PC = ((high << 4) + low);
}

//jump to new location and save return address
void JSR(){
    uint16_t retaddr = PC + 2;
    uint8_t rhigh = retaddr >> 8;
    uint8_t rlow = retaddr & 0xFF;
    push(rhigh);
    push(rlow);
    uint16_t low = read_mem(PC + 1);
    uint16_t high = read_mem(PC + 2);
    PC = ((high << 4) + low);
}

//Load from memory into register
void load(uint16_t addr, uint8_t* reg){
    uint8_t val = read_mem(addr);
    *reg = val;
    SR_flags(val, "NZ");
    
}

//push value to the stack
void push(uint8_t val){
    write_mem(SP, val);
    SP += 1;
}

//pull value from stack
void pull(uint8_t* reg){
    SP -= 1;
   *reg = read_mem(SP);
}

//shift value right or left
void shift(uint16_t addr, char dir){
    uint8_t val;
    if(addr == 0){
        val = A;
        if(dir == 'L'){
            val <<= 1;
        }else if(dir == 'R'){
            val >>= 1;
        }
        A = val;
    }else{
        val = read_mem(addr);
        if(dir == 'L'){
            val <<= 1;
        }else if(dir == 'R'){
            val >>= 1;
        }
        write_mem(addr, val);
    }
    SR_flags(val, "NZC");
}

//rotate value right or left
void rotate(uint16_t addr, char dir){
    uint8_t val;
    uint8_t c = (SR & 0x01) << 7;
    if(addr == 0){
        val = A;
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
        A = val;
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
    SR_flags(val, "NZC");
}

//return from subroutine
void RTS(){
   uint8_t low = 0;
   uint8_t high = 0;
   pull(&low);
   pull(&high);
   PC = high;
   PC <<= 8;
   PC += low;
}

//subtracts value in memory from accumulator with carry
void SBC(uint16_t addr){
    uint16_t val = read_mem(addr);
    uint16_t diff = A - val - (SR &0x01);
    if((val & 0x80) == (A & 0x80)){
        if((val & 0x80) != (diff &0x80)){
            set_flag('V');
        }else{
            clear_flag('V');
        }
    }
    A = diff;
    SR_flags(diff, "NZC");
}

//store value in register to memory
void store(uint16_t addr, uint8_t* reg){
    uint8_t data = *reg;
    write_mem(addr, data);
}

//transfer value from one register to another
void transfer(uint8_t* reg0, uint8_t* reg1){
    uint8_t val = *reg1;
    *reg0 = val;
    SR_flags(val, "NZ");
}

void BRK(){
    //TODO
}

void RTI();

//decodes and executes op code
void cpu_cycle(uint8_t op){

    switch(op){
        case 0x00:
            BRK();
            PC += 7;
        break;
        case 0x01:
            ORA(ind_x(PC, X));
            PC += 6;
        break;
        case 0x05:
            ORA(zero_pg(PC));
            PC += 3;
        break;
        case 0x06:
            shift(zero_pg(PC), 'L');
            PC += 5;
        break;
        case 0x08:
            set_flag('B');
            push(SR);
            PC += 3;
        break;
        case 0x09:
            ORA(imm(PC));
            PC += 2;
        break;
        case 0x0A:
            shift(0, 'L');
            PC += 2;
        break;
        case 0x0D:
            ORA(absolute(PC));
            PC += 4;
        break;
        case 0x0E:
            shift(absolute(PC), 'L');
            PC += 6;
        break;
        case 0x10:
            branch('I');
            PC += 3;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x11:
            ORA(ind_y(PC, Y));
            PC += 5;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x15:
            ORA(zero_pg_x(PC, X));
            PC += 4;
        break;
        case 0x16:
            shift(zero_pg_x(PC, X), 'L');
            PC += 6;
        break;
        case 0x18:
            clear_flag('C');
            PC += 2;
        break;
        case 0x19:
            ORA(abs_y(PC, Y));
            PC += 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1D:
            ORA(abs_x(PC, X));
            PC += 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1E:
            shift(abs_x(PC, X), 'L');
            PC += 7;
        break;
	case 0x20:
            JSR;
            PC += 6;
        break;
	case 0x21:
            AND(ind_x(PC, X));
            PC += 6;
        break;
	case 0x24:
            BIT(zero_pg(PC));
            PC += 3;
        break;
	case 0x25:
            AND(zero_pg(PC));
            PC += 3;
        break;
	case 0x26:
            rotate(zero_pg(PC), 'L');
            PC += 5;
        break;
	case 0x28:
            pull(&SR);
            PC += 4;
        break;
	case 0x29:
            AND(imm(PC));
            PC += 2;
        break;
        //TODO figure out accumlator pointer
        case 0x2A:
            //rotate(&A, 'L');
            PC += 2;
        break;
    }
}





