#include "ball.h"

#include <GL/gl.h>

#include <math.h>

void init_ball(Ball* ball, float x, float y)
{
    ball->x = x;
    ball->y = y;
    ball->radius = 50;
    ball->speed_x = 150;
    ball->speed_y = 150;
    ball->rotation_angle = 0.0f;
    ball->rotation_speed = 360.0f;    // 360 degrees per second = 1 full rotation per second
}

void update_ball(Ball* ball, double time)
{
    ball->x += ball->speed_x * time;
    ball->y += ball->speed_y * time;
    ball->rotation_angle += ball->rotation_speed * time;
    if (ball->rotation_angle >= 360.0f) {
        ball->rotation_angle -= 360.0f;
    }
}

void render_ball(Ball* ball)
{
    double angle;
    double x;
    double y;
    double rotation_rad;

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0);
    
    // Draw the ball circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.9, 0.8);
    glVertex2f(0, 0);
    angle = 0;
    while (angle < 2.0 * M_PI + 1) {
        x = cos(angle) * ball->radius;
        y = sin(angle) * ball->radius;
        glVertex2f(x, y);
        angle += 0.8;
    }
    glEnd();
    
    // Draw rotation indicator (white line)
    glColor3f(1.0, 1.0, 1.0);
    rotation_rad = (ball->rotation_angle * M_PI) / 180.0;
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(cos(rotation_rad) * (ball->radius - 10), sin(rotation_rad) * (ball->radius - 10));
    glEnd();
    
    glPopMatrix();
}
