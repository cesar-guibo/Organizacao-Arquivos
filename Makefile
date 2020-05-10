GCC_FLAGS := -Wall

all: ./bin ./bin/main.o ./bin/regsNascimento.o  ./bin/arquivosStd.o ./bin/arquivosCsv.o ./bin/utils.o
	gcc -o programaTrab ./bin/main.o ./bin/regsNascimneto.o ./bin/arquivosStd.o ./bin/arquivosCsv.o ./bin/utils.o $(GCC_FLAGS)
./bin/main.o: ./src/main.c ./bin
	gcc -o ./bin/main.o -c ./src/main.c $(GCC_FLAGS)
./bin/arquivosStd.o: ./src/arquivosStd.c ./bin
	gcc -o ./bin/arquivosStd.o -c ./src/arquivosStd.c $(GCC_FLAGS)
./bin/arquivosCsv.o: ./src/arquivosCsv.c ./bin
	gcc -o ./bin/arquivosCsv.o -c ./src/arquivosCsv.c $(GCC_FLAGS)
./bin/regsNascimento.o: ./src/regsNascimento.c ./bin 
	gcc -o ./bin/regsNascimneto.o -c ./src/regsNascimento.c $(GCC_FLAGS)
./bin/utils.o: ./src/utils.c ./bin
	gcc -o./bin/utils.o -c ./src/utils.c $(GCC_FLAGS)
./bin:
	mkdir ./bin 

zip:
	rm -rf programaTrab bin *.bin *.zip
	zip -r Trabalho1.zip *

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
	rm -rf programa bin *.bin *.zip
