#ifndef LINE_H
#define LINE_H

#include <SDL2/SDL.h>

#define MAX_LINE_COUNT 100
#define PALETTE_WIDTH 40
#define PALETTE_HEIGHT 400
#define PALETTE_X 10
#define PALETTE_Y 10

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
 * Line segment definition with two endpoints and a color
 */
typedef struct Line
{
	int x1, y1;  // First endpoint
	int x2, y2;  // Second endpoint
	Color color;
} Line;

/**
 * Rectangle definition with position, dimensions and color
 */
typedef struct Rectangle
{
	int x, y;      // Top-left corner
	int width, height;
	Color color;
	int filled;    // 0 = outline, 1 = filled
} Rectangle;

/**
 * Initialize SDL and create window/renderer
 */
SDL_Window* init_sdl(int width, int height, SDL_Renderer** renderer);

/**
 * Draw a line segment
 */
void draw_line(SDL_Renderer* renderer, const Line* line);

/**
 * Draw a rectangle
 */
void draw_rectangle(SDL_Renderer* renderer, const Rectangle* rect);

/**
 * Draw the color palette
 */
void draw_palette(SDL_Renderer* renderer, const Color* colors, int color_count);

/**
 * Check if mouse click is on the palette and return color index
 * Returns -1 if no color was selected
 */
int get_selected_color(int mouse_x, int mouse_y, int color_count);

/**
 * Get mouse cursor position
 */
void get_mouse_position(int* x, int* y);

/**
 * Cleanup SDL resources
 */
void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer);

#endif // LINE_H
