The Prime Counter Application consists of two main components:

1. `randomGenerator`: A tool to generate random numbers.
2. `primeCounter`: A tool to count the number of prime numbers from a given set of numbers.

### randomGenerator

The `randomGenerator` takes two arguments:

1. `<seed>`: The seed value for the random number generator.
2. `<num_of_numbers>`: The number of random numbers to generate.

### primeCounter

The `primeCounter` does not take any arguments directly. It reads numbers from standard input (stdin) and counts how many of them are prime numbers.

## Performance

The `primeCounter` is optimized for performance. To measure the performance, you can use the `time` command in the terminal.

Example:
time ./randomGenerator 10 1000000 | ./primeCounter

## How to Run

### Step-by-Step Instructions

1. **Download**: Download the provided package containing `randomGenerator` and `primeCounter`.

2. **Extract**: Extract the package to your desired directory.

3. **Open Terminal**: Navigate to the directory where you extracted the package using your terminal.

4. **Run the Command**:

   ./randomGenerator <seed> <num_of_numbers> | ./primeCounter

   Replace `<seed>` with your desired seed value and `<num_of_numbers>` with the number of random numbers you want to generate.
   Use the `time` option to measure time.

- The performance of the application may vary based on the number of numbers generated and the computational power of your system.
