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
	int cycles = 0;

    switch(op){
        case 0x00:
            BRK();
            cycles = 7;
        break;
        case 0x01:
            ORA(ind_x(PC, X), &A, &SR);
            cycles = 6;
        break;
        case 0x05:
            ORA(zero_pg(PC), &A, &SR);
            cycles = 3;
        break;
        case 0x06:
            shift(zero_pg(PC), 'L', &A, &SR);
            cycles = 5;
        break;
        case 0x08:
            set_flag('B', &SR);
            push(SR, &SP);
            cycles = 3;
        break;
        case 0x09:
            ORA(imm(PC), &A, &SR);
            cycles = 2;
        break;
        case 0x0A:
            shift(0, 'L', &A, &SR);
            cycles = 2;
        break;
        case 0x0D:
            ORA(absolute(PC), &A, &SR);
            cycles = 4;
        break;
        case 0x0E:
            shift(absolute(PC), 'L', &A, &SR);
            cycles = 6;
        break;
        case 0x10:
            branch('L', &SR, &PC);
            cycles= 2;
            //TODO +1 cyc if page boundary is crossed, +1 cyc if branch is taken
        break;
        case 0x11:
            ORA(ind_y(PC, Y), &A, &SR);
            cycles= 5;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x15:
            ORA(zero_pg_x(PC, X), &A, &SR);
            cycles = 4;
        break;
        case 0x16:
            shift(zero_pg_x(PC, X), 'L', &A, &SR);
            cycles = 6;
        break;
        case 0x18:
            clear_flag('C', &SR);
            cycles = 2;
        break;
        case 0x19:
            ORA(abs_y(PC, Y), &A, &SR);
            cycles = 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1D:
            ORA(abs_x(PC, X), &A, &SR);
            cycles = 4;
            //TODO +1 cyc if page boundary is crossed
        break;
        case 0x1E:
            shift(abs_x(PC, X), 'L', &A, &SR);
            cycles = 7;
        break;
		case 0x20:
            JSR;
            cycles = 6;
        break;
		case 0x21:
            AND(ind_x(PC, X), &A, &SR);
            cycles = 6;
        break;
		case 0x24:
            BIT(zero_pg(PC), &A, &SR);
            cycles = 3;
        break;
		case 0x25:
            AND(zero_pg(PC), &A, &SR);
            cycles = 3;
        break;
		case 0x26:
            rotate(zero_pg(PC), 'L', &A, &SR);
            cycles = 5;
        break;
		case 0x28:
            pull(&SR, &SP);
            cycles = 4;
        break;
		case 0x29:
            AND(imm(PC), &A, &SR);
            cycles = 2;
        break;
        case 0x2A:
            rotate(0, 'L', &A, &SR);
            cycles += 2;
        break;
        case 0x2C:
            BIT(absolute(PC), &A, &SR);
            cycles = 4;
        break;
		case 0x2D:
            AND(absolute(PC), &A, &SR);
            cycles = 4;
        break;
		case 0x2E:
            AND(absolute(PC), &A, &SR);
            cycles = 6;
        break;
		case 0x30:
            branch('I', &SR, &PC);
            cycles= 2;
            //TODO +1 cyc if page boundary is crossed, +1 cyc if branch is taken
        break;
		case 0x31:
            AND(ind_y(PC, Y), &A, &SR);
            cycles = 5;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x35:
            AND(zero_pg_x(PC, X), &A, &SR);
            cycles = 4;
        break;
		case 0x36:
            rotate(zero_pg_x(PC, X), 'L', &A, &SR);
            cycles = 6;
        break;
		case 0x38:
            set_flag('C', &SR);
            cycles = 2;
        break;
		case 0x39:
            AND(abs_y(PC, Y), &A, &SR);
            cycles = 4;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x3D:
            AND(abs_x(PC, X), &A, &SR);
            cycles = 4;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x3E:
            rotate(abs_x(PC, X), 'L', &A, &SR);
            cycles = 7;
        break;
		case 0x40:
			//TODO
            //RTI();
            cycles = 6;
        break;
		case 0x41:
            EOR(ind_x(PC, X), &A, &SR);
            cycles = 6;
        break;
		case 0x45:
            EOR(zero_pg(PC), &A, &SR);
            cycles = 3;
        break;
		case 0x46:
            shift(zero_pg(PC), 'R', &A, &SR);
            cycles = 5;
        break;
		case 0x48:
            push(A, &SP);
            cycles = 3;
        break;
		case 0x49:
            EOR(imm(PC), &A, &SR);
            cycles = 2;
        break;
		case 0x4A:
            shift(0, 'R', &A, &SR);
            cycles = 2;
        break;
		case 0x4C:
            JMP(&PC);
            cycles = 3;
        break;
		case 0x4D:
            EOR(absolute(PC), &A, &SR);
            cycles = 4;
        break;
		case 0x4E:
            shift(absolute(PC), 'R', &A, &SR);
            cycles = 6;
        break;
		case 0x50:
            branch('V', &SR, &PC);
            cycles= 2;
            //TODO +1 cyc if page boundary is crossed, +1 cyc if branch is taken
        break;
		case 0x51:
            EOR(ind_y(PC, Y), &A, &SR);
            cycles = 5;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x55:
            EOR(zero_pg_x(PC, X), &A, &SR);
            cycles = 4;
        break;
		case 0x56:
            shift(zero_pg(PC), 'R', &A, &SR);
            cycles = 5;
        break;
		case 0x58:
            clear_flag('I', &SR);
            cycles = 2;
        break;
		case 0x59:
            EOR(abs_y(PC, Y), &A, &SR);
            cycles = 4;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x5D:
            EOR(abs_x(PC, X), &A, &SR);
            cycles = 4;
			//TODO +1 cyc if page boundary is crossed
        break;
		case 0x5E:
            shift(abs_x(PC, Y), 'R', &A, &SR);
            cycles = 7;
        break;
		case 0x60:
            RTS(&PC, &SP);
            cycles = 6;
        break;
		case 0x61:
            ADC(ind_x(PC, X), &A, &SR);
            cycles = 6;
        break;
		case 0x65:
            ADC(zero_pg(PC), &A, &SR);
            cycles = 3;
        break;
		case 0x66:
            rotate(zero_pg(PC), 'R', &A, &SR);
            cycles = 5;
        break;
		case 0x68:
            pull(&A, &SP);
            cycles = 4;
        break;
		case 0x69:
            ADC(imm(PC), &A, &SR);
            cycles = 2;
        break;
		case 0x6A:
            rotate(0, 'R', &A, &SR);
            cycles = 2;
        break;
		case 0x6C:
            JMP(&PC);
            cycles = 5;
        break;
		case 0x6D:
            ADC(absolute(PC), &A, &SR);
            cycles = 4;
        break;
		case 0x6E:
            rotate(absolute(PC), 'R', &A, &SR);
            cycles = 6;
        break;
    }
}
