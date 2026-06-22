BINARY=sqlite_test

.PHONY: all run clean

all: ${BINARY}

${BINARY}: main.c sqlite3.o
	${CC} -Wall -Wextra -ggdb -o $@ $^ 

sqlite3.o: sqlite3.c
	${CC} -Wall -Wextra -ggdb -o $@ $^ -c

run: ${BINARY}
	./${BINARY}

clean:
	rm *.o ${BINARY}
