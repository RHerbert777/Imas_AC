Como compilar o Imas.c

Recebe um arquivo.txt com os binarios e executa as operaçoes como o IAS

Esse codigo é muito simples e não trata os erros de usuario utilizar binarios errados e outros erros de
ponteiros apontando para NULL, por isso precisa atualizar as função para retornarem um ERROTYPE 

typedef enum {
    IMAS_SUCCESS = 0,          /* Tudo ocorreu bem */
    IMAS_HALT_REACHED,         /* O programa parou normalmente (HALT) */
    IMAS_ERR_MEM_ADDR,         /* Endereço de memória fora do limite (0-4095) */
    IMAS_ERR_INVALID_OPCODE,   /* Instrução não reconhecida pelo switch */
    IMAS_ERR_DIV_ZERO,         /* Tentativa de divisão por zero */
} imas_status_t;

