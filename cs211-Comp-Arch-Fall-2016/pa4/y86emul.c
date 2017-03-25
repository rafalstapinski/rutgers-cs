#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD

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

    

=======
#include "y86emul.h"

char *memory;
int registers[8];
int PC = -1,
    OF = 0,
    ZF = 0,
    SF = 0,
    AOK = 1,
    HLT = 0,
    ADR = 0,
    INS = 0;

int twoPow(int power) {
  int result = 1;

  int i;
  for (i = 0; i < power; i++) {
    result *= 2;
  }

  return result;

}

char *xtob(char *num) {

  char *result = NULL;

  result = malloc((strlen(num)) * 4 + 1);
  result[(strlen(num)) * 4] = '\0';


  int i;

  for (i = 0; i < strlen(num); i++) {
    switch(num[i]) {
      case '0':
        result = strcat(result, "0000");
        break;
      case '1':
        result = strcat(result, "0001");
        break;
      case '2':
        result = strcat(result, "0010");
        break;
      case '3':
        result = strcat(result, "0011");
        break;
      case '4':
        result = strcat(result, "0100");
        break;
      case '5':
        result = strcat(result, "0101");
        break;
      case '6':
        result = strcat(result, "0110");
        break;
      case '7':
        result = strcat(result, "0111");
        break;
      case '8':
        result = strcat(result, "1000");
        break;
      case '9':
        result = strcat(result, "1001");
        break;
      case 'A':
      case 'a':
        result = strcat(result, "1010");
        break;
      case 'B':
      case 'b':
        result = strcat(result, "1011");
        break;
      case 'C':
      case 'c':
        result = strcat(result, "1100");
        break;
      case 'D':
      case 'd':
        result = strcat(result, "1101");
        break;
      case 'E':
      case 'e':
        result = strcat(result, "1110");
        break;
      case 'F':
      case 'f':
        result = strcat(result, "1111");
        break;
    }
  }
  return result;
}

int btod(char *num) {

  int result = 0;
  int i;

  for (i = 1; i <= strlen(num); i++) {
    if (num[strlen(num) - i] == '1') {
      result += twoPow(i - 1);
    }
  }

  return result;

}

int xtod(char *num) {
  return btod(xtob(num));
}


void exec_nop() {
  PC++;
}

void exec_halt() {
  HLT = 1;
}

void exec_rrmovl() {

  char operands = memory[PC + 1];
  char op_one = (operands & 0xf0) >> 4;
  char op_two = operands & 0x0f;

  if (op_one >= 0 && op_two <= 7 && op_two >= 0 && op_two <= 7) {
    registers[op_two] = registers[op_one];
  } else {
    AOK = 0;
    ADR = 1;
  }

  PC += 2;

}

void exec_irmovl() {

  char operands = memory[PC + 1];
  char op_one = (operands & 0xf0) >> 4;
  char op_two = operands & 0x0f;

  

}


int main(int argc, char **argv) {

  if (argc != 2) {

    fprintf(stderr, "Usage: ./y86emul [-h] <input file> \n");
    exit(1);

  } else if (strcmp(argv[1], "-h") == 0) {

    printf("Usage: ./y86emul [-h] <input file> \n");
    exit(0);

  } else {

    FILE *file = fopen(argv[1], "r");
    char *line = NULL;
    char *token;
    size_t len = 0;
    ssize_t read;

    char *directive;
    int address;
    char *data;

    if (file == NULL) {

      fprintf(stderr, "ERROR: The specified file doesn't exist. \n");
      exit(1);
    }

    while ((read = getline(&line, &len, file)) != -1) {

      token = strtok(line, " \t\v\f\n\r");
      directive = strdup(token);

      if (strcmp(directive, ".size") == 0) {

        if (memory != NULL) {
          fprintf(stderr, "ERROR: Only one size directive can be used. \n");
          exit(1);
        }

        token = strtok(NULL, " \t\v\f\n\r");
        memory = (char *) malloc(xtod(token));

      } else if (strcmp(directive, ".string") == 0
              || strcmp(directive, ".byte")   == 0
              || strcmp(directive, ".bss")    == 0
              || strcmp(directive, ".long")   == 0
              || strcmp(directive, ".text")   == 0) {

         token = strtok(NULL, " \t\v\f\n\r");
         address = xtod(token);

         token = strtok(NULL, " \t\v\f\n\r");
         data = strdup(token);

         if (strcmp(directive, ".byte") == 0) {

           memory[address] = (char) xtod(data);

         } else if (strcmp(directive, ".long") == 0) {

         } else if (strcmp(directive, ".bss") == 0) {

         } else if (strcmp(directive, ".string") == 0) {

         } else if (strcmp(directive, ".text") == 0) {

          if (PC != -1) {
            fprintf(stderr, "ERROR: Only one text directive can be used. \n");
            exit(1);
          }

          PC = address;

          int i;
          char *byte = (char *) malloc(2);

          for (i = 0; i < strlen(data); i += 2) {
            byte[0] = (char) data[i];
            byte[1] = (char) data[i + 1];
            memory[address + (i / 2)] = (char) xtod(byte);
          }

          free(byte);

         }

      } else {

        fprintf(stderr, "ERROR: Invalid directive. \n");
        exit(1);

      }
    }

    // int i;
    // for (i = 0; i < 2048; i++) {
    //   printf("%x", memory[i]);
    // }
    //
    // printf("\n %d", PC);

    fclose(file);
    free(line);

    exec_rrmovl();

    while (AOK) {


      switch(memory[PC]) {

        case 0x00:
          exec_nop();
          break;

        case 0x10:
          exec_halt();
          break;

        case 0x20:
          exec_rrmovl();
          break;

        case 0x30:
          exec_irmovl();
          break;

      }

      if (HLT) {
        return 0;
      }
    }

    if (INS) {

      fprintf(stderr, "ERROR: Invalid instruction. \n");
      return 0;

    } else if (ADR) {

      fprintf(stderr, "ERROR: Invalid address. \n");
      return 0;

    } else {

      fprintf(stderr, "ERROR: Emulator probably written poorly. \n");
      return 1;

    }
>>>>>>> 538118eaad0411cec53490a9e79afa4b0d1b8fcd
  }
}
