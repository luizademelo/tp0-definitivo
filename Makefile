#---------------------------------------------------------------------
# Arquivo	: Makefile
# Conteúdo	: compilar o programa matop
# Autor		: Wagner Meira Jr. (meira@dcc.ufmg.br)
# Histórico	: 2021-10-18 - arquivo criado
#		: 2021-10-21 - estrutura de diretórios
#               : 2021-11-15 - Versao 1.1 memlog
#---------------------------------------------------------------------
# Opções	: make all - compila tudo
#               : make mem - gera arquivos de registro de acesso
#               : make prof - faz medidas de tempo de desempenho
#               : make gprof - ilustra uso do gprof para depuração
#		: make clean - remove objetos e executável
#---------------------------------------------------------------------

CC = gcc
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/matop.o $(OBJ)/mat.o $(OBJ)/memlog.o
HDRS = $(INC)/mat.h $(INC)/memlog.h $(INC)/msgassert.h
CFLAGS = -pg -Wall -c -I$(INC)
ANALISAMEM = ./analisamem/bin/analisamem


EXE = $(BIN)/matop

all: mem $(EXE) gprof

mem: $(EXE)
	$(EXE) -s -1 m1.txt -2 m2.txt -o soma.out -p /tmp/somalog.out -l
	rm -rf /tmp/somadin 
	mkdir /tmp/somadin
	$(ANALISAMEM) -i /tmp/somalog.out -p /tmp/somadin
	$(EXE) -m -1 m1.txt -2 m2.txt -o mult.out -p /tmp/internolog.out -l
	rm -rf /tmp/internodin 
	mkdir /tmp/internodin
	$(ANALISAMEM) -i /tmp/internolog.out -p /tmp/internodin
	$(EXE) -t -1 m1.txt -o transp.out -p /tmp/transplog.out -l 
	rm -rf /tmp/transpdin 
	mkdir /tmp/transpdin
	$(ANALISAMEM) -i /tmp/transplog.out -p /tmp/transpdin/transpdin
$(EXE): $(OBJS)
	$(CC) -pg -o $(BIN)/matop $(OBJS) $(LIBS)

$(OBJ)/matop.o: $(HDRS) $(SRC)/matop.c
	$(CC) $(CFLAGS) -o $(OBJ)/matop.o $(SRC)/matop.c 

$(OBJ)/mat.o: $(HDRS) $(SRC)/mat.c
	$(CC) $(CFLAGS) -o $(OBJ)/mat.o $(SRC)/mat.c 

$(OBJ)/memlog.o: $(HDRS) $(SRC)/memlog.c
	$(CC) $(CFLAGS) -o $(OBJ)/memlog.o $(SRC)/memlog.c 
	

gprof: $(BIN)/matop
	$(EXE) -s -1 m1.txt -2 m2.txt -o soma.out -p /tmp/somalog.out -l
	gprof $(EXE) gmon.out > /tmp/soma500gprof.txt
	$(EXE) -m -1 m1.txt -2 m2.txt -o mult.out -p /tmp/internolog.out -l
	gprof $(EXE) gmon.out > /tmp/mult500gprof.txt
	$(EXE) -t -1 m1.txt -o transp.out -p /tmp/transplog.out -l 
	gprof $(EXE) gmon.out > /tmp/transp500gprof.txt
	
clean:
	rm -f $(EXE) $(OBJS) gmon.out