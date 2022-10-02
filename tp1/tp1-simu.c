#include <stdio.h>
#include <stdlib.h>

// smart lcg parameters
#define A 1664525
#define C 1013904223
#define M 4294967296

/* GLOBAL VARIABLES: */

static long int g_curRandLCG =
    5; // `long int` is required with the smart parameters
static u_int8_t g_currRandSR = 0b00000110;

/* VON NEUMAN (middle square): */

void middleSquare(int n0, int it) {
  int i;
  int n = n0;

  for (i = 0; i < it; ++i) {
    printf("%d\n", n);
    n = (n * n / 100) % 10000;
  }
}

void testMiddleSquare() {
  printf("\n\n=== Middle Square ===\n\n");
  printf("##### 1234:\n");
  middleSquare(1234, 10);
  printf("##### 4100:\n");
  middleSquare(4100, 10);
  printf("##### 1324:\n");
  middleSquare(1324, 10);
  printf("##### 3141:\n");
  middleSquare(3141, 10);
}

/* COIN TOSSING: */

void coinTossing(int repeate) {
  int r;
  int i;
  int heads = 0, tails = 0;

  // repeating experience
  for (i = 0; i < repeate; ++i) {
    r = rand() % 2;
    if (r) {
      ++heads;
    } else {
      ++tails;
    }
  }

  printf("Conclusion with %d:\n", repeate);
  printf("heads: %f\n", (float)heads / repeate);
  printf("tails: %f\n", (float)tails / repeate);
}

void testCoinTossing() {
  printf("\n\n=== Coin tossing experiment ===\n\n");
  coinTossing(10);
  coinTossing(100);
  coinTossing(1000);
  coinTossing(10000);
}

/* DICE: */

void rool6(int repeate) {
  int i;
  int results[6] = {0};

  for (i = 0; i < repeate; ++i) {
    results[rand() % 6] += 1;
  }
  // Affiche:
  printf("6 - Conclusion for %d:\n", repeate);
  for (i = 0; i < 6; ++i) {
    printf("%d: %f\n", i + 1, (float)results[i] / repeate);
  }
}

void rool10(int repeate) {
  int i;
  int results[10] = {0};

  for (i = 0; i < repeate; ++i) {
    results[rand() % 10] += 1;
  }
  // Affiche:
  printf("10 - Conclusion for %d:\n", repeate);
  for (i = 0; i < 10; ++i) {
    printf("%d: %f\n", i + 1, (float)results[i] / repeate);
  }
  printf("\n");
}

void testDice() {
  printf("\n\n=== dices experiment ===\n\n");
  printf("\n\n--- dices 6 ---\n\n");
  rool6(10);
  rool6(100);
  rool6(1000);
  printf("\n\n--- dices 10 ---\n\n");
  rool10(10);
  rool10(100);
  rool10(1000);
}

/* LCG: */

void lcg(int a, int c, int m, int x, int repeate) {
  int i;

  for (i = 0; i < repeate; ++i) {
    x = (a * x + c) % m;
    printf("%d\n", x);
  }
  printf("\n");
}

long int intRand() {
  g_curRandLCG = (A * g_curRandLCG + C) % M;
  return g_curRandLCG;
}

double floatRand() { return ((double)intRand() / M); }

void testLCG() {
  int i;

  printf("\n\n=== LCG ===\n\n");

  lcg(5, 1, 16, 5, 32);

  // test intRand
  printf("###### intRand:\n");
  for (i = 0; i < 32; ++i) {
    printf("%ld\n", intRand());
  }
  printf("\n");

  // test floatRand
  printf("###### floatRand:\n");
  for (i = 0; i < 32; ++i) {
    printf("%f\n", floatRand());
  }
}

/* SHIFT REGISTER GENERATOR: */

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

void testSRG() {
  int i;

  printf("\n\n=== SRG ===\n\n");

  for (i = 0; i < 16; ++i) {
    displayBinN(srg());
  }
}

/* MAIN: */

int main(void) {
  srand(0);

  // von neuman
  testMiddleSquare();

  // coin tossing
  testCoinTossing();

  // dice
  testDice();

  // lcg
  testLCG();

  // shift register
  testSRG();

  return 0;
}

/** QUESTION:
 * 3. `rand` utilise un générateur congruenciel linéaire qui est à évité dans
 *    les programmes scientifiques.
 * 10. Library qui implémente Mersenne twister:
 *    https://github.com/ESultanik/mtwister
 * 11. Diehard statistical tests:
 *    https://github.com/GINARTeam/Diehard-statistical-test
 */
