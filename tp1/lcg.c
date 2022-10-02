#include <stdio.h>
#include <stdlib.h>
#define A 1664525
#define C 1013904223
#define M 4294967296

/* GLOBAL VARIABLE */
static int g_curRandLCG = 5;

void lcg(int a, int c, int m, int x, int repeate) {
  int i;

  for (i = 0; i < repeate; ++i) {
    x = (a * x + c)%m;
    printf("%d\n", x);
  }
}

int intRand() {
  g_curRandLCG = (A * g_curRandLCG + C)%M;
  return g_curRandLCG;
}

double floatRand() {
  return ((double) intRand() / 16);
}

int main (void) {
  int i;

  lcg(5, 1, 16, 5, 32);

  // test intRand
  printf("intRand\n");
  for (i = 0; i < 32; ++i) {
    printf("%d\n", intRand());
  }

  // test floatRand
  printf("floatRand\n");
  for (i = 0; i < 32; ++i) {
    printf("%f\n", floatRand());
  }
  return 0;
}
