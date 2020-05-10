#include "../include/regsNascimento.h"
#include "../include/arquivos.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <stdbool.h>

#define SUCESSO 0
#define FALHA 1


int operacao1(FILE *erroStream)
{
	ArquivoStd *arquivoStd;
	ArquivoCsv *arquivoCsv;
	ErroArquivos erro;
	char inputBuffer[50];

	scanf("%s", inputBuffer);
	erro = arquivosCsv_abrirArquivo(&arquivoCsv, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	scanf("%s", inputBuffer);
	erro = arquivosStd_criarArquivo(&arquivoStd, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	erro = arquivosCsv_formatarArquivoParaStd(arquivoStd, arquivoCsv);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	erro = arquivosCsv_fecharArquivo(&arquivoCsv);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	erro = arquivosStd_fecharArquivo(&arquivoStd);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	utils_binarioNaTela(inputBuffer);

	fclose(erroStream);

	return SUCESSO;
}

int operacao2(FILE *erroStream) {
	ArquivoStd *arquivoStd;
	ErroArquivos erro;
	char inputBuffer[50];

	scanf("%s", inputBuffer);
	erro = arquivosStd_abrirArquivo(&arquivoStd, inputBuffer);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	erro = arquivosStd_imprimirTodosRegs(arquivoStd, stdout);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	erro = arquivosStd_fecharArquivo(&arquivoStd);
	if (erro != ARQUIVOS_SEM_ERRO) {
		arquivosErros_logErros(erro, erroStream);
		return FALHA;
	}

	return SUCESSO;
}


int main(int argc, char **argv)
{
	int (*operacoes[2])(FILE *erroStream) = {operacao1, operacao2};
	int selec; 
	FILE *errosLog;

	errosLog = fopen("erros.txt", "w+");

	scanf("%d", &selec);
	if (operacoes[selec - 1](errosLog) != SUCESSO) {
		printf("Falha no processamento do arquivo.\n");
	}

	fclose(errosLog);

	return 0;
}
