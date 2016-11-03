/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {

  char **tokens;    // token strings
  int i, len;       // token index, amount of tokens

};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *ts) {

  //copy ts to make it work with strtok
  char *s = strdup(ts);

  //allocate space for tokenizer struct, and tokens **char
  TokenizerT *tk = malloc(sizeof(TokenizerT));
  tk->tokens = malloc(strlen(ts));

  int i = 0;
  tk->i = 0;
  tk->len = 0;

  //split s into tokens, does not work with consecutive delimiters
  char *token = strtok(s, " \t\v\f\n\r");

  //while there are tokens
  while (token != NULL) {

    //allocate space for token
    tk->tokens[i] = malloc(strlen(token) * sizeof(char *));
    //copy token over
    strcpy(tk->tokens[i], token);
    //increase amt of tokens
    tk->len++;
    //rerun strtok
    token = strtok(NULL, " ");

    i++;
  }

  return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {

  int i;

  //for tokens
  for (i = 0; i < tk->len; i++) {
    //free the token
    free(tk->tokens[i]);
  }
  //free tokens space
  free(tk->tokens);
  //free struct
  free(tk);

}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {

  //if at the end, return 0
  if (tk->i >= tk->len) {
    return 0;
  }

  //copy token to char*
  char *token = tk->tokens[tk->i];
  //increment token index
  tk->i++;
  //return token
  return token;

}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {

  //create tk object
  TokenizerT *tk = TKCreate(argv[1]);

  //while there are tokens
  while (tk->i < tk->len) {

    //get next token and its length
    char* token = TKGetNextToken(tk);
    int tokenlen = strlen(token);

    int i = 0;

    //if starts with 0
    if (token[0] == '0') {

      if (tokenlen == 1) {
        printf("zero %s\n", token);
        continue;
      } else if (tokenlen == 2) {
        printf("mal %s\n", token);
        continue;
      }

      i = 2;

      //if potentially hexadecimal
      if (token[1] == 'x' || token[1] == 'X') {

        int hex = 1;

        //rule out if not hexadecimal
        while (i < tokenlen) {
          if ((token[i] < '0' || token[i] > '9') && (token[i] < 'a' || token[i] > 'f') && (token[i] < 'A' || token[i] > 'F')) {
            hex = 0;
          }
          i++;
        }

        //output result
        if (hex) {
          printf("hexadecimal %s\n", token);
        } else {
          printf("mal %s\n", token);
        }
        continue;

      //if potentially octal
      } else if (token[1] >= '0' && token[1] <= '7') {
        int octal = 1;

        //rule out if not octal
        while (i < tokenlen) {
          if (token[i] < '0' || token[i] > '7') {
            octal = 0;
          }
          i++;
        }
        //output result
        if (octal) {
          printf("octal %s\n", token);
        } else {
          printf("mal %s\n", token);
        }

      //if potentially 0. float
      } else if (token[1] == '.') {

        if (tokenlen == 2) {
          printf("mal %s\n", token);
        }

        i = 2;

        int flt = 1;

        while (i < tokenlen) {

          //rule out if not float
          if (token[i] != 'e' && token[i] != 'E' && token[i] != '-' && token[i] != '+' && (token[i] < '0' || token[i] > '9')) {
            flt = 0;
          }
          i++;
        }
        //output float result
        if (flt) {
          printf("float %s\n", token);
        } else {
          printf("mal %s\n", token);
        }

      }

    //if potentially int or float
    } else if (token[0] >= '1' && token[0] <= '9') {

      if (tokenlen == 1) {
        printf("integer %s\n", token);
      }

      //if second char is valid
      if (token[1] >= '1' && token[1] <= '9') {

        i = 1;

        int integer = 1;
        int flt = 0;

        //rule out if not float or if not integer
        while (i < tokenlen) {

          if ((token[i] <= '1' || token[i] >= '9') && token[i] != '.' && token[i] != 'e' && token[i] != 'E' && token[i] != '-' && token[i] != '+') {
            integer = 0;
          }

          if (token[i] == '.' || token[i] == 'e' || token[i] == 'E' || token[i] == '-' || token[i] == '+' ) {
            flt = 1;
          }

          i++;

        }
        //output integer/float results
        if (integer && !flt) {
          printf("integer %s\n", token);
        } else if (integer && flt) {
          printf("float %s\n", token);
        } else {
          printf("mal %s\n", token);
        }

      //if potentially float
      } else if (token[1] == '.') {

        if (tokenlen == 2) {
          printf("mal %s\n", token);
        }

        i = 2;

        int flt = 1;

        //rule out if not float
        while (i < tokenlen) {

          if (token[i] != 'e' && token[i] != 'E' && token[i] != '-' && token[i] != '+' && (token[i] < '0' || token[i] > '9')) {
            flt = 0;
          }
          i++;

        }
        //print output of float result
        if (flt) {
          printf("float %s\n", token);
        } else {
          printf("mal %s\n", token);
        }

      } else {
        printf("mal %s\n", token);
      }

    } else {
      printf("mal %s\n", token);
    }

  }
  
  //free tk
  TKDestroy(tk);

  return 0;

}
