#include "../include/regsNascimento.h"
#include "../include/arquivos.h"
#include <stdlib.h>
#include <string.h>

/* Constante que define um inteiro que sinaliza que um campo esta vazio. */
#define INT_VAZIO -1

const unsigned TAM_CAMPOS_TAM_FIXO[NUM_CMAPOS_TAM_FIXO] = {
	sizeof(int),
	sizeof(int),
	TAM_DATA,
	TAM_SEXO,
	TAM_ESTADO,
	TAM_ESTADO
};

void regsNascimento_setCampo(struct regNascimento *rn, char *valor, 
			      RegNascimentoCampos campo)
{
	switch(campo) {
		/* Garantido pelas especificacoes que o tamanho dos valors
		 * dos campos de tamanho variaveis nao excedem 
		 * TAM_MEM_COMPARTILHADA */
		case CIDADE_MAE:
			strcpy(rn->cidadeMae, valor);
			break;
		case CIDADE_BEBE:
			strcpy(rn->cidadeBebe, valor);
			break;
		case ID_NASCIMENTO:
			rn->idNascimento = ((strlen(valor) == 0)
					    ? INT_VAZIO : atoi(valor));
			break;
		case IDADE_MAE:
			rn->idadeMae = ((strlen(valor) == 0)
					? INT_VAZIO : atoi(valor));
			break;
		case DATA_NASCIMENTO:
			strcpy(rn->dataNascimento, valor);
			if (strlen(valor) == 0) {
				memset(rn->dataNascimento + 1, LIXO_STD,
				       TAM_DATA);
			}
			break;
		case SEXO_BEBE:
			rn->sexoBebe = (strlen(valor) == 0) ? '0' : *valor;
			break;
		case ESTADO_MAE:
			strcpy(rn->estadoMae, valor);
			if (strlen(valor) == 0) {
				memset(rn->estadoMae + 1, LIXO_STD,
				       TAM_ESTADO);
			}
			break;
		case ESTADO_BEBE:
			strcpy(rn->estadoBebe, valor);
			if (strlen(valor) == 0) {
				memset(rn->estadoBebe + 1, LIXO_STD, 
				       TAM_ESTADO);
			}
			break;
	}
	return;
}

void regsNascimento_formatarParaRegArquivoStd(char *destino, 
						struct regNascimento *rn)
{
	char *auxPtrDest;
	int tamCidadeBebe;
	int tamCidadeMae;
	int quantidadeLixo;

	auxPtrDest = destino;

	tamCidadeMae = strlen(rn->cidadeMae);
	memcpy(auxPtrDest, &tamCidadeMae, sizeof(int));
	auxPtrDest += sizeof(int);
	tamCidadeBebe = strlen(rn->cidadeBebe);
	memcpy(auxPtrDest, &tamCidadeBebe, sizeof(int));
	auxPtrDest += sizeof(int);

	memcpy(auxPtrDest, rn->cidadeMae, tamCidadeMae);
	auxPtrDest += tamCidadeMae;
	memcpy(auxPtrDest, rn->cidadeBebe, tamCidadeBebe);
	auxPtrDest += tamCidadeBebe;

	quantidadeLixo = TAM_MAX_CAMPOS_VAR - (tamCidadeMae + tamCidadeBebe);
	memset(auxPtrDest, LIXO_STD, quantidadeLixo);
	auxPtrDest += quantidadeLixo;	

	memcpy(auxPtrDest, &rn->idNascimento, sizeof(int));
	auxPtrDest += sizeof(int);

	memcpy(auxPtrDest, &rn->idadeMae, sizeof(int));
	auxPtrDest += sizeof(int);

	memcpy(auxPtrDest, &rn->dataNascimento, TAM_DATA);
	auxPtrDest += TAM_DATA;

	memcpy(auxPtrDest, &rn->sexoBebe, TAM_SEXO);
	auxPtrDest += TAM_SEXO;

	memcpy(auxPtrDest, rn->estadoMae, TAM_ESTADO);
	auxPtrDest += TAM_ESTADO;

	memcpy(auxPtrDest, rn->estadoBebe, TAM_ESTADO);

	return;
}

void regsNascimento_imprimir(struct regNascimento *rn, FILE *outStream) 
{
	char *cidade;
	char *sexo;
	char *estado;
	char *data;

	cidade = (strlen(rn->cidadeBebe) != 0) ? rn->cidadeBebe : "-";
	estado = (strlen(rn->estadoBebe) != 0) ? rn->estadoBebe : "-";
	data = (strlen(rn->dataNascimento) != 0) ? rn->dataNascimento : "-";

	if (rn->sexoBebe == '0') {
		sexo = "IGNORADO";
	} else if (rn->sexoBebe == '1') {
		sexo = "MASCULINO";
	} else {
		sexo = "FEMININO";
	} 

	printf("Nasceu em %s/%s, em %s, um bebê de sexo %s.\n",
		cidade, estado, data, sexo);

	return;
}
