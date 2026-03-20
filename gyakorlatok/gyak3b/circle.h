#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>

/**
 * Color definition using RGB components
 */
typedef struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
} Color;

/**
 * Data of a circle object in Descartes coordinate system
 */
typedef struct Circle
{
	double x;
	double y;
	double radius;
	Color color;
	int selected;  // Whether circle is selected for dragging
} Circle;

/**
 * Set the data of the circle
 */
void set_circle_data(Circle* circle, double x, double y, double radius);

/**
 * Initialize circle with color
 */
void init_circle(Circle* circle, double x, double y, double radius, Color color);

/**
 * Calculate the area of the circle.
 */
double calc_circle_area(const Circle* circle);

/**
 * Draw circle approximated by line segments using step count
 */
void draw_circle_by_steps(SDL_Renderer* renderer, const Circle* circle, int step_count);

/**
 * Draw circle approximated by line segments using angle step
 */
void draw_circle_by_angle(SDL_Renderer* renderer, const Circle* circle, double angle_step);

/**
 * Draw circle approximated by line segments using max segment length
 */
void draw_circle_by_length(SDL_Renderer* renderer, const Circle* circle, double max_length);

/**
 * Draw + or x mark at the circle center if mouse is hovering
 */
void draw_mark_if_hovering(SDL_Renderer* renderer, const Circle* circle, int mouse_x, int mouse_y);

/**
 * Check if point is inside circle (for hovering detection)
 */
int is_point_in_circle(const Circle* circle, int x, int y);

#endif // CIRCLE_H
