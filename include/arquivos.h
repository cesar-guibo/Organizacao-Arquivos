#ifndef _ARQUIVOS_
#define _ARQUIVOS_

#include <stdio.h>
#include <stdbool.h>

/** @file "include/arquivos.h"
 * Arquivo de cabecalho que define enums, structs e funcoes relevantes
 * para o desenvolvimento do trabalho 1 relaizado para a disciplina 
 * de Organizacao de Arquivos. */

 /** Constante que sinaliza que o dado deve
 * ser tratado como lixo. */
const char LIXO = '$';

 /** Constante que define o tamanho fixo
 * dos cabecalhos. */
const int CABECALHO_TAM = 128;

 /** Constante que sinaliza que um campo ou
 * um registro possui tamanho variavel. */
const int TAM_VARIAVEL = -1;

 /** Constante que define o comprimento
 * maximo das tags. */
const int TAG_TAM = 15;

 /** Constante que define o caractere que  
 * determina que nenhum delimitador e 
 * utilizado. */
const char NENHUM_DELIMITADOR = '!';

 /** Representa se ocorreu algum erro em operacoes que envolvem
  * streams. */
typedef enum {
	STREAM_OK, ERRO_LEITURA, ERRO_ESCRITA, EOF_ALCANCADO, STREAM_NULA
} StatusStream;

 /** Representa os tipos de dados que um campo pode conter para 
 * que possa ser utilizado pelas funcoes definidas nesse .h. */
typedef enum {
	INT, LONG, LONG_LONG, SHORT, UNSIGNED, UNSIGNED_LONG,
	UNSIGNED_LONG_LONG, UNSIGNED_SHORT, CHAR, UNSIGNED_CHAR,
	SIGNED_CHAR, FLOAT, DOUBLE, LONG_DOUBLE
} TipoDado;


/** @struct Cabecalho "include/arquivos.h" "include/arquivos.h"
 * Armazena os dados presentes no cabecalho
 * de um arquivo. Definicao encapsulada para 
 * restringir o acesso aos dados do Cabecalho. */
struct Cabecalho;

/** @struct RegistrosConfig "include/arquivos.h" "include/arquivos.h"
 * Armazena como os registros devem ser armazenados e
 * interpretados em um arquivo. */
struct RegistrosConfig {
	 /** Tamanho fixo do registro, se o
	 * tamanho for variavel, deve receber
	 * o valor TAM_VARIAVEL. */
	int tamanhoFixo;

	/** @struct CampoConfig "include/arquivos.h" "include/arquivos.h"
	 * Armazena as configuracoes de como cada campo
	 * deve ser armazenado e interpretado. */
	struct CampoConfig {
		 /** Armazena qual tipo de dado
		 * deve ser utilizado para
		 * interpretar um campo. */
		TipoDado tipoDado;

		 /** Armazena a tag que identifica
		 * o campo. */
		char tag[TAG_TAM]; 

	 	/** Tamanho fixo do campo, se o
	 	* tamanho for variavel, deve receber
	 	* o valor TAM_VARIAVEL. */
		int tamanhoFixo;	

		 /** Sinaliza se e utilizado um 
		 * delimitador de tamanho que 
		 * sempre precede o dado para 
		 * informar o tamanho de um 
		 * campo variavel. */
		bool delimitadorTamanho;

		 /** Armazena que caractere e usado
		 * para sinalizar o fim de um 
		 * campo de tamanho variavel. Se 
		 * nao e utilizado um delimitador,
		 * deve receber NENHUM_DELIMITADOR. */
		char delimitadorCampo;
	};

#ifdef NUMERO_CAMPOS 
	 /** Vetor de configuracoes de campos.
	 * Se NUMERO_CAMPOS tiver sido definido,
 	 * o vetor camposConfig e alocado estaticamente */
	struct CampoConfig camposConfig[NUMERO_CAMPOS];
#else
	 /** Vetor de configuracoes de campos. 
	 * Se NUMERO_CAMPOS nao foi definido, 
	 * o vetor camposConfig deve ser alocado
	 * dinamicamente*/
	struct CampoConfig *camposConfig;

	 /** Armazena o numero de campos
	 * contido no registro se 
	 * NUMERO_CAMPOS nao foi definido */
	int numeroCampos;
#endif
};

/** Alloca memoria para um cabecalho.
 * @return endereco onde foi alocada a struct Cabecalho */
struct Cabecalho *cabecalhoAlloc(void);

/** Libera da memoria um cabecalho 
 * @param cabecalho cabecalho que sera liberado */
void cabecalhoFree(struct Cabecalho *cabecalho);

/** Carrega um cabecalho armazenado em um arquivo para poder utilizar as
 * informacoes nele armazenado em tempo de acesso da memoria RAM.
 *
 * @param cabecalho cabecalho que recebera o cabecalho carregado de um
 * arquivo.
 * @param stream stream que se comunica com o arquivo que tera o cabecalho 
 * carregado.
 *
 * @return o status da stream */
StatusStream carregarCabecalho(struct Cabecalho *cabecalho, FILE *stream);

/** Formata um arquivo origem em um arquivo destino.
 *
 * @param streamOrigem stream que se comunica com o arquivo de origem. 
 * @param streamDestino stream que se comunica com o arquivo de destino. 
 * @param configOrigem configuracoes do registro do arquivo de origem. 
 * @param configDestino configuracoes de formatacao para o arquivo destino 
 *
 * @return status das streams */
StatusStream formatarArquivo(FILE *streamOrigem, FILE *streamDestino,
			     struct RegistrosConfig *configOrigem,
			     struct RegistrosConfig *configDestino);

/** Recupera dados de um registro de um arquivo
 *
 * @param stream: stream que se comunica com o arquivo que deve ser 
 * imprimido 
 *
 * @param cabecalho: cabecalho do arquivo carregado na RAM
 * @param regConfig: configuracoes dos registros armazenados no arquivo
 *
 * @return status da stream */
StatusStream imprimirTodosDadosArquivo(FILE *stream,
				      struct Cabecalho *cabecalho,
				      struct RegistrosConfig *regConfig);

#ifdef ERRO_LOG_STREAM
/** Imprime uma mensagem de erro para a stream selecionada. Se
 * ERRO_LOG_STREAM foi definida, a errorStream nao preciase ser passada
 * como parametro.
 *
 * @param status status da stream que se comunica com um arquivo */
void logErroOperacaoArquivo(StatusStream status);
#else
/** Imprime uma mensagem de erro para a stream selecionada. Se
 * ERRO_LOG_STREAM nao foi definida, a errorStream preciase ser passada
 * como parametro.
 *
 * @param status status da stream que se comunica com um arquivo 
 * @param errorStream stream aonde deve ser impressa a mensagem
 * de erro */
void logErroOperacaoArquivo(StatusStream status, FILE *errorStream);
#endif

#endif /* _ARQUIVOS_ */
