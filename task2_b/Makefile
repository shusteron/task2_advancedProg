# .PHONY: all
# all: generator primeCounter

# generator:  generator.c
# 	gcc -o randomGenerator generator.c

# primeCounter:	primeCounter.c
# 	gcc -o primeCounter primeCounter.c

# .PHONY: clean
# clean:
# 	-rm randomGenerator primeCounter 2>/dev/null



# ----- Solution 1 ---------
# CC = gcc
# CFLAGS = -Wall -O3 -pthread

# all: randomGenerator primeCounter

# randomGenerator: randomGenerator.c
# 	$(CC) $(CFLAGS) -o randomGenerator randomGenerator.c

# primeCounter: primeCounter.c
# 	$(CC) $(CFLAGS) -o primeCounter primeCounter.c -lm

# clean:
# 	rm -f randomGenerator primeCounter









# for solution 2

.PHONY: all
all: generator primeCounter

generator: generator.c
	gcc -o randomGenerator generator.c

primeCounter: primeCounter.c
	gcc -o primeCounter primeCounter.c -lpthread -lm

.PHONY: clean
clean:
	-rm -f randomGenerator primeCounter