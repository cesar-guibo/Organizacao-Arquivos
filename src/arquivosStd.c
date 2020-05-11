#include "../include/arquivos.h" 
#include "../include/regsNascimento.h" 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Constante que sinaliza que o rrn selecionado e o em que o arquivo ja se
 * encontra atualmente. */
#define RRN_ATUAL -1 /* Assume o maior valor possivel para unsigned int */

/* Constante que sinaliza que um registro esta logicamente removido. */
#define REGISTRO_REMOVIDO -1

/* Constante que define o tamanho maximo do cabecalho de um ArquivoStd. */
#define TAM_CABECALHO 128

/* Constante que define o tamanho do lixo presente em um cabecalho de um
 * ArquivoStd. */
#define TAM_LIXO_CABECALHO 111

struct cabecalho {
	char status;
	int RRNproxRegistro;
	int numeroRegistrosInseridos;
	int numeroRegistrosRemovidos;
	int numeroRegistrosAtualizados;
};

struct arquivoStd {
	FILE *stream;
	struct cabecalho cabecalho;
};

/* Funcao que escreve o cabecalho no arquivo.
 *
 * Recebe a stream de um arquivo e o cabecalho a ser inserido.
 *
 * Retorna um codigo de erro */
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
	return ARQUIVOS_SEM_ERRO;	
}

/* Funcao que inicializa um novo cabecalho.
 *
 * Recebe uma struct cabecalho por parametro e nao tem retorno */
static void novoCabecalhoInit(struct cabecalho *cabecalho)
{
	cabecalho->status = '0';
	cabecalho->status = '0';
	cabecalho->RRNproxRegistro = 0;
	cabecalho->numeroRegistrosInseridos = 0;
	cabecalho->numeroRegistrosAtualizados = 0;
	cabecalho->numeroRegistrosRemovidos = 0;

	return;
}

ErroArquivos arquivosStd_criarArquivo(struct arquivoStd **arq, char *nomeArq)
{
	char *lixo;
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

	/* Insere lixo no cabecalho para manter o offset de inicio dos
	 * registros de dados em 128. Stream ja apontando para o final
	 * dos dados do cabecalho ao final de inserirCabecalho(). */
	lixo = malloc(TAM_LIXO_CABECALHO * sizeof(char));
	memset(lixo, LIXO_STD, TAM_LIXO_CABECALHO);
	if (fwrite(lixo, sizeof(char), TAM_LIXO_CABECALHO, (*arq)->stream)
	    != TAM_LIXO_CABECALHO) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	free(lixo);

	return ARQUIVOS_SEM_ERRO;
}

/* Funcao que le o cabecalho de um arquivo e o carrega na struct cabecalho.
 *
 * Recebe como parametro a stream do arquivo e a struct cabecalho a ser
 * carregada.
 *
 * Retorna um codigo de erro */
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
	
	/* Assegura que sempre que o cabecalho terminar de ser carregado,
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
		return ARQUIVOS_ERRO_ABERTURA; 
	} 
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

/* Funcao que executa um seek para um determinado rrn em um arquivo padrao
 * do projeto. 
 *
 * Recebe a struct arquivoStd do arquivo em que se deseja realizar o seek e o 
 * rrn para o qual relizar a operacao.
 *
 * Retorna um codigo de erro */
static ErroArquivos seekToRrn(struct arquivoStd *arq, unsigned rrn)
{
	unsigned offset;

	offset = (rrn * TAM_REGISTROS) + TAM_CABECALHO;
	if (ftell(arq->stream) != offset) {
		if (fseek(arq->stream, offset, SEEK_SET) != 0) {
			return ARQUIVOS_ERRO_SEEKING;
		}
	} 
	return ARQUIVOS_SEM_ERRO;
}

ErroArquivos arquivosStd_inserirReg(struct arquivoStd *arq, char *reg)
{
	ErroArquivos erro;

	erro = seekToRrn(arq, arq->cabecalho.RRNproxRegistro);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return erro;
	}

	if (fwrite(reg, sizeof(char), TAM_REGISTROS, arq->stream)
	    != TAM_REGISTROS) {
		return ARQUIVOS_ERRO_ESCRITA;
	}
	
	arq->cabecalho.RRNproxRegistro++;
	arq->cabecalho.numeroRegistrosInseridos++;

	return ARQUIVOS_SEM_ERRO;
}

/* Para ler o registro com o rrn do atual offset, passar RRN_ATUAL como
 * argumento  para o parametro rrn */
static ErroArquivos lerRegistro(char *destino, struct arquivoStd *arq, 
				unsigned rrn)
{
	ErroArquivos erro;

	if (rrn != RRN_ATUAL) {
		erro = seekToRrn(arq, rrn);
		if (erro != ARQUIVOS_SEM_ERRO) {
			return erro;
		}
	}

	if (fread(destino, sizeof(char), TAM_REGISTROS, arq->stream)
	    != TAM_REGISTROS) {
		return ARQUIVOS_ERRO_LEITURA;
	}

	return ARQUIVOS_SEM_ERRO;
}

/* Funcao para verificar se o registro foi removido.
 *
 * Recebe o registro por parametro e tem retorno booleano */
static bool regFoiRemovido(char *reg)
{
	return (*(int *)reg == REGISTRO_REMOVIDO);
}

/* Funcao que analisa um arquivo do arquivo padrao do projeto e armazena cada
 * uma das informacoes nele presentes em seu devido campo em rn.
 *
 * Recebe uma struct regNascimento e um registro do ArquivoStd.
 *
 * Retorna um codigo de erro */
static ErroArquivos parseRegistroArquivo(struct regNascimento *rn,
					 char *registroArq) 
{
	int tamCamposTamVar[NUM_CMAPOS_TAM_VAR];
	char auxBuffer[TAM_MAX_CAMPOS_VAR];
	char *fimRegArq; /* Ponteiro que aponta para o final do registroArq */

	fimRegArq = registroArq + TAM_REGISTROS;

	/* Salva os tamanhos do campos de tamanho variavel no array
	 * tamCamposTamVar */
	for (int i = 0; i < NUM_CMAPOS_TAM_VAR; i++) {
		tamCamposTamVar[i] = *(int *)(registroArq + (i * sizeof(int)));	
	}

	/* Salva os campos em um buffer para poder adicionar um '\0' no final
	 * das strings e assim poder setar os campos da struct regNascimento
	 * com a funcao regsNascimento_setCampo() */

	registroArq += NUM_CMAPOS_TAM_VAR * sizeof(int);
	for (int i = 0; i < NUM_CMAPOS_TAM_VAR; i++) {	
		if (registroArq >= fimRegArq) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		memcpy(auxBuffer, registroArq, tamCamposTamVar[i]);
		auxBuffer[tamCamposTamVar[i]] = '\0';
		regsNascimento_setCampo(rn, auxBuffer, CIDADE_MAE + i);
		registroArq += tamCamposTamVar[i];
	}

	/* Avanca o ponteiro registroArq para os campos de tamanho fixo */
	registroArq += TAM_MAX_CAMPOS_VAR;
	for (int i = 0; i < NUM_CMAPOS_TAM_VAR; i++) {
		registroArq -= tamCamposTamVar[i];
	}

	for (int i = 0; i < NUM_CMAPOS_TAM_FIXO; i++) {
		if (registroArq >= fimRegArq) {
			return ARQUIVOS_REGISTRO_CORROMPIDO;
		}
		memcpy(auxBuffer, registroArq, TAM_CAMPOS_TAM_FIXO[i]);
		auxBuffer[TAM_CAMPOS_TAM_FIXO[i]] = '\0';
		regsNascimento_setCampo(rn, auxBuffer, ID_NASCIMENTO + i);
		registroArq += TAM_CAMPOS_TAM_FIXO[i];
	}

	return ARQUIVOS_SEM_ERRO;
}

ErroArquivos arquivosStd_imprimirTodosRegs(ArquivoStd *arq, FILE *outStream) 
{
	ErroArquivos erro;
	struct regNascimento rn;
	char buffer[TAM_REGISTROS];

	if (fseek(arq->stream, TAM_CABECALHO, SEEK_SET) != 0) {
		return ARQUIVOS_ERRO_SEEKING;
	}

	erro = lerRegistro(buffer, arq, RRN_ATUAL);
	while(erro == ARQUIVOS_SEM_ERRO) {
		if (!regFoiRemovido(buffer)) {
			erro = parseRegistroArquivo(&rn, buffer);
			if (erro != ARQUIVOS_SEM_ERRO) {
				return erro;
			}
			regsNascimento_imprimir(&rn, outStream);
		}
		erro = lerRegistro(buffer, arq, RRN_ATUAL);
	}			

	if (ferror(arq->stream)) {
		return erro;
	}

	return ARQUIVOS_SEM_ERRO;
}
