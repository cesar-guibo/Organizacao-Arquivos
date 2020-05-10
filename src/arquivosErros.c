#include "../include/arquivos.h"
#include <stdio.h>

void arquivosErros_logErros(ErroArquivos erro, FILE *erroStream)
{
	char *mensagemErro;

	switch(erro) {
		case ARQUIVOS_SEM_ERRO:
			mensagemErro = "Nenhum erro ocorreu.";
			break;
		case ARQUIVOS_ARQ_CORROMPIDO:
			mensagemErro = "Arquivo corrompido encontrado.";
			break;
		case ARQUIVOS_ERRO_LEITURA:
			mensagemErro = "Ocorreu um erro de leitura em"
				       " um arquivo.";
			break;
		case ARQUIVOS_ERRO_ESCRITA:
			mensagemErro = "Ocorreu um erro de escrita em"
				       " um arquivo.";
			break;
		case ARQUIVOS_ERRO_SEEKING:
			mensagemErro = "Ocorreu um erro durante um seeking"
				       " em um arquivo.";
			break;
		case ARQUIVOS_ERRO_ABERTURA:
			mensagemErro = "Ocorreu um erro ao abrir um"
				       " arquivo.";
			break;
		case ARQUIVOS_ERRO_FECHAMENTO:
			mensagemErro = "Ocorreu um erro ao fechar um"
				       " arquivo.";
			break;
		case ARQUIVOS_FALTA_DE_MEMORIA:
			mensagemErro = "Ocorreu um erro por falta de"
				       " memoria.";
			break;
		case ARQUIVOS_REGISTRO_CORROMPIDO:
			mensagemErro = "Um registro foi encontrado em"
				       " um arquivo.";
			break;
	}
	fprintf(erroStream, "%s\n", mensagemErro);
}
