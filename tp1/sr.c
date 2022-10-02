#include <stdio.h>
#include <stdlib.h>

/* GLOBAL VARIABLES: */

static u_int8_t g_currRandSR = 0b00000110;
static int g_curRandLCG = 5;

/* SR: */

/* Display a u_int8_t number as a 4 bits binary number */
void displayBinN(u_int8_t n) {
  int i;

  for (i = 3; i >= 0; --i) {
    printf("%d", (n >> i) % 2);
  }
  printf("\n");
}

u_int8_t srg() {
  int a = g_currRandSR % 2;
  int b = (g_currRandSR >>= 1) %
          2; // shift `g_currentRandom` and get the value of the last bit

  if (a ^ b) {
    g_currRandSR += 0b00001000;
  }
  return g_currRandSR;
}

/* LCG: */

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


/* VON NEUMAN: */

void middleSquare(int n0, int it) {
  int i;
  int n = n0;

  for (i = 0; i < it; ++i) {
    printf("%d\n", n);
    n = (n*n/100)%10000;
  }
}

/* DICE: */

void rool6(int repeate) {
  int i;
  int results[6] = {0};

  for (i = 0; i < repeate; ++i) {
    results[rand()%6] += 1;
  }
  // Affiche:
  printf("6 - Conclusion for %d:\n", repeate);
  for (i = 0; i < 6; ++i) {
    printf("%d: %f\n", i+1, (float) results[i]/repeate);
  }
}

void rool10(int repeate) {
  int i;
  int results[10] = {0};

  for (i = 0; i < repeate; ++i) {
    results[rand()%10] += 1;
  }
  // Affiche:
  printf("10 - Conclusion for %d:\n", repeate);
  for (i = 0; i < 10; ++i) {
    printf("%d:  %f\n", i+1, (float) results[i]/repeate);
  }
}

int main(void) {
  int i;

  srand(0);

  // lcg
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

  // dices
  rool6(10);
  rool6(100);
  rool6(1000);
  rool10(10);
  rool10(100);
  rool10(1000);

  // sr
  for (i = 0; i < 16; ++i) {
    displayBinN(srg());
  }

  // von newman
  printf("##### 1234:\n");
  middleSquare(1234, 10);
  printf("##### 4100:\n");
  middleSquare(4100, 10);
  printf("##### 1324:\n");
  middleSquare(1324, 10);
  printf("##### 3141:\n");
  middleSquare(3141, 10);
  return 0;
}
