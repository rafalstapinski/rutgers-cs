#include <stdio.h>
#include <stdlib.h>

double **transpose(double **m, int rows, int cols)
{

    printf("%d, %d\n", rows, cols);

    double **result = (double **) (malloc(cols * sizeof(double *)));

    for (int i = 0; i < cols; i++)
    {
        result[i] = (double *) (malloc(rows * sizeof(double)));
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = m[i][j];
        }
    }

    return result;

}

int main(int argc, char *argv[])
{

    double **m = (double **) (malloc(2 * sizeof(double *)));

    m[0] = (double *) (malloc(3 * sizeof(double)));
    m[1] = (double *) (malloc(3 * sizeof(double)));

    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;

    int rows = 2;
    int cols = 3;

    double **ok = transpose(m, rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%f ", m[i][j]);
        }
    }
    printf("\n");
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            printf("%f ", ok[i][j]);
        }
    }

    // double *ok = transpose(m);

}
