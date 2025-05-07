#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int SIZE;
    cout << "Enter the size of the square matrices: ";
    cin >> SIZE;

    // Dynamic memory allocation
    int** A = new int*[SIZE];
    int** B = new int*[SIZE];
    int** C = new int*[SIZE];

    for (int i = 0; i < SIZE; ++i) {
        A[i] = new int[SIZE];
        B[i] = new int[SIZE];
        C[i] = new int[SIZE];
    }

    // Input matrix A
    cout << "Enter elements of Matrix A:" << endl;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            cin >> A[i][j];

    // Input matrix B
    cout << "Enter elements of Matrix B:" << endl;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            cin >> B[i][j];

    // Matrix multiplication using OpenMP
    double start_time = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end_time = omp_get_wtime();

    // Output the result
    cout << "Resulting Matrix C (A x B):" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            cout << C[i][j] << " ";
        cout << endl;
    }

    cout << "Time taken: " << (end_time - start_time) << " seconds" << endl;

    // Free dynamically allocated memory
    for (int i = 0; i < SIZE; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
