#include <stdlib.h>
#include <stdio.h>
#include "mystery.h"

int num[200];

int add(int x, int y) {
  return x + y;
}

int dothething(int n) {

  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  }

  if (num[n] == -1) {
    num[n] = add(dothething(n - 2), dothething(n - 1));
  }

  return num[n];

}

int main(int argc, char **argv) {

  int x = atoi(argv[1]);

  int i;
  for (i = 0; i <= 199; i++) {
    num[i] = -1;
  }

  printf("Value:   %d\n", dothething(x));

  return 0;

}
