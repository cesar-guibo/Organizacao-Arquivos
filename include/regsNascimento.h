#ifndef _REGS_NASCIMENTO_
#define _REGS_NASCIMENTO_

#include <stdio.h>

/* As funcoes desse .h tem como proposito auxiliar operacoes que precisam
 * manipular regsNascimento. */

/* Constante que define o tamanho dos campos que representam sexo. */
#define TAM_SEXO 1

/* Constante que define o tamanho das strings que representam uma
 * data. */
#define TAM_DATA 10

/* Constante que define o tamanho das strings que representam um 
 * estado. */
#define TAM_ESTADO 2

/* Constante que define o tamanho de um registro separado para campos 
 * de tamanho variavel nos registros de nascimento. */
#define TAM_MAX_CAMPOS_VAR 97

/* Constante que define o numero de campos de tamanho variavel nos registros 
 * de nascimento. */
#define NUM_CMAPOS_TAM_VAR 2

/* Constante que define o numero de campos de tamanho fixo nos registros de 
 * nascimento. */
#define NUM_CMAPOS_TAM_FIXO 6

/* Constante que define o numero maximo de bytes que podem ser ocupados com 
 * dados em um registro de nascimento. */
#define TAM_REGISTROS 128

/* Array de constantes que armazena os tamanhos dos campos de tamanho fixo 
 * em ordem para facilitar operacoes com esses dados. */
extern const unsigned TAM_CAMPOS_TAM_FIXO[NUM_CMAPOS_TAM_FIXO];

/* Enum com cada campo de uma struct regNascimento. E utilizado para facilitar
 * operacoes com esssa struct. */
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

struct regNascimento {
	char cidadeMae[TAM_MAX_CAMPOS_VAR + 1];
	char cidadeBebe[TAM_MAX_CAMPOS_VAR + 1];
	int idNascimento;
	int idadeMae;
	char dataNascimento[TAM_DATA + 1];
	char sexoBebe;
	char estadoMae[TAM_ESTADO + 1];
	char estadoBebe[TAM_ESTADO + 1];
};

/* Funcao para passar as informacoes do valor que vem em forma de string
 * string para o campo selecionado da struct registro de nascimento.
 *
 * Recebe a struct regNascimento em que o valor deve ser armazenado,
 * a string com o valor e o campo a ser escrito.
 *
 * Nao retorna nada.*/
void regsNascimento_setCampo(struct regNascimento *rn, char *valor,
			       RegNascimentoCampos campo);

/* Formata os dados de uma struct regNascimento para um registro de
 * um ArquivoStd.
 *
 * Recebe um buffer de desino que deve possuir no minimo TAM_REGISTROS bytes,
 * o qual recebera a struct formatada e a struct regNascimento que deve ser 
 * formatada. 
 *
 * Nao retorna nada */
void regsNascimento_formatarParaRegArquivoStd(char *dest,
						struct regNascimento *rn);

/* Funcao que imprime um registro.
 *
 * Recebe o regNascimento a ser impresso e a stream em que se imprime.
 *
 * Nao retorna nada */
void regsNascimento_imprimir(struct regNascimento *rn, FILE *outStream);

#endif /* _REGS_NASCIMENTO_ */
