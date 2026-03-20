#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Initializes the identity matrix.
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Multiply matrix by a scalar.
 */
void multiply_matrix_by_scalar(float matrix[3][3], float scalar);

/**
 * Multiply two matrices: result = a * b
 */
void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3]);

/**
 * Transform a point using a transformation matrix (homogeneous coordinates).
 * point should be a 3-element array: {x, y, w} typically {x, y, 1}
 */
void transform_point(const float matrix[3][3], const float point[3], float result[3]);

/**
 * Scale transformation: modifies the transformation matrix in place.
 * sx, sy: scale factors in x and y directions
 */
void scale(float matrix[3][3], float sx, float sy);

/**
 * Translation transformation: modifies the transformation matrix in place.
 * tx, ty: translation amounts
 */
void shift(float matrix[3][3], float tx, float ty);

/**
 * Rotation transformation: modifies the transformation matrix in place.
 * angle: rotation angle in radians, counterclockwise around origin
 */
void rotate(float matrix[3][3], float angle);

#endif // MATRIX_H

