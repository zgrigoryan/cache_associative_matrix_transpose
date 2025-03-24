# Matrix Transposition Benchmark

This project demonstrates two different matrix transposition methods in C++:
- **Naive Transposition:** A simple element-by-element transposition.
- **Blocked Transposition:** A cache-friendly version that processes the matrix in blocks to improve cache utilization.

The program uses the [Kaizen library](https://github.com/heinsaar/kaizen/tree/master) for command-line argument parsing to allow user customization of parameters like the matrix dimension and block sizes.

## Features

- **User Configurable Matrix Size:**  
  Specify the size of the square matrix using the `--size` option.  
  _Example:_ `--size 2048`

- **Custom Block Sizes:**  
  Supply a comma-separated list of block sizes using the `--block-sizes` option.  
  _Example:_ `--block-sizes 8,16,32,64`

- **Benchmarking:**  
  The program measures and outputs the execution time (in milliseconds) for both the naive and the blocked transposition methods, allowing you to observe the performance impact of cache optimization.

- **Result Verification:**  
  After each blocked transposition, the result is verified against the naive transposition to ensure correctness.

- **Formatted Output:**  
  The results are presented in a neatly formatted table.

## Prerequisites

- A C++ compiler supporting C++11 or later.
- The Kaizen library must be available on your system (ensure that the header `kaizen.h` is in your include path).

## Build Instructions

1. Clone the repository or copy the source files.
2. Ensure that `kaizen.h` is in your include path.
3. Compile the program using your favorite C++ compiler. For example:

```bash
g++ -std=c++17 -O2 main.cpp -o matrix_transpose
```
Running the Program
You can run the program with default settings or customize the parameters using command-line options.

Default Run
```bash
./matrix_transpose
```
Custom Matrix Size and Block Sizes
bash
./matrix_transpose --size 2048 --block-sizes 16,32,64,128
Output Example
markdown
Copy
=== Matrix Transposition Benchmark ===
Matrix Dimension: 1024 x 1024

Method                   Block Size     Time (ms)     
=======================================================
Naive Transposition      N/A            150           
Blocked Transposition    8              80    OK     
Blocked Transposition    16             70    OK     
Blocked Transposition    32             65    OK     
Blocked Transposition    64             68    OK     

Benchmark completed.