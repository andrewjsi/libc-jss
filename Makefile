
CC = gcc
CFLAGS = -Wall -ggdb
LIBS = -lcrypto -lev
OBJ=pipe.o netsocket.o
EXTOBJ=udns-0.1/libudns.a
BIN=pipe

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(EXTOBJ) $(LIBS)

doc:
	doxygen
	@echo ""
	@echo "DOC URL: file://"`pwd`"/html/index.html"
	@echo ""

clean:
	rm -rf $(OBJ) $(BIN) core html/ latex/ man/

# Függőségek
#policy.o: server.h
