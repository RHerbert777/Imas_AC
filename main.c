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
    uint16_t memory[IMAS_MEM_SIZE]; //Memoria Total
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

//pega o endereço na istrução ibr
//coloca no MAR
//traz o dado da memoria para o MBR
void memory_read(imas_t *imas){
	//0x0FFF mascara necesaria para evitar que o procesador pegue informação 
	//do opcode que pegue somente o endereço
	imas->mar = imas->ibr & 0x0FFF;

	imas->mbr = imas->memory[imas->mar];
}

//pega o endereço na istrução ibr
//coloca no MAR
//escreve o dado do MBR na memoria
//caso onde queremos subistituir tudo da memoria e substituir apenas o endereço
void memory_write(imas_t *imas, bool modify_address){
	imas->mar = imas->ibr & 0x0FFF;

	if (modify_address){
		uint16_t atual = imas->memory[imas->mar]; //atual
		
		uint16_t opcode_preservado = atual & 0xF000; //preservo a instrução

		uint16_t novo_endereco = imas->mbr & 0x0FFF; //removo a nova instrução 

		imas->memory[imas->mar] = opcode_preservado | novo_endereco;//junto tudo

	}else{
		imas->memory[imas->mar] = imas->mbr;
	}
}
//entrada permite o usuario digitar
//scanf e salva no acumulador
void io_read(imas_t *imas){
	printf("IMAS Input > ");

	int temp;

	if(scanf(" %d",&temp) == 1){ //verifica se foi numero
		if (temp > 32767){
			printf("[AVISO] Valor %d muito alto! Truncado para 32767.\n", temp);
			imas->ac = 32767;
		}else if(temp < -32767){
			printf("[AVISO] Valor %d muito baixo! Truncado para -32767.\n", temp);
			imas->ac = -32767;
		}else{
			imas->ac = temp; 
		}
	}else{ //se for algo diferente de numero
		printf("[ERRO] Entrada invalida! AC zerado.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Limpa buffer
        imas->ac = 0;
	}
}
//saida permite o usuario visualizar o que tem no AC
//printa na tela o AC
void io_write(imas_t *imas){
	int16_t resultado = imas->ac;
	printf("IMAS Output >> %d\n", resultado);
}

void step(imas_t *imas){// Executa um ciclo de instrução

} 

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
	
	bool imas_halt = false;//Enquanto falso faça
	do {
		/* PC before modifications */
		uint16_t original_pc = imas.pc;

		switch(imas.ir) {
		case IMAS_HALT: //Atualiza o estado da cpu
			imas_halt = true;
		break;
		case IMAS_LOAD_M://Load from Memory to Accumulator.
			// 1. O barramento busca o dado na memória e coloca no MBR
    		memory_read(&imas); 
    
    		// 2. A CPU pega do MBR e guarda no Acumulador
    		imas.ac = imas.mbr;
    	break;
		case IMAS_LOAD_MQ: //Load MQ to Accumulator.
			
			imas.ac = imas.mq;
		break;
		case IMAS_LOAD_MQ_M://Load from Memory to MQ.
			memory_read(&imas);
			
			imas.mq = imas.mbr;
		break;
		case IMAS_STOR_M: //Store to Memory
			
		break;
		case IMAS_STA_M:
			// TODO
			break;
		case IMAS_ADD_M: //Pega acm soma o que esta na memoria e quarda acm
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
		case IMAS_JMP_M: //Vá para endereço M
			// TODO
			break;
		case IMAS_JZ_M://if ac == 0 pula para endereço M else ignora
			// TODO
			break;
		case IMAS_JNZ_M:// oposto do IMA_JZ_M
			// TODO
			break;
		case IMAS_JPOS_M://if AC >= 0 pula para M
			// TODO
			break;
		case IMAS_IN: //Pede um numero ao usuario e armazena em acm
			// TODO
			break;
		case IMAS_OUT: //printa o valor atual de AC
			// TODO
			io_write(&imas);
			break;
		default: //Se tudo der errado encere o programa
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