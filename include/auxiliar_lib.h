#ifndef _AUX_
#define _AUX_

/* strtok() que e capaz de processar campos nulos */
char *aux_strtok(char *str, char *delimiters);

void aux_strToUpper(char *destino, char *origem);

/*
* Abaixo seguem funções que fazem a escrita do binário em "stdout" (tela) pra
* poder ser comparado no run.codes.
*
* Funciona assim: você faz tudo o que tiver que fazer na funcionalidade no
* arquivo em disco, assim como ensinado nas aulas da disciplina.
* Ao fim da funcionalidade, use a função "binarioNaTela" e a função já cuida
* de tudo para você. É só chamar a função.
*
* Note que ao usar a binarioNaTela, o fclose no arquivo binário já deve ter
* sido feito anteriormente. Você passa o nome do arquivo binário
* ("arquivoTrabX.bin") pra ela e ela vai ler tudo e escrever na tela.
*
* Você pode colocar isso num módulo .h separado, ou incluir as funções no
* próprio código .c: como preferir. VOCÊ NÃO PRECISA ENTENDER ESSAS FUNÇÕES.
* É só usar elas da forma certa depois de acabar a funcionalidade. NÃO
* MODIFIQUE ESSAS FUNÇÕES. Da forma como elas estão aqui, já estão funcionando
* para o propósito delas.
*
* Tá tudo testado e funcionando, mas qualquer dúvida acerca dessas funções,
* falar com o monitor Matheus (mcarvalhor@usp.br). */

void aux_binarioNaTela(char *nomeArquivoBinario);                           

void aux_trim(char *str);                                                   

void aux_scan_quote_string(char *str); 

#endif
