#include "../include/regsNascimento.h"
#include "../include/arquivos.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <stdbool.h>

#define SUCESSO 0
#define FALHA 1


int operacao1(void)
{
	ArquivoStd *arquivoStd;
	ArquivoCsv *arquivoCsv;
	ErroArquivos erro;
	char inputBuffer[50];


	scanf("%s", inputBuffer);
	erro = arquivosCsv_abrirArquivo(&arquivoCsv, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	scanf("%s", inputBuffer);
	erro = arquivosStd_criarArquivo(&arquivoStd, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	erro = arquivosCsv_formatarArquivoParaStd(arquivoStd, arquivoCsv);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	erro = arquivosCsv_fecharArquivo(&arquivoCsv);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	erro = arquivosStd_fecharArquivo(&arquivoStd);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	utils_binarioNaTela(inputBuffer);

	return SUCESSO;
}

int operacao2(void) {
	ArquivoStd *arquivoStd;
	ErroArquivos erro;
	char inputBuffer[50];

	scanf("%s", inputBuffer);
	erro = arquivosStd_abrirArquivo(&arquivoStd, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	FILE *ptr = fopen("aux.out", "w+");

	erro = arquivoStd_imprimirTodosRegs(arquivoStd, ptr);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	fclose(ptr);

	erro = arquivosStd_fecharArquivo(&arquivoStd);
	if (erro != ARQUIVOS_SEM_ERRO) {
		return FALHA;
	}

	return SUCESSO;
}


int main(int argc, char **argv)
{
	int (*operacoes[2])(void) = {operacao1, operacao2};
	int selec; 

	scanf("%d", &selec);
	if (operacoes[selec - 1]() != SUCESSO)
		printf("Falha no processamento do arquivo.\n");

	return 0;
}
