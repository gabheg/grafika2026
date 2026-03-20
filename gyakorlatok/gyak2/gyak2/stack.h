#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 100

/**
 * Matrix stack structure for storing transformation matrices.
 * Used for saving and restoring transformation states.
 */
typedef struct {
    float matrices[MAX_STACK_SIZE][3][3];
    int top;
} MatrixStack;

/**
 * Initialize the matrix stack (set top to -1).
 */
void init_matrix_stack(MatrixStack* stack);

/**
 * Push a matrix onto the stack.
 * Returns 1 on success, 0 on stack overflow.
 */
int push_matrix(MatrixStack* stack, const float matrix[3][3]);

/**
 * Pop a matrix from the stack.
 * Returns 1 on success, 0 on stack underflow.
 */
int pop_matrix(MatrixStack* stack, float matrix[3][3]);

/**
 * Check if the stack is empty.
 * Returns 1 if empty, 0 otherwise.
 */
int is_stack_empty(MatrixStack* stack);

/**
 * Check if the stack is full.
 * Returns 1 if full, 0 otherwise.
 */
int is_stack_full(MatrixStack* stack);

/**
 * Get current stack size.
 */
int stack_size(MatrixStack* stack);

#endif // STACK_H
