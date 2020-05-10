#include "../include/arquivos.h"
#include "../include/utils.h"
#include "../include/regsNascimento.h"
#include <stdlib.h>
#include <string.h>

struct arquivoCsv {
	FILE *stream;
	int offsetIniSegDados;
};

/* Constante que determian o tamanho em bytes dos buffers utilizados
 * nesse modulo. */
#define TAM_BUFFERS 128

ErroArquivos arquivosCsv_abrirArquivo(ArquivoCsv **arq, char *nomeArq)
{
	char buff[TAM_BUFFERS];

	if ((*arq = malloc(sizeof(struct arquivoCsv))) == NULL) {
		return ARQUIVOS_FALTA_DE_MEMORIA;
	}

	if (((*arq)->stream = fopen(nomeArq, "rb+")) == NULL) {
		free(*arq);
		*arq = NULL;
		return ARQUIVOS_ERRO_ABERTURA;
	}
	
	if (fgets(buff, TAM_BUFFERS, (*arq)->stream) == NULL) {
	    	if (ferror((*arq)->stream)) {
			fclose((*arq)->stream);
			free(*arq);
			*arq = NULL;
			return  ARQUIVOS_ERRO_LEITURA;
		}
	}
		 	
	(*arq)->offsetIniSegDados = ftell((*arq)->stream);

	return ARQUIVOS_SEM_ERRO;	
}

ErroArquivos arquivosCsv_fecharArquivo(ArquivoCsv **arq)
{
	if (*arq == NULL) {
		return ARQUIVOS_SEM_ERRO;
	}

	if (fclose((*arq)->stream) != 0) {
		return ARQUIVOS_ERRO_FECHAMENTO;
	}

	free (*arq);
	*arq = NULL;

	return ARQUIVOS_SEM_ERRO;
}

/* Funcao que analisa um registro do arquivo .csv e armazena cada uma das 
 * informacoes nele presentes em seu devido campo em rn.
 * Recebe uma struct regNascimento, uma registro do .csv em regCsv o tamanho
 * do regCsv.
 * Retorna o tipo de erro */
static ErroArquivos parseRegistroArquivo(struct regNascimento *rn, char *regCsv,
				  int tamReg) 
{
	char *auxPtr;

	auxPtr = utils_strtok(regCsv, ",\r\n");
	for (RegNascimentoCampos i = CIDADE_MAE; i <= ESTADO_BEBE; i++) {
		if (auxPtr == NULL) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		if (auxPtr - regCsv >= tamReg) {
			return ARQUIVOS_FALTA_DE_MEMORIA;
		}
		regsNascimento_setCampo(rn, auxPtr, i);
		auxPtr = utils_strtok(NULL, ",\r\n");
	} 
	
	return ARQUIVOS_SEM_ERRO;	
}


ErroArquivos arquivosCsv_formatarArquivoParaStd(struct arquivoStd *arqStd,
						struct arquivoCsv *arqCsv)
{
	struct regNascimento rn;
	char buff[TAM_BUFFERS];
	ErroArquivos erro;


	if (fseek(arqCsv->stream, arqCsv->offsetIniSegDados, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	} 

	while (fgets(buff, TAM_BUFFERS, arqCsv->stream) != NULL) {
		memset(&rn, 0, sizeof(struct regNascimento));
		erro = parseRegistroArquivo(&rn, buff, TAM_BUFFERS);
		if (erro != ARQUIVOS_SEM_ERRO) {
			return erro;
		}
		regsNascimento_formatarParaRegArquivoStd(buff, &rn);
		erro = arquivosStd_inserirReg(arqStd, buff);
		if (erro != ARQUIVOS_SEM_ERRO) {
			return erro;
		}
	}
	if (ferror(arqCsv->stream))
		return erro;

	return ARQUIVOS_SEM_ERRO;
}
