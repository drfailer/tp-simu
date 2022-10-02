#include <stdio.h>

void middleSquare(int n0, int it) {
  int i;
  int n = n0;

  for (i = 0; i < it; ++i) {
    printf("%d\n", n);
    n = (n*n/100)%10000;
  }
}

int main(void) {
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
