#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


// Calculates C = A * B where A is MxK, B is KxN, and C is MxN.
void matmul(const size_t M, const size_t K, const size_t N,
            double** A, double** B, double** C)
{
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      C[i][j] = 0.;
      for (size_t k = 0; k < K; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

// Allocate memory for a matrix of size rows x cols
double** create_matrix(const size_t rows, const size_t cols) {
  double** mat = malloc(sizeof(double*) * rows);
  for (size_t row = 0; row < rows; ++row)
    mat[row] = malloc(sizeof(double) * cols);
  return mat;
}

// Clean up memory allocated by `create_matrix`
void destroy_matrix(double*** mat, size_t rows) {
  for (size_t row = 0; row < rows; ++row)
    free((*mat)[row]);
  free(*mat);
  *mat = NULL;
}



int main(void) {
  const size_t REPEATS = 100;

  // Matrix sizes.
  // In a real application these could come from anywhere,
  // so you shouldn't assume that they will always be the same value.
  const size_t M = 500;
  const size_t K = 500;
  const size_t N = 500;

  // Allocate memory for matrices.
  double** A = create_matrix(M, K);
  double** B = create_matrix(K, N);
  double** C = create_matrix(M, N);

  // Set some element values.
  // In a real application this would probably be done by the user,
  // so leave don't worry about optimising this section.
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < K; ++j) {
      A[i][j] = 2.0*((double)(i-j)) / ((double)(i+j+2));
    }
  }
  for (size_t i = 0; i < K; ++i) {
    for (size_t j = 0; j < N; ++j) {
      B[i][j] = sqrt((double)(i+1)/(double)(j+1));
    }
  }

  printf("A[%ld,%ld] = %g\n", M, K, A[M-1][K-1]);
  printf("B[%ld,%ld] = %g\n", K, N, B[K-1][N-1]);

  // Test the matmul function
  for (size_t r = 0; r < REPEATS; ++r) {
    matmul(M, K, N, A, B, C);
  }

  // Print out one of the result elements as a simple check
  printf("C[%ld,%ld] = %g\n", M, N, C[M-1][N-1]);

  // Clean up memory
  destroy_matrix(&A, M);
  destroy_matrix(&B, K);
  destroy_matrix(&C, M);

}
