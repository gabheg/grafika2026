#include "circle.h"

#include <math.h>

void set_circle_data(Circle* circle, double x, double y, double radius)
{
	circle->x = x;
	circle->y = y;
	if (radius > 0.0) {
		circle->radius = radius;
	} else {
		circle->radius = NAN;
	}
	circle->selected = 0;
}

void init_circle(Circle* circle, double x, double y, double radius, Color color)
{
	set_circle_data(circle, x, y, radius);
	circle->color = color;
	circle->selected = 0;
}

double calc_circle_area(const Circle* circle)
{
	double area = circle->radius * circle->radius * M_PI;
	return area;
}

/**
 * Draw line segment with SDL
 */
static void draw_sdl_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void draw_circle_by_steps(SDL_Renderer* renderer, const Circle* circle, int step_count)
{
	if (step_count < 3 || circle->radius <= 0)
		return;

	double angle_step = 2.0 * M_PI / step_count;
	double prev_x = circle->x + circle->radius * cos(0);
	double prev_y = circle->y + circle->radius * sin(0);

	for (int i = 1; i <= step_count; i++) {
		double angle = i * angle_step;
		double curr_x = circle->x + circle->radius * cos(angle);
		double curr_y = circle->y + circle->radius * sin(angle);

		draw_sdl_line(renderer, (int)prev_x, (int)prev_y, (int)curr_x, (int)curr_y, circle->color);

		prev_x = curr_x;
		prev_y = curr_y;
	}
}

void draw_circle_by_angle(SDL_Renderer* renderer, const Circle* circle, double angle_step)
{
	if (angle_step <= 0 || angle_step > 2.0 * M_PI || circle->radius <= 0)
		return;

	double prev_x = circle->x + circle->radius * cos(0);
	double prev_y = circle->y + circle->radius * sin(0);

	for (double angle = angle_step; angle <= 2.0 * M_PI + angle_step / 2.0; angle += angle_step) {
		double curr_x = circle->x + circle->radius * cos(angle);
		double curr_y = circle->y + circle->radius * sin(angle);

		draw_sdl_line(renderer, (int)prev_x, (int)prev_y, (int)curr_x, (int)curr_y, circle->color);

		prev_x = curr_x;
		prev_y = curr_y;
	}
}

void draw_circle_by_length(SDL_Renderer* renderer, const Circle* circle, double max_length)
{
	if (max_length <= 0 || circle->radius <= 0)
		return;

	// arc_length = radius * angle, so angle = arc_length / radius
	double angle_step = max_length / circle->radius;
	if (angle_step > 2.0 * M_PI)
		angle_step = 2.0 * M_PI;

	draw_circle_by_angle(renderer, circle, angle_step);
}

int is_point_in_circle(const Circle* circle, int x, int y)
{
	double dx = x - circle->x;
	double dy = y - circle->y;
	double dist = sqrt(dx * dx + dy * dy);
	return dist <= circle->radius;
}

void draw_mark_if_hovering(SDL_Renderer* renderer, const Circle* circle, int mouse_x, int mouse_y)
{
	if (!is_point_in_circle(circle, mouse_x, mouse_y))
		return;

	// Draw + mark (cross)
	Color mark_color = {255, 255, 255};  // White

	int size = 5;
	// Horizontal line
	draw_sdl_line(renderer, (int)circle->x - size, (int)circle->y, 
		(int)circle->x + size, (int)circle->y, mark_color);
	// Vertical line
	draw_sdl_line(renderer, (int)circle->x, (int)circle->y - size, 
		(int)circle->x, (int)circle->y + size, mark_color);
}
