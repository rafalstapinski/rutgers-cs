#include <stdio.h>
#include <stdlib.h>

double **multMatrices(double **m1, double **m2, int m1rows, int m1cols, int m2rows, int m2cols) {

    if (m1cols != m2rows)
    {
        return NULL;
    }

    double temp1[m1cols];
    double temp2[m1cols];

    double **result = (double **) (malloc((m1rows * sizeof(double))));

    for (int i = 0; i < m1rows; i++)
    {
        result[i] = (double *) (malloc(m2cols * sizeof(double)));
    }

    for (int i = 0; i < m1rows; i++)
    {
        // get the row from m1
        for (int m = 0; m < m1cols; m++)
        {
            temp1[m] = m1[i][m];
        }

        for (int j = 0; j < m2cols; j++)
        {
            // get the column from m2
            for (int n = 0; n < m2rows; n++)
            {
                temp2[n] = m2[n][j];
            }

            result[i][j] = 0;

            // set cross product of row/column to result element
            for (int k = 0; k < m1cols; k++) {
                result[i][j] += (temp1[k] * temp2[k]);
            }
        }
    }

    return result;
}

int main(int argc, char *argv[]) {

    int m1rows = 2;
    int m1cols = 3;

    int m2rows = 3;
    int m2cols = 2;

    double **matrix1 = (double **) (malloc((2 * sizeof(double *))));

    for (int i = 0; i < m1rows; i++)
    {
        matrix1[i] = (double *) (malloc(3 * sizeof(double)));
    }

    double **matrix2 = (double **) (malloc((3 * sizeof(double *))));

    for (int i = 0; i < m2rows; i++)
    {
        matrix2[i] = (double *) (malloc(2 * sizeof(double)));
    }

    matrix1[0][0] = 1;
    matrix1[0][1] = 2;
    matrix1[0][2] = 3;
    matrix1[1][0] = 4;
    matrix1[1][1] = 5;
    matrix1[1][2] = 6;

    matrix2[0][0] = 1;
    matrix2[0][1] = 2;
    matrix2[1][0] = 3;
    matrix2[1][1] = 4;
    matrix2[2][0] = 5;
    matrix2[2][1] = 6;

    double **ok = multMatrices(matrix1, matrix2, m1rows, m1cols, m2rows, m2cols);

    for (int i = 0; i < m1rows; i++)
    {
        for (int j = 0; j < m2cols; j++)
        {
            printf("%f ", ok[i][j]);
        }
    }

}
