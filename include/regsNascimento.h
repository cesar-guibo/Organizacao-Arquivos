#ifndef _REGS_NASCIMENTO_
#define _REGS_NASCIMENTO_

#include <stdio.h>

/* Constante que define o numero de campos em um registro de em arquivoRN. */
#define NUM_CAMPOS_REG 8

/* Constante que define o tamanho maximo dos registros de um arquivoRN. */
#define TAM_REGS 128

#define INT_VAZIO -1

#define REG_RMD -1

#define TAM_SEXO 1

#define TAM_DATA 10

#define TAM_ESTADO 2

#define TAM_CAMPOS_VAR 97

typedef enum {
	CIDADE_MAE = 0,
	CIDADE_BEBE = 1,
	ID_NASCIMENTO = 2,
	IDADE_MAE = 3, 
	DATA_NASCIMENTO = 4,
	SEXO_BEBE = 5,
	ESTADO_MAE = 6,
	ESTADO_BEBE = 7
} RegNascimentoCampos;

 /* Representa se ocorreu algum erro em operacoes que envolvem
  * registros de nascimento. */
typedef enum {
	RN_SEM_ERRO, RN_FALTA_DE_MEMORIA
} ErroRn;

struct regNascimento {
	char cidadeMae[TAM_CAMPOS_VAR + 1];
	char cidadeBebe[TAM_CAMPOS_VAR + 1];
	int idNascimento;
	int idadeMae;
	char dataNascimento[TAM_DATA + 1];
	char sexoBebe;
	char estadoMae[TAM_ESTADO + 1];
	char estadoBebe[TAM_ESTADO + 1];
};

ErroRn regsNascimento_setCampo(struct regNascimento *rn, char *valor,
			       RegNascimentoCampos campo);

ErroRn regsNascimento_formatarParaRegArquivoStd(char dest[TAM_REGS],
						struct regNascimento *rn);

void regsNascimento_imprimir(struct regNascimento *rn, FILE *outStream);

/* Imprime uma mensagem de erro para a stream selecionada */
void regsNascimento_logErroOperacao(ErroRn erro, FILE *errorStream);

#endif /* _REGS_NASCIMENTO_ */
