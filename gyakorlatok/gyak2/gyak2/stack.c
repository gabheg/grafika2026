#include "stack.h"

void init_matrix_stack(MatrixStack* stack)
{
    stack->top = -1;
}

int push_matrix(MatrixStack* stack, const float matrix[3][3])
{
    int i;
    int j;

    if (is_stack_full(stack)) {
        return 0;  // Stack overflow
    }

    stack->top++;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            stack->matrices[stack->top][i][j] = matrix[i][j];
        }
    }

    return 1;  // Success
}

int pop_matrix(MatrixStack* stack, float matrix[3][3])
{
    int i;
    int j;

    if (is_stack_empty(stack)) {
        return 0;  // Stack underflow
    }

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = stack->matrices[stack->top][i][j];
        }
    }

    stack->top--;
    return 1;  // Success
}

int is_stack_empty(MatrixStack* stack)
{
    return stack->top == -1;
}

int is_stack_full(MatrixStack* stack)
{
    return stack->top == MAX_STACK_SIZE - 1;
}

int stack_size(MatrixStack* stack)
{
    return stack->top + 1;
}
