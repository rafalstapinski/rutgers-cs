double *multMatrices(double *m1, double *m2, int m1rows, int m1cols, int m2rows, int m2cols) {

    if (m1cols != m2rows) {
        return NULL;
    }

    double *result = (double *) (malloc((m1rows * m2cols * sizeof(double))));

    // linear counter of result array
    int k = 0;

    // for rows of result
    for (int i = 0; i < m1rows; i++) {

        // for columns of result
        for (int j = 0; j < m2cols; j++) {

            // set result element to 0
            result[k] = 0;

            // for combinations of row/column element
            for (int c = 0; c < m1cols; c++) {

                result[k] += (m1[(i * m1cols) + c] * m2[j + (c * m2cols)]);
            }

            k++;
        }
    }

    return result;
}
