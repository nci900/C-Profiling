#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Calculates C = A * B where A is MxK, B is KxN, and C is MxN.
//
// We've changed this to explicitly attach the array sizes to A,
// B and C to work with the new variable types.
// This can have compatibility issues with C++ and with some C compilers, so see
// below for an alternative.
//
// We've also added the `restrict` keyword to promise
// to the compiler that A, B, and C do not overlap with each other.
// This helps improve auto-vectorisation.
void matmul(const size_t M, const size_t K, const size_t N,
            double A[restrict M][K],
            double B[restrict K][N],
            double C[restrict M][N])
{
  // Zero out the C matrix
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      C[i][j] = 0.;
    }
  }
  // Using this loop ordering means that we're accessing
  // elements of B sequentially, which is nicer on the cache
  // and helps with auto-vectorisation.
  //
  // There is more that can be done (e.g. check out the BLIS or BLAS libraries),
  // but those optimisations get quite technical and are beyond the scope
  // of this workshop.
  for (size_t i = 0; i < M; ++i) {
    for (size_t k = 0; k < K; ++k) {
      for (size_t j = 0; j < N; ++j) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}


/* Alternative version that doesn't use array syntax.
 * In this case, allocate with:
 * double* A = malloc(sizeof(*A) * M * K);
void matmul2(const size_t M, const size_t K, const size_t N,
            const double* restrict A,
            const double* restrict B,
            double* restrict C)
{
  // Zero out the C matrix
  for (size_t i = 0; i < M*N; ++i) {
    C[i] = 0.;
  }

  for (size_t i = 0; i < M; ++i) {
    for (size_t k = 0; k < K; ++k) {
      for (size_t j = 0; j < N; ++j) {
        C[i*M + j] += A[i*M + k] * B[k*K + j];
      }
    }
  }

}
*/

/* No longer needed with new memory layout

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

*/


int main(void) {
  const size_t REPEATS = 100;

  // Matrix sizes.
  // In a real application these could come from anywhere,
  // so you shouldn't assume that they will always be the same value.
  const size_t M = 500;
  const size_t K = 500;
  const size_t N = 500;

  // Allocate memory for matrices.
  // Using pointer to a 2D array to get contiguous block of memory with convenient indexing.
  // Alternatively, could use just:
  // double* A = malloc(sizeof(*A) * M * K);
  // and address it as A[i*K+j] instead of A[i][j]
  double (*A)[M][K] = malloc(sizeof(*A));
  double (*B)[K][N] = malloc(sizeof(*B));
  double (*C)[M][N] = malloc(sizeof(*C));

  // Set some element values.
  // In a real application this would probably be done by the user,
  // so leave don't worry about optimising this section.
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < K; ++j) {
      (*A)[i][j] = 2.0*((double)(i-j)) / ((double)(i+j+2));
    }
  }
  for (size_t i = 0; i < K; ++i) {
    for (size_t j = 0; j < N; ++j) {
      (*B)[i][j] = sqrt((double)(i+1)/(double)(j+1));
    }
  }

  printf("A[%ld,%ld] = %g\n", M, K, (*A)[M-1][K-1]);
  printf("B[%ld,%ld] = %g\n", K, N, (*B)[K-1][N-1]);

  // Test the matmul function
  for (size_t r = 0; r < REPEATS; ++r) {
    matmul(M, K, N, *A, *B, *C);
  }

  // Print out one of the result elements as a simple check
  printf("C[%ld,%ld] = %g\n", M, N, (*C)[M-1][N-1]);

  // Clean up memory
  free(A);
  free(B);
  free(C);

}
