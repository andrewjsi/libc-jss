CC = gcc
CFLAGS = -Wall -I.. -ggdb
LDFLAGS = -lpthread

all: test_htclient

test_htclient: test_htclient.o htclient.o
	$(CC) $(CFLAGS) -o test_htclient test_htclient.o htclient.o $(LDFLAGS)

clean:
	rm -f test_htclient *.o
