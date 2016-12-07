OBJS= list.o array.o graph.o graph.o partition.o weisfeiler-lehman.o main.o
CC= gcc
CCFLAGS= -std=c99 -O2
BIN= main

all: $(OBJS)
	$(CC) $(CCFLAGS) -o $(BIN) $^

%.o: %.c
	$(CC) -c $(CCFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o $(BIN)

