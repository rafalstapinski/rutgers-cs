#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "nCr.h"

int main(int argc, char **argv[]) {

  if (argv[1][0] == '-' && argv[1][1] == 'h' && strlen(argv[1]) == 2) {

    printf("Usage: formula <positive integer>\n");

  } else {

    struct timeval start, end;

    gettimeofday(&start, NULL);

    int n = atoi(argv[1]);

    if (n < 0) {
      printf("Usage: formula <positive integer>\n");
      return 1;
    } else if (Factorial(n) == 0) {
      printf("Overflow error.\n");
      return 1;
    }

    printf("(1 + x)^%d = 1", n);

    int r;

    for (r = 1; r <= n; r++) {
      printf(" + %d*x^%d", nCr(n, r), r);
    }

    gettimeofday(&end, NULL);

    printf("\nTime Required = %ld microsecond\n", (1000000 * end.tv_sec + end.tv_usec) - (1000000 * start.tv_sec + start.tv_usec));

  }

  return 0;
}
