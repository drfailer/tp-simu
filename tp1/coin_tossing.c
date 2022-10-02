#include <stdio.h>
#include <stdlib.h>
#define SEED 100

void experimentCoin(int repeate) {
  int r;
  int i;
  int heads = 0, tails = 0;

  // repeating experience
  for (i = 0; i < repeate; ++i) {
    r = rand()%2;
    if (r) {
      ++heads;
    } else {
      ++tails;
    }
  }

  printf("Conclusion with: %d:\n", repeate);
  printf("heads: %f:\n", (float) heads/repeate);
  printf("tails: %f:\n", (float) tails/repeate);
}


int main (void) {
  srand(0);
  experimentCoin(10);
  experimentCoin(100);
  experimentCoin(1000);
  experimentCoin(10000);
  return 0;
}
