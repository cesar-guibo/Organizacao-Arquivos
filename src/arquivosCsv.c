#include "../include/arquivos.h"
#include "../include/auxiliar_lib.h"
#include "../include/regsNascimento.h"
#include <stdlib.h>
#include <string.h>

struct arquivoCsv {
	FILE *stream;
	int offsetIniSegDados;
};

ErroArquivos arquivosCsv_abrirArquivo(ArquivoCsv **arq, char *nomeArq)
{
	char buff[256];

	if ((*arq = malloc(sizeof(struct arquivoCsv))) == NULL) {
		return ARQUIVOS_FALTA_DE_MEMORIA;
	}

	if (((*arq)->stream = fopen(nomeArq, "rb+")) == NULL) {
		free(*arq);
		*arq = NULL;
		return ARQUIVOS_ERRO_ABERTURA;
	}
	
	if ((fgets(buff, 256, (*arq)->stream) == NULL) && ferror((*arq)->stream)) {
		fclose((*arq)->stream);
		free(*arq);
		*arq = NULL;
		return  ARQUIVOS_ERRO_LEITURA;
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

static ErroArquivos regParse(struct regNascimento *rn, char *regCsv,
			     int tamBuffer) 
{
	char *auxPtr;

	auxPtr = aux_strtok(regCsv, ",\r\n");
	for (RegNascimentoCampos i = CIDADE_MAE; i <= ESTADO_BEBE; i++) {
		if (auxPtr == NULL) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		if (auxPtr - regCsv >= tamBuffer) {
			return ARQUIVOS_FALTA_DE_MEMORIA;
		}
		regsNascimento_setCampo(rn, auxPtr, i);
		auxPtr = aux_strtok(NULL, ",\r\n");
	} 
	
	return ARQUIVOS_SEM_ERRO;	
}


ErroArquivos arquivosCsv_formatarArquivoParaStd(struct arquivoStd *arqStd,
						struct arquivoCsv *arqCsv)
{
	struct regNascimento rn;
	char buff[TAM_REGS];
	ErroArquivos erro;


	if (fseek(arqCsv->stream, arqCsv->offsetIniSegDados, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	} 
	while (fgets(buff, TAM_REGS, arqCsv->stream) != NULL) {
		memset(&rn, 0, sizeof(struct regNascimento));
		erro = regParse(&rn, buff, TAM_REGS);
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
