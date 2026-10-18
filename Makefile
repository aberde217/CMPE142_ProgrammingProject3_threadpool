# makefile provided in source repo Professor Chen provided us

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: Berde_client.o Berde_threadpool.o
	$(CC) $(CFLAGS) -o example Berde_client.o Berde_threadpool.o $(PTHREADS)

Berde_client.o: Berde_client.c
	$(CC) $(CFLAGS) -c Berde_client.c $(PTHREADS)

Berde_threadpool.o: Berde_threadpool.c Berde_threadpool.h
	$(CC) $(CFLAGS) -c Berde_threadpool.c $(PTHREADS)

clean:
	rm -rf *.o
	rm -rf example'