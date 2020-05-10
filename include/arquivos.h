#ifndef _ARQUIVOS_
#define _ARQUIVOS_

#include <stdio.h>

/* Constante que define um caractere que indica que um byte e lixo. */
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

/* Arquivo .bin padrado que segue as especificacoes do projeto */
typedef struct arquivoStd ArquivoStd;

/* Arquivo .csv */
typedef struct arquivoCsv ArquivoCsv;

/* Funcao que cria um arquivo com as formatacoes std.
 * Recebe um ponteiro para struct aquivoStd por referencia e uma string
 * com o nome do arquivo que deve ser cirado.
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_criarArquivo(ArquivoStd **arq, char *nomeArq);

/* Funcao que abre um arquivo com as formatacoes std.
 * Recebe um ponteiro para struct aquivoStd por referencia e uma string
 * com o nome do arquivo que deve ser aberto.
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_abrirArquivo(ArquivoStd **arq, char *nomeArq);

/* Funcao que fecha um ArquivoStd.
 * Recebe um ponteiro para struct arquvioStd por referencia.
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_fecharArquivo(ArquivoStd **arq);

/* Funcao que insere um registro em um ArquivoStd.
 * Recebe um ponteiro para o ArquivoStd que recebera a insercao e a string
 * com o registro formatado para arquivos std por parametro.
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_inserirReg(ArquivoStd *arq, char *reg);

/* Funcao que imprime todos os registros de um ArquivoStd.
 * Recebe o arquivo que deve ser impresso e a stream na qual a impressao deve
 * ocorrer.
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_imprimirTodosRegs(ArquivoStd *arq, FILE *outStream);

/* Funcao que abre um arquivo .csv.
 * Recebe um ponteiro que aponta para um ArquivoCsv por referencia e uma
 * string com o nome do arquivo que deve ser aberto.
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_abrirArquivo(ArquivoCsv **arq, char *nomeArq);

/* Funcao que fecha um arquivo .csv.
 * Recebe como parametro um ponteiro que aponta para um ArquivoCsv por
 * referencia.
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_fecharArquivo(ArquivoCsv **arq);

/* Funcao que formata um arquivo .csv para um .bin padrao para esse projeto. 
 * Recebe um ponteiro para um ArquivoStd novo ja criado e um um ponteiro
 * para um ArquivoCsv ja aberto.
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_formatarArquivoParaStd(ArquivoStd *arqRn, 
						ArquivoCsv *arqCSV);

/* Funcao que registra erros das funcoes desse .h na stream erroStream.
 * Recebe um ErroArquivos e uma stream para registrar os erros. */
void arquivos_logErros(ErroArquivos erro, FILE *erroStream);

#endif /* _ARQUIVOS_ */

