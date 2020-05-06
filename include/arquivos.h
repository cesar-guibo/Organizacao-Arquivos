#ifndef _ARQUIVOS_
#define _ARQUIVOS_

#include <stdio.h>

/* Constante que define o tamanho maximo do cabecalho dos arquviso do tipo
 * arquivoRN */
#define TAM_CABECALHO_ARQ_STD 128

#define TAM_LIXO_CABECALHO_ARQ_STD 111

#define ARQ_STD_REG_REMOVIDO -1

/* Constante que sinaliza que o dado deve ser tratado como lixo. */
#define LIXO_STD '$'
 
/* Representa se ocorreu algum erro em operacoes que envolvem aruqivos
 * manipulados por essa interface. */
typedef enum {
	ARQUIVOS_SEM_ERRO,
	ARQUIVOS_ARQ_CORROMPIDO,
	ARQUIVOS_ERRO_LEITURA,
	ARQUIVOS_ERRO_ESCRITA, 
	ARQUIVOS_ERRO_SEEKING,
	ARQUIVOS_ERRO_ABERTURA,
	ARQUIVOS_ERRO_FECHAMENTO,
	ARQUIVOS_FALTA_DE_MEMORIA,
	ARQUIVOS_EOF,
	ARQUIVOS_REGISTRO_CORROMPIDO,
} ErroArquivos;

/* Arquivo .bin que segue as especificacoes padroes do projeto */
typedef struct arquivoStd ArquivoStd;

/* Arquivo .csv */
typedef struct arquivoCsv ArquivoCsv;

ErroArquivos arquivosStd_criarArquivo(ArquivoStd **arq, char *nomeArq);

ErroArquivos arquivosStd_abrirArquivo(ArquivoStd **arq, char *nomeArq);

ErroArquivos arquivosStd_fecharArquivo(ArquivoStd **arq);

ErroArquivos arquivosStd_inserirReg(ArquivoStd *arq, char *reg);

/* Imprime todos os registros armazenados em um ArquivoStd para
 * a stream outStream. */
ErroArquivos arquivoStd_imprimirTodosRegs(ArquivoStd *arq, FILE *outStream);

ErroArquivos arquivosCsv_abrirArquivo(ArquivoCsv **arq, char *nomeArq);

ErroArquivos arquivosCsv_fecharArquivo(ArquivoCsv **arq);

ErroArquivos arquivosCsv_formatarArquivoParaStd(ArquivoStd *arqRn, 
						ArquivoCsv *arqCSV);

#endif

