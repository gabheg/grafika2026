#ifndef PONG_H
#define PONG_H

#include "ball.h"
#include "pad.h"

#include <stdbool.h>

/**
 * Pong game elements
 */
typedef struct Pong
{
    int width;
    int height;
    Ball ball;
    Pad left_pad;
    Pad right_pad;
    int left_score;           // Left player score
    int right_score;          // Right player score
} Pong;

/**
 * Initialize the pong table.
 */
void init_pong(Pong* pong, int width, int height);

/**
 * Update the pong game.
 */
void update_pong(Pong* pong, double time);

/**
 * Render the pong game.
 */
void render_pong(Pong* pong);

/**
 * Move the left pad to the given position.
 */
void set_left_pad_position(Pong* pong, float position);

/**
 * Set the speed of the left pad.
 */
void set_left_pad_speed(Pong* pong, float speed);

/**
 * Move the right pad to the given position.
 */
void set_right_pad_position(Pong* pong, float position);

/**
 * Set the speed of the right pad.
 */
void set_right_pad_speed(Pong* pong, float speed);

/**
 * Bounce the ball when necessary.
 */
void bounce_ball(Pong* pong);

/**
 * Move the ball to a new position.
 */
void set_ball_position(Pong* pong, float x, float y);

/**
 * Resize the ball by a delta amount.
 * @param delta the change in radius (positive or negative)
 */
void resize_ball(Pong* pong, float delta);

#endif /* PONG_H */
