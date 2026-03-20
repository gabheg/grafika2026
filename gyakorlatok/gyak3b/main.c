#include "circle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define MAX_CIRCLE_COUNT 50
#define COLOR_COUNT 8
#define PALETTE_WIDTH 40
#define PALETTE_HEIGHT 400
#define PALETTE_X 10
#define PALETTE_Y 10

typedef enum {
	MODE_BY_STEPS,
	MODE_BY_ANGLE,
	MODE_BY_LENGTH
} ApproximationMode;

typedef enum {
	MARK_PLUS,
	MARK_X
} MarkType;

typedef struct {
	Circle circles[MAX_CIRCLE_COUNT];
	int circle_count;
	Color palette[COLOR_COUNT];
	int current_color;
	ApproximationMode approx_mode;
	int step_count;          // For MODE_BY_STEPS
	double angle_step;       // For MODE_BY_ANGLE (in degrees)
	double max_segment_length; // For MODE_BY_LENGTH
	int selected_circle;     // Index of selected circle for dragging (-1 if none)
	MarkType mark_type;
} AppState;

void init_palette(AppState* state)
{
	state->palette[0] = (Color){255, 0, 0};      // Piros
	state->palette[1] = (Color){0, 255, 0};      // Zöld
	state->palette[2] = (Color){0, 0, 255};      // Kék
	state->palette[3] = (Color){255, 255, 0};    // Sárga
	state->palette[4] = (Color){255, 0, 255};    // Magenta
	state->palette[5] = (Color){0, 255, 255};    // Cián
	state->palette[6] = (Color){255, 165, 0};    // Narancs
	state->palette[7] = (Color){255, 255, 255}; // Fehér
	state->current_color = 0;
}

void init_app_state(AppState* state)
{
	state->circle_count = 0;
	state->approx_mode = MODE_BY_STEPS;
	state->step_count = 36;
	state->angle_step = 10.0 * M_PI / 180.0; // 10 degrees
	state->max_segment_length = 5.0;
	state->selected_circle = -1;
	state->mark_type = MARK_PLUS;
	init_palette(state);
}

void draw_palette(SDL_Renderer* renderer, const AppState* state)
{
	for (int i = 0; i < COLOR_COUNT; i++) {
		int y_offset = i * (PALETTE_HEIGHT / COLOR_COUNT);
		SDL_Rect rect = {PALETTE_X, PALETTE_Y + y_offset, PALETTE_WIDTH, PALETTE_HEIGHT / COLOR_COUNT};
		SDL_SetRenderDrawColor(renderer, state->palette[i].r, state->palette[i].g, state->palette[i].b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}

	// Draw selection rectangle around current color
	int selected_y = state->current_color * (PALETTE_HEIGHT / COLOR_COUNT);
	SDL_Rect select_rect = {PALETTE_X - 2, PALETTE_Y + selected_y - 2, 
		PALETTE_WIDTH + 4, PALETTE_HEIGHT / COLOR_COUNT + 4};
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(renderer, &select_rect);
}

void draw_scene(SDL_Renderer* renderer, const AppState* state, int mouse_x, int mouse_y)
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw palette
	draw_palette(renderer, state);

	// Draw circles
	for (int i = 0; i < state->circle_count; i++) {
		switch (state->approx_mode) {
			case MODE_BY_STEPS:
				draw_circle_by_steps(renderer, &state->circles[i], state->step_count);
				break;
			case MODE_BY_ANGLE:
				draw_circle_by_angle(renderer, &state->circles[i], state->angle_step);
				break;
			case MODE_BY_LENGTH:
				draw_circle_by_length(renderer, &state->circles[i], state->max_segment_length);
				break;
		}
	}

	// Draw marks on hovering circles
	for (int i = 0; i < state->circle_count; i++) {
		draw_mark_if_hovering(renderer, &state->circles[i], mouse_x, mouse_y);
	}

	// Present the frame
	SDL_RenderPresent(renderer);
}

int get_selected_color(AppState* state, int mouse_x, int mouse_y)
{
	(void)state;  // Unused parameter
	if (mouse_x < PALETTE_X || mouse_x > PALETTE_X + PALETTE_WIDTH)
		return -1;
	if (mouse_y < PALETTE_Y || mouse_y > PALETTE_Y + PALETTE_HEIGHT)
		return -1;

	int offset = mouse_y - PALETTE_Y;
	int color_index = offset / (PALETTE_HEIGHT / COLOR_COUNT);
	if (color_index >= COLOR_COUNT)
		color_index = COLOR_COUNT - 1;
	return color_index;
}

void add_circle(AppState* state, double x, double y, double radius)
{
	if (state->circle_count >= MAX_CIRCLE_COUNT) {
		printf("Figyelmeztetés: maximális körszám elérve (%d)\n", MAX_CIRCLE_COUNT);
		return;
	}

	init_circle(&state->circles[state->circle_count], x, y, radius, state->palette[state->current_color]);
	state->circle_count++;
	printf("Kör hozzáadva: (%lf, %lf), r=%lf, szín: RGB(%d,%d,%d)\n", 
		x, y, radius,
		state->palette[state->current_color].r,
		state->palette[state->current_color].g,
		state->palette[state->current_color].b);
}

int find_circle_at_point(const AppState* state, int x, int y)
{
	for (int i = state->circle_count - 1; i >= 0; i--) {
		if (is_point_in_circle(&state->circles[i], x, y)) {
			return i;
		}
	}
	return -1;
}

void print_help()
{
	printf("\n=== KÖRÖK RAJZOLÁSA SDL2-VEL ===\n");
	printf("Parancsok:\n");
	printf("  - Bal egérgomb kattintás: új kör hozzáadása\n");
	printf("  - Bal egérgomb húzás: kör mozgatása\n");
	printf("  - Jobb egérgomb: szín kiválasztása a palettából\n");
	printf("  - '1': Lépések szám alapján közelítés (36 lépés)\n");
	printf("  - '2': Szög alapján közelítés (10°)\n");
	printf("  - '3': Szakaszhossz alapján közelítés (5 pixel)\n");
	printf("  - '+': Lépések számának növelése\n");
	printf("  - '-': Lépések számának csökkentése\n");
	printf("  - 'M': Jelölés típusa váltása (+ vagy x)\n");
	printf("  - 'C': Összes kör törlése\n");
	printf("  - 'Q' vagy ESC: Kilépés\n");
	printf("Körök száma: %d/_count_\n\n", MAX_CIRCLE_COUNT);
}

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	SDL_Window* window = SDL_CreateWindow(
		"Körök rajzolása - SDL2",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		printf("Hiba: SDL ablak nem hozható létre\n");
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Hiba: SDL renderer nem hozható létre\n");
		SDL_DestroyWindow(window);
		return -1;
	}

	AppState state;
	init_app_state(&state);
	print_help();

	SDL_Event event;
	int running = 1;
	int prev_mouse_x = -1, prev_mouse_y = -1;

	while (running) {
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_q:
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_1:
							state.approx_mode = MODE_BY_STEPS;
							printf("Módváltás: Lépések szám alapján (36 lépés)\n");
							break;
						case SDLK_2:
							state.approx_mode = MODE_BY_ANGLE;
							printf("Módváltás: Szög alapján (%.1f°)\n", state.angle_step * 180.0 / M_PI);
							break;
						case SDLK_3:
							state.approx_mode = MODE_BY_LENGTH;
							printf("Módváltás: Szakaszhossz alapján (%.1f pixel)\n", state.max_segment_length);
							break;
						case SDLK_PLUS:
						case SDLK_EQUALS:
							state.step_count += 4;
							printf("Lépések száma: %d\n", state.step_count);
							break;
						case SDLK_MINUS:
							if (state.step_count > 4)
								state.step_count -= 4;
							printf("Lépések száma: %d\n", state.step_count);
							break;
						case SDLK_m:
							state.mark_type = (state.mark_type == MARK_PLUS) ? MARK_X : MARK_PLUS;
							printf("Jelölés típusa: %s\n", state.mark_type == MARK_PLUS ? "+" : "x");
							break;
						case SDLK_c:
							state.circle_count = 0;
							printf("Összes kör törölve\n");
							break;
						default:
							break;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						// Check if clicking on a circle
						int circle_idx = find_circle_at_point(&state, mouse_x, mouse_y);
						if (circle_idx >= 0) {
							state.selected_circle = circle_idx;
							printf("Kör kiválasztva a mozgatáshoz: %d\n", circle_idx);
						} else {
							// Add new circle
							double radius = 30.0 + (rand() % 40);
							add_circle(&state, mouse_x, mouse_y, radius);
						}
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						// Select color from palette
						int color_idx = get_selected_color(&state, mouse_x, mouse_y);
						if (color_idx >= 0) {
							state.current_color = color_idx;
							printf("Szín kiválasztva: %d. (RGB: %d,%d,%d)\n", 
								color_idx,
								state.palette[color_idx].r,
								state.palette[color_idx].g,
								state.palette[color_idx].b);
						}
					}
					break;

				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						state.selected_circle = -1;
					}
					break;

				case SDL_MOUSEMOTION:
					// Move selected circle
					if (state.selected_circle >= 0) {
						int dx = event.motion.x - prev_mouse_x;
						int dy = event.motion.y - prev_mouse_y;
						state.circles[state.selected_circle].x += dx;
						state.circles[state.selected_circle].y += dy;
					}
					break;

				default:
					break;
			}
		}

		prev_mouse_x = mouse_x;
		prev_mouse_y = mouse_y;

		draw_scene(renderer, &state, mouse_x, mouse_y);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
