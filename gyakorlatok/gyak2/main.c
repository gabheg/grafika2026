#include "matrix.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    printf("=== MATRIX ADDITION EXAMPLE ===\n");
    printf("Matrix A:\n");
    print_matrix(a);
    printf("\nMatrix B:\n");
    print_matrix(b);
    
    add_matrices(a, b, c);
    printf("\nA + B:\n");
    print_matrix(c);

    printf("\n=== IDENTITY MATRIX EXAMPLE ===\n");
    float identity[3][3];
    init_identity_matrix(identity);
    printf("Identity matrix:\n");
    print_matrix(identity);

    printf("\n=== SCALAR MULTIPLICATION EXAMPLE ===\n");
    float scalar_test[3][3];
    init_identity_matrix(scalar_test);
    printf("Identity matrix:\n");
    print_matrix(scalar_test);
    printf("\nMultiplied by 2.5:\n");
    multiply_matrix_by_scalar(scalar_test, 2.5f);
    print_matrix(scalar_test);

    printf("\n=== MATRIX MULTIPLICATION EXAMPLE ===\n");
    float m1[3][3];
    float m2[3][3];
    float m_result[3][3];
    
    init_identity_matrix(m1);
    m1[0][0] = 2.0f;
    m1[1][1] = 3.0f;
    
    init_identity_matrix(m2);
    m2[0][2] = 1.0f;
    m2[1][2] = 2.0f;
    
    printf("Matrix M1:\n");
    print_matrix(m1);
    printf("\nMatrix M2:\n");
    print_matrix(m2);
    
    multiply_matrices(m1, m2, m_result);
    printf("\nM1 * M2:\n");
    print_matrix(m_result);

    printf("\n=== POINT TRANSFORMATION EXAMPLE ===\n");
    float transform[3][3];
    float point[3] = {2.0f, 3.0f, 1.0f};
    float transformed[3];
    
    init_identity_matrix(transform);
    transform[0][2] = 5.0f;  // translate by (5, 0)
    transform[1][2] = 7.0f;  // translate by (0, 7)
    
    printf("Original point: (%.2f, %.2f)\n", point[0], point[1]);
    transform_point(transform, point, transformed);
    printf("After transformation: (%.2f, %.2f)\n", transformed[0], transformed[1]);

    printf("\n=== SCALE TRANSFORMATION EXAMPLE ===\n");
    float scale_matrix[3][3];
    float scale_point[3] = {2.0f, 3.0f, 1.0f};
    float scale_result[3];
    
    init_identity_matrix(scale_matrix);
    printf("Original point: (%.2f, %.2f)\n", scale_point[0], scale_point[1]);
    
    scale(scale_matrix, 2.0f, 3.0f);
    printf("Scale matrix (2x, 3y):\n");
    print_matrix(scale_matrix);
    
    transform_point(scale_matrix, scale_point, scale_result);
    printf("Scaled point: (%.2f, %.2f)\n", scale_result[0], scale_result[1]);

    printf("\n=== SHIFT (TRANSLATION) TRANSFORMATION EXAMPLE ===\n");
    float shift_matrix[3][3];
    float shift_point[3] = {1.0f, 2.0f, 1.0f};
    float shift_result[3];
    
    init_identity_matrix(shift_matrix);
    printf("Original point: (%.2f, %.2f)\n", shift_point[0], shift_point[1]);
    
    shift(shift_matrix, 3.0f, 4.0f);
    printf("Shift matrix (+3, +4):\n");
    print_matrix(shift_matrix);
    
    transform_point(shift_matrix, shift_point, shift_result);
    printf("Shifted point: (%.2f, %.2f)\n", shift_result[0], shift_result[1]);

    printf("\n=== ROTATION TRANSFORMATION EXAMPLE ===\n");
    float rotate_matrix[3][3];
    float rotate_point[3] = {1.0f, 0.0f, 1.0f};
    float rotate_result[3];
    
    init_identity_matrix(rotate_matrix);
    printf("Original point: (%.2f, %.2f)\n", rotate_point[0], rotate_point[1]);
    
    float angle = M_PI / 4.0f;  // 45 degrees
    rotate(rotate_matrix, angle);
    printf("Rotation matrix (45 degrees):\n");
    print_matrix(rotate_matrix);
    
    transform_point(rotate_matrix, rotate_point, rotate_result);
    printf("Rotated point: (%.2f, %.2f)\n", rotate_result[0], rotate_result[1]);

    printf("\n=== COMBINED TRANSFORMATIONS EXAMPLE ===\n");
    float combined[3][3];
    float combined_point[3] = {1.0f, 1.0f, 1.0f};
    float combined_result[3];
    
    init_identity_matrix(combined);
    printf("Original point: (%.2f, %.2f)\n", combined_point[0], combined_point[1]);
    
    printf("\n1. First scale by (2, 2):\n");
    scale(combined, 2.0f, 2.0f);
    print_matrix(combined);
    
    printf("\n2. Then rotate by 45 degrees:\n");
    rotate(combined, M_PI / 4.0f);
    print_matrix(combined);
    
    printf("\n3. Finally shift by (5, 5):\n");
    shift(combined, 5.0f, 5.0f);
    print_matrix(combined);
    
    transform_point(combined, combined_point, combined_result);
    printf("\nFinal transformed point: (%.2f, %.2f)\n", combined_result[0], combined_result[1]);

	return 0;
}

