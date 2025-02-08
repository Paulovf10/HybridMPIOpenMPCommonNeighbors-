CC = mpicc
CFLAGS = -Iinclude -Wall -Wextra
CFLAGS_PAR = -Iinclude -Wall -Wextra -fopenmp 

SRC_SEQ = src/grafo.c src/util.c src/versao_sequencial.c
SRC_PAR = src/grafo.c src/util.c src/versao_paralela.c

OBJ_SEQ = $(SRC_SEQ:.c=.o)
OBJ_PAR = $(SRC_PAR:.c=.o)

all: sequencial paralela

sequencial: $(OBJ_SEQ)
	$(CC) -o bin/versao_sequencial $(OBJ_SEQ)

paralela: $(OBJ_PAR)
	$(CC) $(CFLAGS_PAR) -o bin/versao_paralela $(OBJ_PAR)  # Compilando com OpenMP

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o bin/versao_sequencial bin/versao_paralela
