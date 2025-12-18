#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Imas.h"

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