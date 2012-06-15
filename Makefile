
CC = gcc
CFLAGS = -Wall
LIBS = -lcrypto -lev
OBJ=pipe.o netsocket.o
BIN=pipe

all: clean logger netsocket

netsocket: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(EXTOBJ) $(LIBS)

logger: logger.o logger_test.o misc.o
	$(CC) $(CFLAGS) -o logger logger.o logger_test.o misc.o

doc:
	doxygen
	@echo ""
	@echo "DOC URL: file://"`pwd`"/html/index.html"
	@echo ""

clean:
	rm -rf $(OBJ) $(BIN) logger core *.o html/ latex/ man/

# Függőségek
#policy.o: server.h
