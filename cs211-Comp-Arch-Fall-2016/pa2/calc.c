#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calc.h"

// check if each input is valid num
int is_valid_num(char *num) {

  if (*num == '-') {
    num++;
  }

  char *ptr = NULL;

  switch (*num) {

    case 'b':
      num++;
      for (ptr = num; *ptr != '\0'; ptr++) {
        if (*ptr != '0' && *ptr != '1') {
          return 0;
        }
      }
      break;
    case 'o':
      num++;
      for (ptr = num; *ptr != '\0'; ptr++) {
        if (*ptr < '0' || *ptr > '7') {
          return 0;
        }
      }
      break;
    case 'd':
      num++;
      for (ptr = num; *ptr != '\0'; ptr++) {
        if (*ptr < '0' || *ptr > '9') {
          return 0;
        }
      }
      break;
    case 'x':
      num++;
      for (ptr = num; *ptr != '\0'; ptr++) {
        if ((*ptr < '0' || *ptr > '9') && (*ptr < 'a' || *ptr > 'f') && (*ptr < 'A' || *ptr > 'F')) {
          return 0;
        }
      }
      break;
    default:
      return 0;
  }
  return 1;
}

// flips the bits in bitstring
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

// pads the bitstring with int amount of 0s
char *pad(char *num, int amount) {

  char *result = malloc(strlen(num) + amount);
  result[strlen(num) + amount - 1] = '\0';

  int i;

  for (i = 0; i < strlen(num) + amount; i++) {
    if (i < amount) {
      result[i] = '0';
    } else {
      result[i] = num[i - amount];
    }
  }
  free(num);
  return result;

}

// convert from decimal to bitstring with bitshift
char *dtob(char *num) {

  char *result = malloc(33);
  result[32] = '\0';

  int n, i, b, l;

  n = 0;
  l = 1;

  for (i = strlen(num) - 1; i >= 0; i--) {
    if (num[i] != 'd' && num[i] != '-') {
      n += l * (int) (num[i] - '0');
      l = l * 10;
    }
  }

  for (i = 31; i >= 0; i--) {
    b = n >> i;

    if (b & 1) {
      result[31 - i] = '1';
    } else {
      result[31 - i] = '0';
    }
  }

  return result;
}

// convert octal to bitstring
char *otob(char *num) {

  if (num[0] == '-') {
    char *result = malloc((strlen(num) - 2) * 3 + 1);
    result[(strlen(num) - 2) * 3 + 1] = '\0';
  }

  char *result = malloc((strlen(num) - 1) * 3 + 1);
  result[(strlen(num) - 1) * 3 + 1] = '\0';

  int i;

  for (i = 0; i < strlen(num); i++) {
    if (num[i] != 'd' && num[i] - '0') {
      switch(num[i]) {
        case '0':
          result = strcat(result, "000");
          break;
        case '1':
          result = strcat(result, "001");
          break;
        case '2':
          result = strcat(result, "010");
          break;
        case '3':
          result = strcat(result, "011");
          break;
        case '4':
          result = strcat(result, "100");
          break;
        case '5':
          result = strcat(result, "101");
          break;
        case '6':
          result = strcat(result, "110");
          break;
        case '7':
          result = strcat(result, "111");
          break;
      }
    }
  }
  return result;
}

// convert hex to bitstring
char *xtob(char *num) {

  char *result = NULL;

  if (num[0] == '-') {
    result = malloc((strlen(num) - 2) * 4 + 1);
    result[(strlen(num) - 2) * 4 + 1] = '\0';
  } else {
    result = malloc((strlen(num) - 1) * 4 + 1);
    result[(strlen(num) - 1) * 4 + 1] = '\0';
  }

  int i;

  for (i = 0; i < strlen(num); i++) {
    if (num[i] != 'x' && num[i] - '0') {
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
  }
  return result;
}
// convert binary to bitstring (removes unnecessary chars)
char *btob(char *num) {

  int start = 1;
  int i;
  if (num[0] == '-') {
    start = 2;
  }

  char *result = malloc(strlen(num) - start + 1);
  result[strlen(num) - start] = '\0';

  for (i = start; i < strlen(num); i++) {
    result[i - start] = num[i];
  }

  return result;

}

// convert binary to decimal char array
char *btod(char *num) {

  int i;
  int resultInt = 0;
  int base = 1;
  // calculate actual integer value
  // end before 1 to skip sign bit
  for (i = strlen(num) - 1; i > 0; i--) {
    resultInt += (num[i] - '0') * base;
    base *= 2;
  }

  int resultIntCpy = resultInt;
  int resultLength = 0;

  // amount of digits in number
  while (resultIntCpy != 0) {
    resultIntCpy /= 10;
    resultLength++;
  }

  // create result char array to fit resultInt
  char *result = malloc(resultLength + 1);
  result[resultLength] = '\0';

  // insert digits of resultInt into result
  for (i = 0; i < resultLength; i++) {
    result[resultLength - 1 - i] = resultInt % 10 + '0';
    resultInt /= 10;
  }

  return result;

}

// binary to octal
char *btoo(char *num) {

  // get rid of sign bit, unnecessary
  num[0] = '0';

  // pad to be multiple of 3
  if (strlen(num) != 0) {
    num = pad(num, 3 - (strlen(num) % 3));
  }

  // initialize result char array
  char *result = malloc(1 + strlen(num) / 3);
  result[strlen(result) - 1] = '\0';

  // loop through and assign corresponding octal values
  int i;
  for (i = 0; i < strlen(num); i += 3) {

    if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '0') {
      result[i / 3] = '0';
    } else if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '1') {
      result[i / 3] = '1';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '0') {
      result[i / 3] = '2';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '1') {
      result[i / 3] = '3';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '0') {
      result[i / 3] = '4';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '1') {
      result[i / 3] = '5';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '0') {
      result[i / 3] = '6';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '1') {
      result[i / 3] = '7';
    }

  }
  return result;
}

// binary to octal
char *btox(char *num) {

  // get rid of sign bit, unnecessary
  num[0] = '0';

  // pad to be multiple of 4
  if (strlen(num) != 0) {
    num = pad(num, 4 - (strlen(num) % 4));
  }

  // initialize result char array
  char *result = malloc(1 + strlen(num) / 4);
  result[strlen(result) - 1] = '\0';

  // loop through and assign corresponding hex values
  int i;
  for (i = 0; i < strlen(num); i += 4) {

    if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '0' && num[i + 3] == '0') {
      result[i / 4] = '0';
    } else if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '0' && num[i + 3] == '1') {
      result[i / 4] = '1';
    } else if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '1' && num[i + 3] == '0') {
      result[i / 4] = '2';
    } else if (num[i + 0] == '0' && num[i + 1] == '0' && num[i + 2] == '1' && num[i + 3] == '1') {
      result[i / 4] = '3';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '0' && num[i + 3] == '0') {
      result[i / 4] = '4';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '0' && num[i + 3] == '1') {
      result[i / 4] = '5';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '1' && num[i + 3] == '0') {
      result[i / 4] = '6';
    } else if (num[i + 0] == '0' && num[i + 1] == '1' && num[i + 2] == '1' && num[i + 3] == '1') {
      result[i / 4] = '7';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '0' && num[i + 3] == '0') {
      result[i / 4] = '8';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '0' && num[i + 3] == '1') {
      result[i / 4] = '9';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '1' && num[i + 3] == '0') {
      result[i / 4] = 'a';
    } else if (num[i + 0] == '1' && num[i + 1] == '0' && num[i + 2] == '1' && num[i + 3] == '1') {
      result[i / 4] = 'b';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '0' && num[i + 3] == '0') {
      result[i / 4] = 'c';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '0' && num[i + 3] == '1') {
      result[i / 4] = 'd';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '1' && num[i + 3] == '0') {
      result[i / 4] = 'e';
    } else if (num[i + 0] == '1' && num[i + 1] == '1' && num[i + 2] == '1' && num[i + 3] == '1') {
      result[i / 4] = 'f';
    }
  }

  return result;
}

// add two binary char arrays
char *addBin(char *bin1, char *bin2) {

  // bit1 and bit2 represent bits from each array
  // carry bit is result if 1 needs to be carried over
  int i, carry, bit1, bit2;
  carry = 0;

  // allocate result (pad bit for overflow already taken care of)
  char *result = malloc(strlen(bin1));
  result[strlen(bin1) - 1] = '\0';

  // loop through bits starting at back and add
  for (i = strlen(bin1) - 1; i >= 0; i--) {

    bit1 = bin1[i] - '0';
    bit2 = bin2[i] - '0';

    if (bit1 + bit2 + carry == 0) {
      result[i] = '0';
      carry = 0;
    } else if (bit1 + bit2 + carry == 1) {
      result[i] = '1';
      carry = 0;
    } else if (bit1 + bit2 + carry == 2) {
      result[i] = '0';
      carry = 1;
    } else {
      result[i] = '1';
      carry = 1;
    }

  }
  return result;
}

// calculate twos complement
char *twosComp(char *num) {

  char *result = malloc(strlen(num));
  char *oneBit = malloc(strlen(num));

  int i;

  // create b1 and pad to be same length
  for (i = 0; i < strlen(num); i++) {
    oneBit[i] = '0';
  }
  oneBit[i - 1] = '1';

  // flip the bits then add one
  num = flip(num);
  result = addBin(num, oneBit);

  // twos complement is stored in same var, free original
  // free temporary oneBit
  free(oneBit);
  free(num);

  return result;
}

int main(int argc, char **argv) {

  // set cmdline args
  char *op = argv[1];
  char *num1 = argv[2];
  char *num2 = argv[3];
  char *out = argv[4];

  // check for errors in args
  if ((op[0] != '-' && op[0] != '+') || strlen(op) != 1) {
    fprintf(stderr, "ERROR: operation must be eithe + or -");
    return 1;
  } else if (!is_valid_num(num1)) {
    fprintf(stderr, "ERROR: number1 must be in the form of -?(b|o|d|x)nn...nn where n is appropriate number for specified base");
    return 1;
  } else if (!is_valid_num(num2)) {
    fprintf(stderr, "ERROR: number2 must be in the form of -?(b|o|d|x)nn...nn where n is appropriate number for specified base");
    return 1;
  } else if ((out[0] != 'b' && out[0] != 'o' && out[0] != 'd' && out[0] != 'x') || strlen(out) != 1) {
    fprintf(stderr, "ERROR: output must either be b, o, d, x");
    return 1;
  }

  // initialize two binary char arrays, converted numbers will be put in here
  char *bin1 = NULL;
  char *bin2 = NULL;

  // convert first num and store in bin1
  if (num1[0] == 'b' || num1[1] == 'b') {
    bin1 = btob(num1);
  } else if (num1[0] == 'o' || num1[1] == 'o') {
    bin1 = otob(num1);
  } else if (num1[0] == 'd' || num1[1] == 'd') {
    bin1 = dtob(num1);
  } else if (num1[0] == 'x' || num1[1] == 'x') {
    bin1 = xtob(num1);
  } else {
    fprintf(stderr, "ERROR: number1 must be in the form of -?(b|o|d|x)nn...nn where n is appropriate number for specified base");
    return 1;
  }

  // convert second num and store in bin2
  if (num2[0] == 'b' || num2[1] == 'b') {
    bin2 = btob(num2);
  } else if (num2[0] == 'o' || num2[1] == 'o') {
    bin2 = otob(num2);
  } else if (num2[0] == 'd' || num2[1] == 'd') {
    bin2 = dtob(num2);
  } else if (num2[0] == 'x' || num2[1] == 'x') {
    bin2 = xtob(num2);
  } else {
    fprintf(stderr, "ERROR: number2 must be in the form of -?(b|o|d|x)nn...nn where n is appropriate number for specified base");
    return 1;
  }

  // pad the lengths of the bins
  // the lengths will be equal
  // there is one extra pad for for overflow in addition
  // there is one extra pad for sign bit if twos complement needs to be taken
  if (strlen(bin1) > strlen(bin2)) {
    bin2 = pad(bin2, strlen(bin1) - strlen(bin2) + 2);
    bin1 = pad(bin1, 2);
  } else if (strlen(bin2) > strlen(bin1)) {
    bin1 = pad(bin1, strlen(bin2) - strlen(bin1) + 2);
    bin2 = pad(bin2, 2);
  } else {
    bin1 = pad(bin1, 2);
    bin2 = pad(bin2, 2);
  }

  // init empty addedBin
  char *addedBin = NULL;

  // add numbers
  // pos + pos
  if (*op == '+' && num1[0] != '-' && num2[0] != '-') {
    addedBin = addBin(bin1, bin2);
  // pos + neg
  } else if (*op == '+' && num1[0] != '-' && num2[0] == '-') {
    addedBin = addBin(bin1, twosComp(bin2));
  // neg + pos
  } else if (*op == '+' && num1[0] == '-' && num2[0] != '-') {
    addedBin = addBin(twosComp(bin1), bin2);
  // neg + neg = pos + pos with - output sign
  } else if (*op == '+' && num1[0] == '-' && num2[0] == '-') {
    addedBin = addBin(twosComp(bin1), twosComp(bin2));
  // pos - pos = pos + neg
  } else if (*op == '-' && num1[0] != '-' && num2[0] != '-') {
    addedBin = addBin(bin1, twosComp(bin2));
  // pos - neg = pos + pos
  } else if (*op == '-' && num1[0] != '-' && num2[0] == '-') {
    addedBin = addBin(bin1, bin2);
  // neg - pos = neg + neg
  } else if (*op == '-' && num1[0] == '-' && num2[0] != '-') {
    addedBin = addBin(twosComp(bin1), twosComp(bin2));
  // neg - neg = neg + pos
  } else if (*op == '-' && num1[0] == '-' && num2[0] == '-') {
    addedBin = addBin(twosComp(bin1), bin2);
  }

  // init empty result char array and negative flag
  char *result = NULL;
  int negative = 0;

  // if empty, set negative flag
  if (addedBin[0] == '1') {
    negative = 1;
  }

  // format output to correct output format
  switch (*out) {
    case 'b':
      if (negative) {
        addedBin = flip(addedBin);
        result = addedBin;
      }
      break;
    case 'd':
      result = btod(addedBin);
      break;
    case 'o':
      result = btoo(addedBin);
      break;
    case 'x':
      result = btox(addedBin);
      break;
    default:
      fprintf(stderr, "ERROR: output must either be b, o, d, x");
      return 1;
  }

  // if negative print -
  if (negative) {
    printf("%c", '-');
  }

  // get rid of leading 0s
  while (*result == '0' && *result != '\0') {
    result++;
  }

  // if nothing is left from depadding, output is 0
  if (*result == '\0') {
    result = "0\0";
  }

  // print output
  printf("%c%s", *out, result);

  // free memory
  free(addedBin);
  free(result);
  free(bin1);
  free(bin2);

  return 0;

}
