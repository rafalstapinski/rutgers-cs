#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *memory;
int registers[9];
int PC, OF, ZF, SF;

int main(int argc, char **argv) {
  if (argc != 2) {

    printf("%s", argv[0]);
    printf("\n");
    printf("%s", argv[1]);
    exit(1);

    fprintf(stderr, "Usage: ./y86emul [-h] <input file>\n");
    exit(1);
  } else if (strcmp(argv[1], "-h") == 0) {

    printf("Usage: ./y86emul [-h] <input file>\n");
    exit(0);
  } else {

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {

      fprintf(stderr, "ERROR: The specified file doesn't exist. ");
      exit(1);
    }

    

  }
}
