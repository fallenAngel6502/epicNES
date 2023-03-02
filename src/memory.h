#ifndef MEMORY_H_
#define MEMORY_H_

//access memory
void write_mem(uint16_t addr, uint8_t data);
uint8_t read_mem(uint16_t addr);
//addressing modes
uint16_t imm(uint16_t PC);
uint16_t absolute(uint16_t PC);
uint16_t abs_x(uint16_t PC, uint8_t X);
uint16_t abs_y(uint16_t PC, uint8_t Y);
uint16_t ind_x(uint16_t PC, uint8_t X);
uint16_t ind_y(uint16_t PC, uint8_t Y);
uint16_t zero_pg(uint16_t PC);
uint16_t zero_pg_x(uint16_t PC, uint8_t X);
uint16_t zero_pg_y(uint16_t PC, uint8_t Y);

#endif