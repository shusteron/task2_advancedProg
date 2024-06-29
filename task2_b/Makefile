
.PHONY: all
all: generator primesCounter

generator: generator.c
	gcc -o randomGenerator generator.c

primesCounter: primesCounter.c
	gcc -o primesCounter primesCounter.c -lpthread -lm

.PHONY: clean
clean:
	-rm -f randomGenerator primesCounter