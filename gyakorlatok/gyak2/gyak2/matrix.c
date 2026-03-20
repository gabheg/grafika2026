#include "matrix.h"

#include <stdio.h>
#include <math.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void multiply_matrix_by_scalar(float matrix[3][3], float scalar)
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] *= scalar;
        }
    }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3])
{
    int i;
    int j;
    int k;
    float temp[3][3];

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            temp[i][j] = 0.0f;
            for (k = 0; k < 3; ++k) {
                temp[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = temp[i][j];
        }
    }
}

void transform_point(const float matrix[3][3], const float point[3], float result[3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        result[i] = 0.0f;
        for (j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

void scale(float matrix[3][3], float sx, float sy)
{
    float scale_matrix[3][3];
    float temp[3][3];

    init_identity_matrix(scale_matrix);
    scale_matrix[0][0] = sx;
    scale_matrix[1][1] = sy;

    multiply_matrices(scale_matrix, matrix, temp);

    int i;
    int j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = temp[i][j];
        }
    }
}

void shift(float matrix[3][3], float tx, float ty)
{
    float shift_matrix[3][3];
    float temp[3][3];

    init_identity_matrix(shift_matrix);
    shift_matrix[0][2] = tx;
    shift_matrix[1][2] = ty;

    multiply_matrices(shift_matrix, matrix, temp);

    int i;
    int j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = temp[i][j];
        }
    }
}

void rotate(float matrix[3][3], float angle)
{
    float rotate_matrix[3][3];
    float temp[3][3];
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);

    init_identity_matrix(rotate_matrix);
    rotate_matrix[0][0] = cos_a;
    rotate_matrix[0][1] = -sin_a;
    rotate_matrix[1][0] = sin_a;
    rotate_matrix[1][1] = cos_a;

    multiply_matrices(rotate_matrix, matrix, temp);

    int i;
    int j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = temp[i][j];
        }
    }
}

