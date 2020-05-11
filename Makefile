GCC_FLAGS := -Wall

ALL_DEPENDENCIES := ./bin/main.o ./bin/regsNascimento.o ./bin/arquivosStd.o\
		    ./bin/arquivosCsv.o ./bin/utils.o ./bin/arquivosErros.o

all: $(ALL_DEPENDENCIES)
	gcc -o programaTrab $(ALL_DEPENDENCIES) $(GCC_FLAGS)
./bin/main.o: ./src/main.c ./bin
	gcc -o ./bin/main.o -c ./src/main.c $(GCC_FLAGS)
./bin/arquivosStd.o: ./src/arquivosStd.c ./bin
	gcc -o ./bin/arquivosStd.o -c ./src/arquivosStd.c $(GCC_FLAGS)
./bin/arquivosCsv.o: ./src/arquivosCsv.c ./bin
	gcc -o ./bin/arquivosCsv.o -c ./src/arquivosCsv.c $(GCC_FLAGS)
./bin/arquivosErros.o: ./src/arquivosErros.c ./bin
	gcc -o ./bin/arquivosErros.o -c ./src/arquivosErros.c $(GCC_FLAGS)
./bin/regsNascimento.o: ./src/regsNascimento.c ./bin 
	gcc -o ./bin/regsNascimento.o -c ./src/regsNascimento.c $(GCC_FLAGS)
./bin/utils.o: ./src/utils.c ./bin
	gcc -o./bin/utils.o -c ./src/utils.c $(GCC_FLAGS)
./bin:
	mkdir ./bin 

zip:
	rm -rf programaTrab bin *.bin *.zip
	zip -r Trabalho1.zip include/ src/ Makefile README.md

pull:
	git pull origin master

push:
	git add *
	git commit -m "..."
	git push -u origin master

test:
	valgrind --leak-check=full --track-origins=yes ./programaTrab 

run:
	./programaTrab
clean:
	rm -rf programa bin *.bin *.zip erros.txt
