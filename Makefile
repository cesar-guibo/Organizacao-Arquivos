GCC_FLAGS := -Wall

all: ./bin ./bin/main.o ./bin/regsNascimento.o  ./bin/arquivosStd.o ./bin/arquivosCsv.o ./bin/auxiliar_lib.o
	gcc -o programaTrab ./bin/main.o ./bin/regsNascimneto.o ./bin/arquivosStd.o ./bin/arquivosCsv.o ./bin/auxiliar_lib.o $(GCC_FLAGS)
./bin/main.o: ./src/main.c ./bin
	gcc -o ./bin/main.o -c ./src/main.c $(GCC_FLAGS)
./bin/arquivosStd.o: ./src/arquivosStd.c ./bin
	gcc -o ./bin/arquivosStd.o -c ./src/arquivosStd.c $(GCC_FLAGS)
./bin/arquivosCsv.o: ./src/arquivosCsv.c ./bin
	gcc -o ./bin/arquivosCsv.o -c ./src/arquivosCsv.c $(GCC_FLAGS)
./bin/regsNascimento.o: ./src/regsNascimento.c ./bin 
	gcc -o ./bin/regsNascimneto.o -c ./src/regsNascimento.c $(GCC_FLAGS)
./bin/auxiliar_lib.o: ./src/auxiliar_lib.c ./bin
	gcc -o./bin/auxiliar_lib.o -c ./src/auxiliar_lib.c $(GCC_FLAGS)
./bin:
	mkdir ./bin 

zip:
	rm -rf programaTrab bin *.bin *.zip
	zip -r Trabalho1.zip *

test:
	valgrind --leak-check=full --track-origins=yes ./programaTrab 

run:
	./programaTrab
clean:
	rm -rf programa bin *.bin *.zip
