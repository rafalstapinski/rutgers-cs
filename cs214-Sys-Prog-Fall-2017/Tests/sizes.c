#include <stdio.h>

int main(int argc, char *argv[]) {

    int s = sizeof(int);

    union four {
        char a;
        char b;
        int x;
        float y;
        double z;
    };

    printf("%lu", sizeof(union four));
}
