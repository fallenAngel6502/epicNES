#ifndef OPCODES_H_
#define OPCODES_H_

void SR_flags(uint16_t val, char flags[], uint8_t* SR);

//TODO increment and decrement set flags!!

//op codes
void ADC(uint16_t addr, uint8_t* A, uint8_t* SR);
void AND(uint16_t addr, uint8_t* A, uint8_t* SR);
void BIT(uint16_t addr, uint8_t* A, uint8_t* SR);
void BRK();
//BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS
void branch(char cond, uint8_t* SR, uint16_t* PC);
//CLC, CLD, CLI, CLV
void clear_flag(char flag, uint8_t* SR);
//CMP, CPX, CPY
void compare(uint16_t addr, uint8_t* reg, uint8_t* SR);
//DEC, DEX, DEY
void decrement(uint16_t addr, uint8_t* reg, uint8_t* SR);
void EOR(uint16_t addr, uint8_t* A, uint8_t* SR);
//returns flag value
uint8_t get_flag(char flag, uint8_t* SR);
//INC, INX, INY
void increment(uint16_t addr, uint8_t* reg, uint8_t* SR);
void JMP(uint16_t* PC);
void JSR(uint16_t* PC, uint8_t* SP);
//LDA, LDX LDY
void load(uint16_t addr, uint8_t* reg, uint8_t* SR);
//ASL, LSR
void shift(uint16_t addr, char dir, uint8_t* A, uint8_t* SR);
void NOP();
void ORA(uint16_t addr, uint8_t* A, uint8_t* SR);
//PHA, PHP
void push(uint8_t val, uint8_t* SP);
//PLA, PLP
void pull(uint8_t* reg, uint8_t* SP);
//ROL, ROR
void rotate(uint16_t addr, char dir, uint8_t* A, uint8_t* SR);
void RTI();
void RTS(uint16_t* PC, uint8_t* SP);
void SBC(uint16_t addr, uint8_t* A, uint8_t* SR);
//SEC, SED, SEI
void set_flag(char flag, uint8_t* SR);
//STA, STX, STY
void store(uint16_t addr, uint8_t* reg);
//TAX, TAY, TSX, TXA, TXS, TYA
void transfer(uint8_t* reg0, uint8_t* reg1, uint8_t* SR);

#endif