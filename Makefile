
CC = gcc
CFLAGS = -Wall
LIBS = -lcrypto -lev
OBJ=pipe.o netsocket.o
EXTOBJ=udns-0.1/libudns.a
BIN=pipe

all: clean logger

netsocket: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(EXTOBJ) $(LIBS)

logger: logger.o logger_test.o
	$(CC) $(CFLAGS) -o logger logger.o logger_test.o

doc:
	doxygen
	@echo ""
	@echo "DOC URL: file://"`pwd`"/html/index.html"
	@echo ""

clean:
	rm -rf $(OBJ) $(BIN) logger logger.o logger_test.o core html/ latex/ man/

# Függőségek
#policy.o: server.h
