#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "kaizen.h" 

using namespace std;
using namespace std::chrono;

// Default matrix dimension if not provided by the user.
constexpr int DEFAULT_MATRIX_SIZE = 1024;

// Naive transposition: simply iterate over every element.
void transposeNaive(const vector<double>& A, vector<double>& B, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            B[j * n + i] = A[i * n + j];
}

// Blocked (cache-friendly) transposition: process sub-blocks sized 'blockSize'.
// By working on a block that fits in the cache, we improve data locality.
void transposeBlocked(const vector<double>& A, vector<double>& B, int n, int blockSize) {
    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            // Transpose the current block.
            for (int i2 = i; i2 < min(i + blockSize, n); ++i2)
                for (int j2 = j; j2 < min(j + blockSize, n); ++j2)
                    B[j2 * n + i2] = A[i2 * n + j2];
        }
    }
}

// Utility to check if two matrices are identical.
bool checkEqual(const vector<double>& A, const vector<double>& B) {
    return A == B;
}

int main(int argc, char* argv[]) {
    // Parse command-line arguments using Kaizen.
    zen::cmd_args args(argv, argc);
    
    // Allow user to override the matrix dimension.
    int matrixSize = DEFAULT_MATRIX_SIZE;
    if (args.is_present("--size")) {
        matrixSize = std::stoi(args.get_options("--size")[0]);
    }
    
    // Allow user to provide custom block sizes as a comma-separated list.
    vector<int> blockSizes = {8, 16, 32, 64};
    if (args.is_present("--block-sizes")) {
        blockSizes.clear();
        std::string bsStr = args.get_options("--block-sizes")[0];
        std::istringstream ss(bsStr);
        std::string token;
        while(std::getline(ss, token, ',')) {
            blockSizes.push_back(std::stoi(token));
        }
    }

    cout << "\n=== Matrix Transposition Benchmark ===\n";
    cout << "Matrix Dimension: " << matrixSize << " x " << matrixSize << "\n\n";

    // Create matrices A (input), B (naive result), and C (blocked result).
    vector<double> A(matrixSize * matrixSize);
    vector<double> B(matrixSize * matrixSize, 0.0);
    vector<double> C(matrixSize * matrixSize, 0.0);

    // Initialize matrix A with distinct values.
    for (int i = 0; i < matrixSize; ++i)
        for (int j = 0; j < matrixSize; ++j)
            A[i * matrixSize + j] = static_cast<double>(i * matrixSize + j);

    // Table header for results.
    cout << left << setw(25) << "Method"
         << setw(15) << "Block Size"
         << setw(15) << "Time (ms)" << "\n";
    cout << string(55, '=') << "\n";

    // --- Naive Transposition ---
    auto start = high_resolution_clock::now();
    transposeNaive(A, B, matrixSize);
    auto end = high_resolution_clock::now();
    auto durationNaive = duration_cast<milliseconds>(end - start).count();
    cout << left << setw(25) << "Naive Transposition"
         << setw(15) << "N/A"
         << setw(15) << durationNaive << "\n";

    // --- Blocked Transposition ---
    for (int blockSize : blockSizes) {
        // Reset matrix C for each trial.
        fill(C.begin(), C.end(), 0.0);
        
        start = high_resolution_clock::now();
        transposeBlocked(A, C, matrixSize, blockSize);
        end = high_resolution_clock::now();
        auto durationBlocked = duration_cast<milliseconds>(end - start).count();

        // Verify correctness by comparing with the naive transposition result.
        string status = checkEqual(B, C) ? "OK" : "ERROR";

        cout << left << setw(25) << "Blocked Transposition"
             << setw(15) << blockSize
             << setw(15) << durationBlocked;
        cout << "  " << status << "\n";
    }

    cout << "\nBenchmark completed.\n";
    return 0;
}
