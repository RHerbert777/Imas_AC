#include "Imas.h"
#include <string.h>

/* Inicializa o estado do processador */
void imas_init(imas_t *imas) {
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