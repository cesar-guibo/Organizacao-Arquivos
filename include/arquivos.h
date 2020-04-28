#ifndef _ARQUIVOS_
#define _ARQUIVOS_

#include <stdio.h>
#include <stdbool.h>

 /* Arquivo de cabecalho que define enums, structs e funcoes relevantes
 * para o desenvolvimento do trabalho 1 relaizado para a disciplina 
 * de Organizacao de Arquivos. */

 /* Constante que sinaliza que o dado deve
 * ser tratado como lixo. */
const char LIXO = '$';

 /* Constante que define o tamanho fixo
 * dos cabecalhos. */
const int CABECALHO_TAM = 128;

 /* Constante que sinaliza que um campo ou
 * um registro possui tamanho variavel. */
const int TAM_VARIAVEL = -1;

 /* Constante que define o comprimento
 * maximo das tags. */
const int TAG_TAM = 15;

 /* Constante que define o caractere que  
 * determina que nenhum delimitador e 
 * utilizado. */
const char NENHUM_DELIMITADOR = '!';

 /* Representa se ocorreu algum erro em operacoes que envolvem
  * streams. */
typedef enum {
	STREAM_OK, ERRO_LEITURA, ERRO_ESCRITA, EOF_ALCANCADO, STREAM_NULA
} FileCode;

 /* Representa os tipos de dados que um campo pode conter para 
 * que possa ser utilizado pelas funcoes definidas nesse .h. */
typedef enum {
	INT, LONG, LONG_LONG, SHORT, UNSIGNED, UNSIGNED_LONG,
	UNSIGNED_LONG_LONG, UNSIGNED_SHORT, CHAR, UNSIGNED_CHAR,
	SIGNED_CHAR, FLOAT, DOUBLE, LONG_DOUBLE
} TipoDado;


/* Armazena os dados presentes no cabecalho
 * de um arquivo. Definicao encapsulada para 
 * restringir o acesso aos dados do Cabecalho. */
struct Cabecalho;

/* Armazena como os registros devem ser armazenados e
 * interpretados em um arquivo. */
struct RegistrosConfig {
	/* Tamanho fixo do registro, se o
	 * tamanho for variavel, deve receber
	 * o valor TAM_VARIAVEL. */
	int tamanhoFixo;

	/* Armazena as configuracoes de como cada campo
	 * deve ser armazenado e interpretado. */
	struct CampoConfig {
		/* Armazena qual tipo de dado
		 * deve ser utilizado para
		 * interpretar um campo. */
		TipoDado tipoDado;

		/* Armazena a tag que identifica
		 * o campo. */
		char tag[TAG_TAM]; 

	 	/* Tamanho fixo do campo, se o
	 	 * tamanho for variavel, deve receber
	 	 * o valor TAM_VARIAVEL. */
		int tamanhoFixo;	

		/* Sinaliza se e utilizado um 
		 * delimitador de tamanho que 
		 * sempre precede o dado para 
		 * informar o tamanho de um 
		 * campo variavel. */
		bool delimitadorTamanho;

		/* Armazena que caractere e usado
		 * para sinalizar o fim de um 
		 * campo de tamanho variavel. Se 
		 * nao e utilizado um delimitador,
		 * deve receber NENHUM_DELIMITADOR. */
		char delimitadorCampo;
	};

	/* Vetor de configuracoes de campos.*/
#ifdef NUMERO_CAMPOS 
	/* Se NUMERO_CAMPOS tiver sido definido,
 	* o vetor camposConfig e alocado estaticamente */
	struct CampoConfig camposConfig[NUMERO_CAMPOS];
#else
	/* Se NUMERO_CAMPOS nao foi definido, 
	 * o vetor camposConfig deve ser alocado
	 * dinamicamente*/
	struct CampoConfig *camposConfig;

	/* Se NUMERO_CAMPOS nao foi definido,
	 * Armazena o numero de campos
	 * contido no registro. */
	int numeroCampos;
#endif
};

struct Cabecalho *cabecalhoAlloc(void);

void cabecalhoFree(struct Cabecalho *cabecalho);

/* Carrega um cabecalho armazenado em um arquivo para poder utilizar as
 * informacoes nele armazenado em tempo de acesso da memoria RAM. */
FileCode carregarCabecalho(struct Cabecalho *cabecalho, FILE *stream);

/* Formata um arquivo origem em um arquivo destino. Precisa receber 
 * como parametros as streams dos arquivos de destino e de origem e
 * as configuracoes dos reggistros contidos nos dois arquivos. */
FileCode formatarArquivo(FILE *streamOrigem, FILE *streamDestino,
			     struct RegistrosConfig *configOrigem,
			     struct RegistrosConfig *configDestino);

/* Imprime todos os dados armazenados em um arquivo. Precisa receber
 * como parametros a stream que se comunica com o arquivo, o cabecalho
 * do arquivo e a configuracao dos registros do arquivo. */
FileCode imprimirTodosDadosArquivo(FILE *stream,
				      struct Cabecalho *cabecalho,
				      struct RegistrosConfig *regConfig);

/* Imprime uma mensagem de erro para a stream selecionada */
void logErroOperacaoArquivo(FileCode status, FILE *errorStream);

#endif /* _ARQUIVOS_ */
