#ifndef IMAS_H
#define IMAS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define IMAS_MEM_SIZE 4096

/* Opcodes do IMAS */
#define IMAS_HALT      0x0
#define IMAS_LOAD_M    0x1
#define IMAS_LOAD_MQ   0x2
#define IMAS_LOAD_MQ_M 0x3
#define IMAS_STOR_M    0x4
#define IMAS_STA_M     0x5
#define IMAS_ADD_M     0x6
#define IMAS_SUB_M     0x7
#define IMAS_MUL_M     0x8
#define IMAS_DIV_M     0x9
#define IMAS_JMP_M     0xA
#define IMAS_JZ_M      0xB
#define IMAS_JNZ_M     0xC
#define IMAS_JPOS_M    0xD
#define IMAS_IN        0xE
#define IMAS_OUT       0xF

/* Estrutura do Processador */
typedef struct {
    // Unidade de Controle
    uint16_t pc;    // Program Counter
    uint16_t mar;   // Memory Address Register
    uint16_t ibr;   // Instruction Buffer Register
    uint16_t ir;    // Instruction Register

    // Unidade Lógica e Aritmética
    int16_t mbr;    // Memory Buffer Register
    int16_t ac;     // Accumulator
    int16_t mq;     // Multiplier Quotient

    // Memória
    uint16_t memory[IMAS_MEM_SIZE];
} imas_t;

/* Protótipos das Funções */
void imas_init(imas_t *imas);
void memory_read(imas_t *imas);
void memory_write(imas_t *imas, bool modify_address);
void io_read(imas_t *imas);
void io_write(imas_t *imas);
void step(imas_t *imas); // Executa um ciclo de instrução

#endif