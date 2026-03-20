#include "pong.h"

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    pong->left_score = 0;
    pong->right_score = 0;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));
}

void set_left_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->left_pad), position);
}

void set_left_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->left_pad), speed);
}

void set_right_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->right_pad), position);
}

void set_right_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->right_pad), speed);
}

void bounce_ball(Pong* pong)
{
    // Top and bottom walls (always bounce)
    if (pong->ball.y - pong->ball.radius < 0) {
        pong->ball.y = pong->ball.radius;
        pong->ball.speed_y *= -1;
        // Increase rotation speed on top/bottom wall
        pong->ball.rotation_speed += 180.0f;
    }
    if (pong->ball.y + pong->ball.radius > pong->height) {
        pong->ball.y = pong->height - pong->ball.radius;
        pong->ball.speed_y *= -1;
        // Increase rotation speed on top/bottom wall
        pong->ball.rotation_speed += 180.0f;
    }

    // Left pad collision: check both X and Y overlap
    if (pong->ball.x - pong->ball.radius < 50) {
        // Check if ball Y is within pad Y range
        if (pong->ball.y >= pong->left_pad.y && 
            pong->ball.y <= pong->left_pad.y + pong->left_pad.height) {
            pong->ball.x = pong->ball.radius + 50;
            pong->ball.speed_x *= -1;
            
            // Change rotation speed based on where on the paddle the ball hit
            float pad_middle = pong->left_pad.y + pong->left_pad.height / 2;
            if (pong->ball.y < pad_middle) {
                // Hit upper part - increase rotation speed
                pong->ball.rotation_speed += 240.0f;
            } else {
                // Hit lower part - decrease rotation speed
                pong->ball.rotation_speed = pong->ball.rotation_speed > 120.0f ? 
                    pong->ball.rotation_speed - 120.0f : 120.0f;
            }
        } else if (pong->ball.x < 0) {
            // Ball went off left side - right player scores
            pong->right_score++;
            // Reset ball position and speed
            pong->ball.x = pong->width / 2;
            pong->ball.y = pong->height / 2;
            pong->ball.speed_x = -150;
            pong->ball.speed_y = 150;
            pong->ball.rotation_angle = 0.0f;
            pong->ball.rotation_speed = 360.0f;
        }
    }

    // Right pad collision: check both X and Y overlap
    if (pong->ball.x + pong->ball.radius > pong->width - 50) {
        // Check if ball Y is within pad Y range
        if (pong->ball.y >= pong->right_pad.y && 
            pong->ball.y <= pong->right_pad.y + pong->right_pad.height) {
            pong->ball.x = pong->width - pong->ball.radius - 50;
            pong->ball.speed_x *= -1;
            
            // Change rotation speed based on where on the paddle the ball hit
            float pad_middle = pong->right_pad.y + pong->right_pad.height / 2;
            if (pong->ball.y < pad_middle) {
                // Hit upper part - increase rotation speed
                pong->ball.rotation_speed += 240.0f;
            } else {
                // Hit lower part - decrease rotation speed
                pong->ball.rotation_speed = pong->ball.rotation_speed > 120.0f ? 
                    pong->ball.rotation_speed - 120.0f : 120.0f;
            }
        } else if (pong->ball.x > pong->width) {
            // Ball went off right side - left player scores
            pong->left_score++;
            // Reset ball position and speed
            pong->ball.x = pong->width / 2;
            pong->ball.y = pong->height / 2;
            pong->ball.speed_x = 150;
            pong->ball.speed_y = 150;
            pong->ball.rotation_angle = 0.0f;
            pong->ball.rotation_speed = 360.0f;
        }
    }
}

void set_ball_position(Pong* pong, float x, float y)
{
    pong->ball.x = x;
    pong->ball.y = y;
}

void resize_ball(Pong* pong, float delta)
{
    pong->ball.radius += delta;
    // Constrain ball size between 20 and 80 pixels
    if (pong->ball.radius < 20) {
        pong->ball.radius = 20;
    }
    if (pong->ball.radius > 80) {
        pong->ball.radius = 80;
    }
}
