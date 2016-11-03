#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char *flip(char *num) {

  int i;

  for (i = 0; i < strlen(num); i++) {
    if (num[i] == '0') {
      num[i] = '1';
    } else {
      num[i] = '0';
    }
  }

  return num;
}

int main (int argc, char **argv) {

  if (argc > 3) {
    fprintf(stderr, "ERROR: Only 2 arguments can be passed, <bit sequence> and <int/float> ");
    return 1;
  }

  // whether to convert to float, int, or error
  int doInt;

  if (strcmp(argv[2], "int") != 0 && strcmp(argv[2], "float") != 0) {
    printf("ERROR: Second argument can only be int or float ");
    return 1;
  } else if (strcmp(argv[2], "int") == 0) {
    doInt = 1;
  } else {
    doInt = 0;
  }


  char *bits;
  // global for loop i variable
  int i;

  // get the bits, or throw error
  if (strlen(argv[1]) != 32) {
    printf("ERROR: First argument must be of length 32 ");
    return 1;
  } else {
    for (i = 0; i < 32; i++) {
      if (argv[1][i] != '1' && argv[1][i] != '0') {
        printf("ERROR: First argument must be a sequence of bits 1 and 0 ");
        return 1;
      }
      bits = argv[1];
    }
  }

  // stores whether number is negative of positive
  int negative;

  if (bits[0] == '1') {
    negative = 1;
  }

  /*
  *  This part converts to int
  */
  if (doInt) {

    int outputInt = 0;
    int base = 1;


    /*
    *  If negative, convert integer to positive representation
    */
    if (negative) {

      // temporary storage for complemented bits, will be set to bits later
      char *complement = malloc(33);
      complement[32] = '\0';

      // print - if negative
      printf("-");

      // flip the bits
      bits = flip(bits);

      // add 1 to the bits
      int carry = 1;
      int bit;

      for (i = 31; i >= 0; i--) {
        bit = bits[i] - '0';

        if (bit + carry == 0) {
          complement[i] = '0';
          carry = 0;
        } else if (bit + carry == 1) {
          complement[i] = '1';
          carry = 0;
        } else if (bit + carry == 2) {
          complement[i] = '0';
          carry = 1;
        }
      }
      // set bits to complement for rest of int code below
      bits = complement;
      free(complement);
    }


    /*
    *  Take bits and convert to integer
    */

    // find integer that stores value
    for (i = 31; i > 0; i--) {
      outputInt += base * (bits[i] - '0');
      base *= 2;
    }

    int outputCpy = outputInt;
    int length = 0;

    // find number of digits in int
    while (outputCpy != 0) {
      outputCpy /= 10;
      length++;
    }

    // output array that will hold base 10 digits
    char *output = malloc(length + 1);
    output[length] = '\0';

    // convert int into individual digits
    for (i = 0; i < length; i++) {
      output[length - 1 - i] = outputInt % 10 + '0';
      outputInt /= 10;
    }

    // print output
    printf("%s", output);

    free(output);


  /*
  *  This part converts to float
  */

  } else {

    // cover special cases
    if (strcmp(bits, "01111111111111111111111111111111") == 0) {
      printf("NaN");
      return 0;
    } else if (strcmp(bits, "01111111100000000000000000000000") == 0) {
      printf("pinf");
      return 0;
    } else if (strcmp(bits, "11111111100000000000000000000000") == 0) {
      printf("ninf");
      return 0;
    } else if (strcmp(bits, "10000000000000000000000000000000") == 0) {
      printf("-0.0e0");
      return 0;
    } else if (strcmp(bits, "00000000000000000000000000000000") == 0) {
      printf("0.0e0");
      return 0;
    }
    double number;
    double mantissa = 1;
    int exponent = 0;
    int base = 1;

    // calculate the mantissa
    for (i = 9; i < 32; i++) {
      if (bits[i] == '1') {
        mantissa += (1 / pow(2, i - 8));
      }
    }

    // calculate the exponent
    for (i = 8; i > 0; i--) {
      exponent += (bits[i] - '0') * base;
      base *= 2;
    }

    // normalize the exponent
    exponent -= 127;

    // calculate the numerical value of bitstring
    number = pow(2, exponent) * mantissa;

    // reset exponent to 0, will be used in printint sci notation output
    exponent = 0;

    // calculate new number and exponent
    if (number < 1) {

  		while (number < 1) {
  			number *= 10;
  			exponent--;
  		}
  	} else if (number > 9) {

  		while (number > 9) {
  			number /= 10;
  			exponent++;
  		}
  	}

    printf("%fe%d", number, exponent);

  }

  return 0;

}
