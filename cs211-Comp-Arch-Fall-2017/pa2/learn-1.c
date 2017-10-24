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

    printf("%d, %d\n", rows, cols);

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

int main(int argc, const char * argv[])
{
    if(argc < 3) {
        printf("error\n");
        return 0;
    }

    FILE *trainingFile = fopen(argv[1], "r");
    FILE *testingFile = fopen(argv[2], "r");

    // int numTrainingSamples;
    // int numTrainingAttributes;
    //
    // int numTestingSamples;
    // int numTestingAttributes;
    //
    // fscanf(trainingFile, "%d\t%d", &numTrainingAttributes, &numTrainingSamples);
    //
    // double *trainingMatrix = (double *) malloc(numTrainingSamples * numTrainingAttributes * sizeof(double));
    // double *trainingPrices = (double *) malloc(numTrainingSamples * sizeof(double));
    //
    // int i = 0;
    // int y = 0;
    // double readValue = 0;
    //
    // for(i = 0; i < numTrainingSamples; i++) {
    //     for(y = 0; y < numTrainingAttributes; y++) {
    //         fscanf(trainingFile, "%lf,", &readValue);
    //         *(trainingMatrix + (i * numTrainingAttributes) + y) = readValue;
    //     }
    //     fscanf(trainingFile, "%lf,", &readValue);
    //     *(trainingPrices + i) = readValue;
    // }
    // fclose(trainingFile);
    //
    // fscanf(testingFile, "%d\n", &numTestingSamples);
    // numTestingAttributes = numTrainingAttributes;
    //
    // double *testingMatrix = (double *) malloc(numTestingSamples * numTestingAttributes * sizeof(double));
    //
    // for(i = 0; i < numTestingSamples; i++) {
    //     for(y = 0; y < numTestingAttributes; y++) {
    //         fscanf(testingFile, "%lf,", &readValue);
    //         *(testingMatrix + (i * numTestingAttributes) + y) = readValue;
    //     }
    // }
    // fclose(testingFile);
    //
    // for(i = 0; i < numTrainingSamples; i++) {
    //     printf("Training data: %f\tPrice: %f\n", *(trainingMatrix + i), *(trainingPrices + i));
    // }
    //
    // printf("\n");
    // print2DMatrix(numTrainingSamples, numTrainingAttributes, trainingMatrix);
    // printf("\n");
    // print2DMatrix(numTestingSamples, numTestingAttributes, testingMatrix);
    //
    //
    // int counter = 1;
    // trainingMatrix = (double *) malloc(3 * 3 * sizeof(double));
    //
    // for(i = 0; i < 3; i++) {
    //     for(y = 0; y < 3; y++) {
    //         *(trainingMatrix + (i * 3) + y) = counter;
    //         counter += 1;
    //     }
    // }
    //
    // printf("good stuff\n");
    // print2DMatrix(3, 3, trainingMatrix);
    //
    // printf("\nAugmented matrix\n");
    // double augmentedMatrix[3][3 + 3];
    // augmentMatrix(3, 3, trainingMatrix, augmentedMatrix);
    // print2DMatrix(3, 3 + 3, augmentedMatrix);

    int m1rows = 3;
    int m1cols = 3;

    int m2rows = 3;
    int m2cols = 3;

    double **matrix1 = (double **) (malloc((8 * sizeof(double *))));

    for (int i = 0; i < m1rows; i++)
    {
        matrix1[i] = (double *) (malloc(3 * sizeof(double)));
    }

    matrix1[0][0] = 1;
    matrix1[0][1] = 2;
    matrix1[0][2] = 3;
    matrix1[1][0] = 4;
    matrix1[1][1] = 5;
    matrix1[1][2] = 6;
    matrix1[2][0] = 7;
    matrix1[2][1] = 8;
    matrix1[2][2] = 9;

    double **matrix2 = transpose(matrix1, 3, 3);
    double **matrix3 = multMatrices(matrix1, matrix2, 3, 3, 3, 3);

    printMatrix(matrix1, 3, 3);
    printMatrix(matrix2, 3, 3);
    printMatrix(matrix3, 3, 3);

    double **matrix4 = augment(matrix3, 3);

    printMatrix(matrix4, 3, 6);

    free(matrix1);
    free(matrix2);
    free(matrix3);
    free(matrix4);

    return 0;
}
