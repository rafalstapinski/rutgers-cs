#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "nCr.h"

int main(int argc, char *argv[]) {

  if (argv[1][0] == '-' && argv[1][1] == 'h' && strlen(argv[1]) == 2) {

    printf("Usage: formula <positive integer>");

  } else {

    struct timeval start, end;

    gettimeofday(&start, NULL);

    int r = atoi(argv[1]);

    printf("(1 + x)^%d = 1", r);

    int n;
    for (n = 1; n <= r; n++) {
      printf(" + %d*x^%d", nCr(n, r), n);
    }

    gettimeofday(&end, NULL);

    printf("%ld", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

  }

  return 0;

}
