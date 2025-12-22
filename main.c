#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


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
    uint16_t pc;    // Program Counter - Endereço da proxima instrução.
    uint16_t mar;   // Memory Address Register - Endereço a ser acesado pela memoria. 
    uint16_t ibr;   // Instruction Buffer Register - Mantem toda a instrução.
    uint16_t ir;    // Instruction Register - Mantem o opcode da instrução.

    // Unidade Lógica e Aritmética
    int16_t mbr;    // Memory Buffer Register - Dado a ser lido/escrito na memoria.
    int16_t ac;     // Accumulator - acm.
    int16_t mq;     // Multiplier Quotient - usado na multiplicação e divisão.

    // Memória
    uint16_t memory[IMAS_MEM_SIZE];
} imas_t;

//Recebe a estrutura e inicializa os estados dos registradores zerados.
void imas_init(imas_t *imas){
	imas->pc = 0;
    imas->mar = 0;
    imas->ibr = 0;
    imas->ir = 0;
    imas->mbr = 0;
    imas->ac = 0;
    imas->mq = 0;
    
    // Zera a memória de forma segura (2 bytes por posição)
    memset(imas->memory, 0, IMAS_MEM_SIZE * sizeof(uint16_t));
}

void memory_read(imas_t *imas);

void memory_write(imas_t *imas, bool modify_address);

void io_read(imas_t *imas);

void io_write(imas_t *imas);

void step(imas_t *imas); // Executa um ciclo de instrução

int main(int argc, char *argv[]) {
	/* Check arguments */
	if(argc < 2) {
		printf("IMAS expects at least 2 arguments.\n");
		return 1;
	}

	/* Open input file */
	FILE *input_file = fopen(argv[1], "r");
	if(!input_file) {
		printf("Error opening %s!\n", argv[1]);
		return 1;
	}

	/* Set breakpoints */
	bool breakpoints[IMAS_MEM_SIZE] = {false};
	for(int i = 2; i < argc; i++) {
		int address = strtol(argv[i], NULL, 16);
		breakpoints[address] = true;
	}
	
	/* Initiate IMAS registers as zero */
	imas_t imas = {0};

	/* Zero IMAS memory */
	memset(&imas.memory, 0x0000, IMAS_MEM_SIZE);

	/* Fill IMAS memory */
	uint16_t address, buffer;
    while(fscanf(input_file, "%hX %hX%*[^\n]", &address, &buffer) == 2) {
		imas.memory[address] = buffer;
    }


	/* Processor running */
	//Enquanto falso faça
	bool imas_halt = false;
	do {
		/* PC before modifications */
		uint16_t original_pc = imas.pc;

		/* Fetch subcycle */
		// TODO: Fetch instruction from memory (like in IAS)

		/* Decode subcycle */
		// TODO: Put instruction fields in registers

		/* Execute subcycle */
		switch(imas.ir) {
		case IMAS_HALT:
			// TODO
			break;
		case IMAS_LOAD_M:
			// TODO
			break;
		case IMAS_LOAD_MQ:
			// TODO
			break;
		case IMAS_LOAD_MQ_M:
			// TODO
			break;
		case IMAS_STOR_M:
			// TODO
			break;
		case IMAS_STA_M:
			// TODO
			break;
		case IMAS_ADD_M:
			// TODO
			break;
		case IMAS_SUB_M:
			// TODO
			break;
		case IMAS_MUL_M:
			// TODO
			break;
		case IMAS_DIV_M:
			// TODO
			break;
		case IMAS_JMP_M:
			// TODO
			break;
		case IMAS_JZ_M:
			// TODO
			break;
		case IMAS_JNZ_M:
			// TODO
			break;
		case IMAS_JPOS_M:
			// TODO
			break;
		case IMAS_IN:
			// TODO
			break;
		case IMAS_OUT:
			// TODO
			io_write(&imas);
			break;
		default:
			printf("Invalid instruction %04X!\n", imas.ibr);
			imas_halt = true;
			break;
		}

		/* Breakpoint subcycle */
		if(breakpoints[original_pc]) {
			printf("<== IMAS Registers ==>\n");
			printf("PC = 0x%04hX\n", original_pc);
			printf("PC+ = 0x%04hX\n", imas.pc);
			printf("MAR = 0x%04hX\n", imas.mar);
			printf("IBR = 0x%04hX\n", imas.ibr);
			printf("IR = 0x%04hX\n", imas.ir);
			printf("MBR = 0x%04hX\n", imas.mbr);
			printf("AC = 0x%04hX\n", imas.ac);
			printf("MQ = 0x%04hX\n", imas.mq);
		}

	} while(!imas_halt);

	return 0;
}