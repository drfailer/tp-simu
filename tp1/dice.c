#include <stdio.h>
#include <stdlib.h>

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
  srand(0);
  rool6(10);
  rool6(100);
  rool6(1000);
  rool10(10);
  rool10(100);
  rool10(1000);
  return 0;
}
