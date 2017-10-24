#include <stdio.h>
#include <stdlib.h>


void printMatrix(double **m, int rows, int cols)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f\t", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double **transpose(double **m, int rows, int cols)
{

    double **result = (double **) (malloc(cols * sizeof(double *)));

    for (int i = 0; i < cols; i++) {
        result[i] = (double *) (malloc(rows * sizeof(double)));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = m[i][j];
        }
    }

    return result;
}

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

double **augment(double **m, int size)
{

    double **result = (double **) (malloc(size * sizeof(double *)));

    for (int i = 0; i < size; i++)
    {
        result[i] = (double *) (malloc(size * 2 * sizeof(double)));

        for (int j = size; j < size * 2; j++)
        {
            result[i][j] = 0;
        }

        for (int j = 0; j < size; j++)
        {
            result[i][j] = m[i][j];
            result[i][i + size] = 1;
        }
    }

    return result;

}

double **invert(double **m, int size)
{

    // copy m to new matrix which will be manipulated in memory
    // this is only temporary, just in case
    // ultimately, this can be turned into a void function because dont need
    // to reuse the augmented matrix ever again, but whatever

    double **tmp = (double **) (malloc(size * sizeof(double *)));

    for (int i = 0; i < size; i++) {
        tmp[i] = (double *) (malloc(size * 2 * sizeof(double)));
        for (int j = 0; j < size * 2; j++) {
            tmp[i][j] = m[i][j];
        }
    }

    // make upper triangular matrix, pivots = 1

    double old_pivot;
    double multiplier;

    for (int i = 0; i < size; i++)
    {

        // make pivot = 1
        old_pivot = tmp[i][i];

        for (int j = 0; j < size * 2; j++)
        {
            tmp[i][j] /= old_pivot;
        }

        // go down the rows, remove all under pivot
        if (i < size - 1) {
            for (int k = i + 1; k < size; k++)
            {

                multiplier = tmp[k][i];

                for (int j = 0; j < size * 2; j++)
                {
                    tmp[k][j] -= (tmp[i][j] * multiplier);
                }
            }
        }
    }

    // go up the rows, remove all above pivots
    for (int i = size - 1; i > 0; i--)
    {
        for (int k = i - 1; k >= 0; k--)
        {
            multiplier = tmp[k][i];

            for (int j = 0; j < size * 2; j++)
            {
                tmp[k][j] -= (tmp[i][j] * multiplier);
            }
        }
    }

    double **result = (double **) (malloc(size * sizeof(double *)));


    // copy inverse from tmp to its own size*size matrix and return
    for (int i = 0; i < size; i++)
    {

        result[i] = (double *) (malloc(size * sizeof(double *)));

        for (int j = 0; j < size; j++)
        {
            result[i][j] = tmp[i][size + j];
        }
    }

    free(tmp);

    return result;

}

int main(int argc, const char * argv[])
{
    if(argc < 3) {
        printf("error\n");
        return 0;
    }

    FILE *trainingFile = fopen(argv[1], "r");
    FILE *testingFile = fopen(argv[2], "r");

    int numTrainingSamples;
    int numTrainingAttributes;

    int numTestingSamples;
    int numTestingAttributes;

    fscanf(trainingFile, "%d\t%d", &numTrainingAttributes, &numTrainingSamples);

    numTrainingAttributes++;

    double **trainingMatrix = (double **) (malloc(numTrainingSamples * sizeof(double *)));
    double **trainingPrices = (double **) (malloc(numTrainingSamples * sizeof(double *)));

    double readValue = 0;

    for (int i = 0; i < numTrainingSamples; i++)
    {

        trainingMatrix[i] = (malloc(1 + numTrainingAttributes * sizeof(double)));

        trainingMatrix[i][0] = 1;

        for (int y = 1; y < numTrainingAttributes; y++)
        {

            fscanf(trainingFile, "%lf,", &readValue);
            trainingMatrix[i][y] = readValue;
        }

        fscanf(trainingFile, "%lf,", &readValue);

        trainingPrices[i] = (double *) (malloc(sizeof(double)));
        trainingPrices[i][0] = readValue;

    }

    fclose(trainingFile);


    fscanf(testingFile, "%d\n", &numTestingSamples);
    numTestingAttributes = numTrainingAttributes;

    double **testingMatrix = (double **) (malloc(numTestingSamples * sizeof(double)));

    for (int i = 0; i < numTestingSamples; i++)
    {

        testingMatrix[i] = (double *) (malloc(numTestingAttributes * sizeof(double)));

        testingMatrix[i][0] = 1;

        for (int y = 1; y < numTestingAttributes; y++)
        {

            fscanf(testingFile, "%lf,", &readValue);
            testingMatrix[i][y] = readValue;
        }
    }

    fclose(testingFile);


    double **transposedTrainingMatrix = transpose(trainingMatrix, numTrainingSamples, numTrainingAttributes);
    double **product = multMatrices(transposedTrainingMatrix, trainingMatrix, numTrainingAttributes, numTrainingSamples, numTrainingSamples, numTrainingAttributes);
    double **augmented = augment(product, numTrainingAttributes);
    double **inverted = invert(augmented, numTrainingAttributes);
    double **product2 = multMatrices(inverted, transposedTrainingMatrix, numTrainingAttributes, numTrainingAttributes, numTrainingAttributes, numTrainingSamples);
    double **weights = multMatrices(product2, trainingPrices, numTrainingAttributes, numTrainingSamples, numTrainingSamples, 1);

    // printMatrix(transposedTrainingMatrix, numTrainingAttributes, numTrainingSamples);

    // printf("%d, %d\n", numTestingSamples, numTestingAttributes);
    //
    // printMatrix(trainingMatrix, numTrainingSamples, numTrainingAttributes);
    // printMatrix(testingMatrix, numTestingSamples, numTestingAttributes);
    // printMatrix(product, numTrainingAttributes, numTrainingAttributes);
    // printMatrix(augmented, numTrainingAttributes, numTrainingAttributes * 2);
    // printMatrix(inverted, numTrainingAttributes, numTrainingAttributes);
    // printMatrix(product2, numTrainingAttributes, numTrainingSamples);

    // printf("weights: \n");
    // printMatrix(weights, numTrainingAttributes, 1);
    //
    free(transposedTrainingMatrix);
    free(product);
    free(augmented);
    free(inverted);
    free(product2);

    // printMatrix(testingMatrix, 2, 4);

    float price;

    for (int i = 0; i < numTestingSamples; i++)
    {
        price = 0;
        for (int j = 0; j < numTestingAttributes; j++)
        {
            price += (testingMatrix[i][j] * weights[j][0]);
            // printf("%f * %f + ", testingMatrix[i][j], weights[j][0]);

        }
        printf("%0.0lf\n", price);
        // printf("\n");
    }


    return 0;
}
