CC=gcc
CFLAGS= -Wall
DEPS = scanner.h parser.h queries.h
OBJ = main.o scanner.o parser.o queries.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm wscarl.txt

.PHONE: clean

clean:
	rm $(OBJ)
	rm wscarl.txt
