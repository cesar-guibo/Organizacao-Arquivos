#ifndef _ARQUIVOS_
#define _ARQUIVOS_

#include <stdio.h>
#include <stdbool.h>

/* As funcoes e structs presentes nesse .h tem como proposito lidar com 
 * arquivos .csv e arquivos que seguem as especificacoes do projeto. */

/* Constante que define um caractere que indica que um byte deve ser 
 * tratado como lixo. */
#define LIXO_STD '$'
 
/* Representa se ocorreu algum erro em funcoes desse .h */
typedef enum {
	ARQUIVOS_SEM_ERRO,
	ARQUIVOS_ARQ_CORROMPIDO,
	ARQUIVOS_ERRO_LEITURA,
	ARQUIVOS_ERRO_ESCRITA, 
	ARQUIVOS_ERRO_SEEKING,
	ARQUIVOS_ERRO_ABERTURA,
	ARQUIVOS_ERRO_FECHAMENTO,
	ARQUIVOS_FALTA_DE_MEMORIA,
	ARQUIVOS_REGISTRO_CORROMPIDO
} ErroArquivos;

/* Arquivo .bin padrado que segue as especificacoes do projeto */
typedef struct arquivoStd ArquivoStd;

/* Arquivo .csv */
typedef struct arquivoCsv ArquivoCsv;

/* Funcao que cria um novo arquivo com as formatacoes de um ArquivoStd.
 *
 * Recebe um ponteiro para struct aquivoStd por referencia e uma string
 * com o nome do arquivo que deve ser cirado.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_criarArquivo(ArquivoStd **arq, char *nomeArq);

/* Funcao que abre um arquivo ja existente com as formatacoes de um ArquivoStd.
 *
 * Recebe um ponteiro para struct aquivoStd por referencia e uma string
 * com o nome do arquivo que deve ser aberto.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_abrirArquivo(ArquivoStd **arq, char *nomeArq);

/* Funcao que fecha um ArquivoStd.
 *
 * Recebe um ponteiro para struct arquvioStd por referencia.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_fecharArquivo(ArquivoStd **arq);

/* Funcao que insere um registro em um ArquivoStd.
 *
 * Recebe um ponteiro para o ArquivoStd que recebera a insercao e a string
 * com o registro formatado para arquivos std por parametro.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_inserirReg(ArquivoStd *arq, char *reg);

/* Funcao que imprime todos os registros de um ArquivoStd.
 *
 * Recebe o arquivo que deve ser impresso e a stream na qual a impressao deve
 * ocorrer.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosStd_imprimirTodosRegs(ArquivoStd *arq, FILE *outStream);

/* Funcao que abre um arquivo ArquiovCsv ja existente.
 *
 * Recebe um ponteiro que aponta para um ArquivoCsv por referencia e uma
 * string com o nome do arquivo que deve ser aberto.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_abrirArquivo(ArquivoCsv **arq, char *nomeArq);

/* Funcao que fecha um arquivo ArquivoCsv.
 *
 * Recebe como parametro um ponteiro que aponta para um ArquivoCsv por
 * referencia.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_fecharArquivo(ArquivoCsv **arq);

/* Funcao que formata um ArquivoCsv para um ArquivoStd. 
 *
 * Recebe um ponteiro para um ArquivoStd novo ja criado e um um ponteiro
 * para um ArquivoCsv ja aberto.
 *
 * Retorna um codigo de erro */
ErroArquivos arquivosCsv_formatarArquivoParaStd(ArquivoStd *arqRn, 
						ArquivoCsv *arqCSV);

/* Funcao que registra erros das funcoes desse .h na stream erroStream.
 *
 * Recebe um ErroArquivos e uma stream para registrar os erros.
 *
 * Nao retorna nada. */
void arquivosErros_logErros(ErroArquivos erro, FILE *erroStream);

#endif /* _ARQUIVOS_ */

