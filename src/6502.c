#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"
#include "opcode.h"

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


//decodes and executes op code
void cpu_cycle(uint8_t op);

int main(){
    return 0;
}



//decodes and executes op code
void cpu_cycle(uint8_t op){

    switch(op){
        case 0x00:
            BRK();
            PC += 7;
        break;
        case 0x01:
            ORA(ind_x(PC, X), &A, &SR);
            PC += 6;
        break;
        case 0x05:
            ORA(zero_pg(PC), &A, &SR);
            PC += 3;
        break;
        case 0x06:
            shift(zero_pg(PC), 'L', &A, &SR);
            PC += 5;
        break;
        case 0x08:
            set_flag('B', &SR);
            push(SR, &SP);
            PC += 3;
        break;
        case 0x09:
            ORA(imm(PC), &A, &SR);
            PC += 2;
        break;
        case 0x0A:
            shift(0, 'L', &A, &SR);
            PC += 2;
        break;
        case 0x0D:
            ORA(absolute(PC), &A, &SR);
            PC += 4;
        break;
        case 0x0E:
            shift(absolute(PC), 'L', &A, &SR);
            PC += 6;
        break;
        case 0x10:
            branch('I', &SR, &PC);
            PC += 3;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x11:
            ORA(ind_y(PC, Y), &A, &SR);
            PC += 5;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x15:
            ORA(zero_pg_x(PC, X), &A, &SR);
            PC += 4;
        break;
        case 0x16:
            shift(zero_pg_x(PC, X), 'L', &A, &SR);
            PC += 6;
        break;
        case 0x18:
            clear_flag('C', &SR);
            PC += 2;
        break;
        case 0x19:
            ORA(abs_y(PC, Y), &A, &SR);
            PC += 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1D:
            ORA(abs_x(PC, X), &A, &SR);
            PC += 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1E:
            shift(abs_x(PC, X), 'L', &A, &SR);
            PC += 7;
        break;
	case 0x20:
            JSR;
            PC += 6;
        break;
	case 0x21:
            AND(ind_x(PC, X), &A, &SR);
            PC += 6;
        break;
	case 0x24:
            BIT(zero_pg(PC), &A, &SR);
            PC += 3;
        break;
	case 0x25:
            AND(zero_pg(PC), &A, &SR);
            PC += 3;
        break;
	case 0x26:
            rotate(zero_pg(PC), 'L', &A, &SR);
            PC += 5;
        break;
	case 0x28:
            pull(&SR, &SP);
            PC += 4;
        break;
	case 0x29:
            AND(imm(PC), &A, &SR);
            PC += 2;
        break;
        //TODO figure out accumlator pointer
        case 0x2A:
            //rotate(&A, 'L');
            PC += 2;
        break;
        case 0x2C:
            BIT(absolute(PC), &A, &SR);
            PC += 4;
        break;
    }
}





