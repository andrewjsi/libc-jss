CC = gcc
# Wall-t visszakapcsolni!
CFLAGS = -Wall -ggdb -I../libjsi

LIBS = -lev -lm -lmysqlclient -lpthread
OBJ=amysql.o ../libjsi/logger.o ../libjsi/misc.o

all: clean compile

compile: $(OBJ) test_amysql.o
	$(CC) $(CFLAGS) -o test_amysql test_amysql.o $(OBJ) $(LIBS)

clean:
	rm -rf $(OBJ) *.o test_amysql


