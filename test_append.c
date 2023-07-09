#include "list.h"

int main() {
  const int N = 100000;
  const int REPEATS = 1000;

  // Build list of first N squares, repeating REPEATS times
  for (int r = 0; r < REPEATS; ++r) {
    List squares = list_init();
    for (int i = 0; i < N; ++i) {
      list_append(&squares, (double)i * i);
    }
    list_destroy(&squares);
  }
}
