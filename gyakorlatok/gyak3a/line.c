#include "line.h"
#include <stdio.h>

SDL_Window* init_sdl(int width, int height, SDL_Renderer** renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL inicializálás sikertelen: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Szakasz és Téglalap Rajzoló",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		fprintf(stderr, "Ablak létrehozása sikertelen: %s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}

	*renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!*renderer) {
		fprintf(stderr, "Renderer létrehozása sikertelen: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return NULL;
	}

	return window;
}

void draw_line(SDL_Renderer* renderer, const Line* line)
{
	SDL_SetRenderDrawColor(renderer, line->color.r, line->color.g, line->color.b, 255);
	SDL_RenderDrawLine(renderer, line->x1, line->y1, line->x2, line->y2);
}

void draw_rectangle(SDL_Renderer* renderer, const Rectangle* rect)
{
	SDL_SetRenderDrawColor(renderer, rect->color.r, rect->color.g, rect->color.b, 255);
	SDL_Rect sdl_rect = {rect->x, rect->y, rect->width, rect->height};
	
	if (rect->filled) {
		SDL_RenderFillRect(renderer, &sdl_rect);
	} else {
		SDL_RenderDrawRect(renderer, &sdl_rect);
	}
}

void draw_palette(SDL_Renderer* renderer, const Color* colors, int color_count)
{
	int color_height = PALETTE_HEIGHT / color_count;
	
	for (int i = 0; i < color_count; i++) {
		SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, 255);
		SDL_Rect rect = {PALETTE_X, PALETTE_Y + i * color_height, PALETTE_WIDTH, color_height};
		SDL_RenderFillRect(renderer, &rect);
		
		// Border
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}
}

int get_selected_color(int mouse_x, int mouse_y, int color_count)
{
	// Check if click is within palette area
	if (mouse_x < PALETTE_X || mouse_x > PALETTE_X + PALETTE_WIDTH ||
	    mouse_y < PALETTE_Y || mouse_y > PALETTE_Y + PALETTE_HEIGHT) {
		return -1;
	}
	
	int color_height = PALETTE_HEIGHT / color_count;
	int selected = (mouse_y - PALETTE_Y) / color_height;
	
	if (selected >= 0 && selected < color_count) {
		return selected;
	}
	return -1;
}

void get_mouse_position(int* x, int* y)
{
	SDL_GetMouseState(x, y);
	printf("Egér pozício: x=%d, y=%d\n", *x, *y);
}

void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer)
{
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}
