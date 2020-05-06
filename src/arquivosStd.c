#include "../include/arquivos.h" 
#include "../include/regsNascimento.h" 
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#define RRN_ATUAL INT_MAX

const unsigned TAM_CAMPOS_TAM_FIXO[6] = {
	sizeof(int), sizeof(int), TAM_DATA, TAM_SEXO, TAM_ESTADO, TAM_ESTADO
};

struct cabecalho {
	char status;
	int RRNproxRegistro;
	int numeroRegistrosInseridos;
	int numeroRegistrosRemovidos;
	int numeroRegistrosAtualizados;
	char lixo[TAM_LIXO_CABECALHO_ARQ_STD];
};

struct arquivoStd {
	FILE *stream;
	struct cabecalho cabecalho;
};

static ErroArquivos inserirCabecalho(FILE *arq, struct cabecalho *c)
{
	if (fseek(arq, 0, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}

	if (fwrite(&c->status, sizeof(char), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;	
	}
	if (fwrite(&c->RRNproxRegistro, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	if (fwrite(&c->numeroRegistrosInseridos, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	if (fwrite(&c->numeroRegistrosRemovidos, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	if (fwrite(&c->numeroRegistrosAtualizados,sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	if (fwrite(&c->lixo, sizeof(char), 111, arq) != 111) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	return ARQUIVOS_SEM_ERRO;	
}

static inline void novoCabecalhoInit(struct cabecalho *cabecalho)
{
	cabecalho->status = '0';
	cabecalho->status = '0';
	cabecalho->RRNproxRegistro = 0;
	cabecalho->numeroRegistrosInseridos = 0;
	cabecalho->numeroRegistrosAtualizados = 0;
	cabecalho->numeroRegistrosRemovidos = 0;
	memset(cabecalho->lixo, LIXO_STD, TAM_LIXO_CABECALHO_ARQ_STD);

	return;
}

ErroArquivos arquivosStd_criarArquivo(struct arquivoStd **arq, char *nomeArq)
{
	ErroArquivos erro;
	if ((*arq = malloc(sizeof(struct arquivoStd))) == NULL) {
		return ARQUIVOS_FALTA_DE_MEMORIA;
	}

	if (((*arq)->stream = fopen(nomeArq, "wb+")) == NULL) {
		free(*arq);
		*arq = NULL;
		return ARQUIVOS_ERRO_ABERTURA;
	}

	novoCabecalhoInit(&(*arq)->cabecalho);
	erro = inserirCabecalho((*arq)->stream, &(*arq)->cabecalho);
	if (erro != ARQUIVOS_SEM_ERRO) {
		fclose((*arq)->stream);
		free(*arq);
		*arq = NULL;
		return erro;
	}

	return ARQUIVOS_SEM_ERRO;
}

static ErroArquivos carregarCabecalho(FILE *arq, struct cabecalho *c)
{
	char emUso = '0';

	if (fseek(arq, 0, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}

	if (fread(&c->status, sizeof(char), 1, arq) != 1) {
		return ARQUIVOS_ERRO_LEITURA;	
	}

	if (c->status == '0') {
		return ARQUIVOS_ARQ_CORROMPIDO;
	}

	if (fread(&c->RRNproxRegistro, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_LEITURA;
	}
	if (fread(&c->numeroRegistrosInseridos, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_LEITURA;
	}
	if (fread(&c->numeroRegistrosRemovidos, sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_LEITURA;
	}
	if (fread(&c->numeroRegistrosAtualizados,sizeof(int), 1, arq) != 1) {
		return ARQUIVOS_ERRO_LEITURA;
	}
	if (fread(&c->lixo, sizeof(char), 111, arq) != 111) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	
	/* Assegura que sempre que o cabecalho for carregado,
	 * o arquivo tenha como status no cabecalho '0' */
	if (fseek(arq, 0, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}
	if (fwrite(&emUso, sizeof(char), 1, arq) != 1) {
		return ARQUIVOS_ERRO_ESCRITA;
	}

	return ARQUIVOS_SEM_ERRO;	
}

ErroArquivos arquivosStd_abrirArquivo(struct arquivoStd **arq, char *nomeArq)
{
	ErroArquivos erro;
	
	if ((*arq = malloc(sizeof(struct arquivoStd))) == NULL) {
		return ARQUIVOS_FALTA_DE_MEMORIA;
	}

	if (((*arq)->stream = fopen(nomeArq, "rb+")) == NULL) {
		free(*arq);
		*arq = NULL;
		return ARQUIVOS_ERRO_ABERTURA; } 
	erro = carregarCabecalho((*arq)->stream, &(*arq)->cabecalho);
	if (erro != ARQUIVOS_SEM_ERRO) {
		fclose((*arq)->stream);
		free(*arq);
		*arq = NULL;
		return erro;
	}

	return ARQUIVOS_SEM_ERRO;
}

ErroArquivos arquivosStd_fecharArquivo(struct arquivoStd **arq)
{
	(*arq)->cabecalho.status = '1';
	inserirCabecalho((*arq)->stream, &(*arq)->cabecalho);

	if (fclose((*arq)->stream) != 0) {
		return ARQUIVOS_ERRO_FECHAMENTO;	
	}

	free(*arq);
	*arq = NULL;

	return ARQUIVOS_SEM_ERRO;
}

static long getOffset(struct arquivoStd *arq, unsigned rrn)
{
	return (rrn * TAM_REGS + TAM_CABECALHO_ARQ_STD);
}

ErroArquivos arquivosStd_inserirReg(struct arquivoStd *arq, char *reg)
{
	if (fseek(arq->stream, getOffset(arq, arq->cabecalho.RRNproxRegistro),
		  SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}

	if (fwrite(reg, sizeof(char), TAM_REGS, arq->stream) != TAM_REGS) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	
	arq->cabecalho.RRNproxRegistro++;
	arq->cabecalho.numeroRegistrosInseridos++;

	return ARQUIVOS_SEM_ERRO;
}

/* Para usar o rrn do atual registro, passar RRN_ATUAL como argumento 
 * para o parametro rrn */
static ErroArquivos lerRegistro(char *destino, ArquivoStd *arq, 
				unsigned rrn)
{
	long offset;

	offset = getOffset(arq, rrn);
	if ((rrn != RRN_ATUAL) && (ftell(arq->stream) != offset)) {
		if (fseek(arq->stream, offset, SEEK_SET) != 0) {
			return ARQUIVOS_ERRO_SEEKING;
		}
	}

	if (fread(destino, sizeof(char), TAM_REGS, arq->stream) != TAM_REGS) {
		if (ferror(arq->stream)) {
			return ARQUIVOS_ERRO_LEITURA;
		}
		return ARQUIVOS_EOF;
	}

	return ARQUIVOS_SEM_ERRO;
}

static bool regFoiRemovido(char *reg)
{
	return (*(int *)reg == ARQ_STD_REG_REMOVIDO);
}

static ErroArquivos regParse(struct regNascimento *rn, char *reg) 
{
	int tamCamposTamVar[2];
	char auxBuffer[50];
	char *fimReg = reg + TAM_REGS;

	tamCamposTamVar[0] = *(int *)reg;
	tamCamposTamVar[1] = *(int *)(reg + sizeof(int));
	
	/* Salva os campos de tamanho variavel em um buffer para poder 
	 * adicionar um '\0' no final da string e setar o campo da struct 
	 * regNascimento com a funcao regsNascimento_setCampo() */
	reg += 2 * sizeof(int);
	for (int i = 0; i < 2; i++) {	
		if (reg >= fimReg) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		memcpy(auxBuffer, reg, tamCamposTamVar[i]);
		auxBuffer[tamCamposTamVar[i]] = '\0';
		regsNascimento_setCampo(rn, auxBuffer, CIDADE_MAE + i);
		reg += tamCamposTamVar[i];
	}

	reg += TAM_CAMPOS_VAR - (tamCamposTamVar[0] + tamCamposTamVar[1]);

	for (int i = 0; i < 6; i++) {
		if (reg >= fimReg) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		memcpy(auxBuffer, reg, TAM_CAMPOS_TAM_FIXO[i]);
		auxBuffer[TAM_CAMPOS_TAM_FIXO[i]] = '\0';
		regsNascimento_setCampo(rn, auxBuffer, ID_NASCIMENTO + i);
		reg += TAM_CAMPOS_TAM_FIXO[i];
	}

	return ARQUIVOS_SEM_ERRO;
}

ErroArquivos arquivoStd_imprimirTodosRegs(ArquivoStd *arq, FILE *outStream) 
{
	ErroArquivos erro;
	struct regNascimento rn;
	char buffer[TAM_REGS];

	if (fseek(arq->stream, TAM_CABECALHO_ARQ_STD, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}

	while((erro = lerRegistro(buffer, arq, RRN_ATUAL)) == ARQUIVOS_SEM_ERRO) {
		if (!regFoiRemovido(buffer)) {
			if ((erro = regParse(&rn, buffer)) != ARQUIVOS_SEM_ERRO) {
				return erro;
			}
			regsNascimento_imprimir(&rn, outStream);
		}
	}			

	if (erro != ARQUIVOS_EOF) {
		return erro;
	}

	return ARQUIVOS_SEM_ERRO;
}
