#include "line.h"
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define COLOR_COUNT 8

typedef enum {
	MODE_DRAW_LINES,
	MODE_DRAW_RECTANGLES
} DrawMode;

int main(int argc, char* argv[])
{
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = init_sdl(WINDOW_WIDTH, WINDOW_HEIGHT, &renderer);
	
	if (!window || !renderer) {
		return -1;
	}

	// Definiera a palettát
	Color palette[COLOR_COUNT] = {
		{255, 0, 0},      // Piros
		{0, 255, 0},      // Zöld
		{0, 0, 255},      // Kék
		{255, 255, 0},    // Sárga
		{255, 0, 255},    // Magenta
		{0, 255, 255},    // Cián
		{255, 165, 0},    // Narancs
		{255, 255, 255}   // Fehér
	};

	// Szakaszok tömb
	Line lines[MAX_LINE_COUNT];
	int line_count = 0;

	// Téglalapok tömb
	Rectangle rectangles[MAX_LINE_COUNT];
	int rect_count = 0;

	// Állapot
	DrawMode mode = MODE_DRAW_LINES;
	int current_color = 0;  // Alapértelmezett piros szín
	int first_point_set = 0;
	int first_x, first_y;

	SDL_Event event;
	int running = 1;

	printf("=== SZAKASZ ÉS TÉGLALAP RAJZOLÓ ===\n");
	printf("Parancsok:\n");
	printf("  - Bal egérgomb kattintás: pont bemérése\n");
	printf("  - Jobb egérgomb dans: paletta szín kiválasztása\n");
	printf("  - 'L' billentyű: Szakasz rajzolás mód\n");
	printf("  - 'R' billentyű: Téglalap rajzolás mód\n");
	printf("  - 'C' billentyű: Törlés\n");
	printf("  - 'Q' vagy ESC: Kilépés\n\n");

	while (running) {
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
						case SDLK_l:
							mode = MODE_DRAW_LINES;
							first_point_set = 0;
							printf("Szakasz rajzolás mód aktiválva\n");
							break;
						case SDLK_r:
							mode = MODE_DRAW_RECTANGLES;
							first_point_set = 0;
							printf("Téglalap rajzolás mód aktiválva\n");
							break;
						case SDLK_c:
							line_count = 0;
							rect_count = 0;
							first_point_set = 0;
							printf("Összes elem törölve\n");
							break;
						default:
							break;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						int mouse_x = event.button.x;
						int mouse_y = event.button.y;
						
						get_mouse_position(&mouse_x, &mouse_y);

						if (!first_point_set) {
							first_x = mouse_x;
							first_y = mouse_y;
							first_point_set = 1;
							
							if (mode == MODE_DRAW_LINES) {
								printf("Első pont rögzítve: (%d, %d)\n", first_x, first_y);
							} else {
								printf("Téglalap bal felső sarka: (%d, %d)\n", first_x, first_y);
							}
						} else {
							if (mode == MODE_DRAW_LINES) {
								if (line_count < MAX_LINE_COUNT) {
									lines[line_count].x1 = first_x;
									lines[line_count].y1 = first_y;
									lines[line_count].x2 = mouse_x;
									lines[line_count].y2 = mouse_y;
									lines[line_count].color = palette[current_color];
									line_count++;
									printf("Szakasz hozzáadva (%d, %d) - (%d, %d), szín index: %d\n",
										   first_x, first_y, mouse_x, mouse_y, current_color);
								} else {
									printf("Maximális szakaszszám elérve!\n");
								}
							} else {
								if (rect_count < MAX_LINE_COUNT) {
									int width = mouse_x - first_x;
									int height = mouse_y - first_y;
									
									if (width < 0) width = -width;
									if (height < 0) height = -height;
									
									rectangles[rect_count].x = (first_x < mouse_x) ? first_x : mouse_x;
									rectangles[rect_count].y = (first_y < mouse_y) ? first_y : mouse_y;
									rectangles[rect_count].width = width;
									rectangles[rect_count].height = height;
									rectangles[rect_count].color = palette[current_color];
									rectangles[rect_count].filled = 1;
									rect_count++;
									printf("Téglalap hozzáadva, szín index: %d\n", current_color);
								} else {
									printf("Maximális téglalapszám elérve!\n");
								}
							}
							first_point_set = 0;
						}
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						int mouse_x = event.button.x;
						int mouse_y = event.button.y;
						int color_idx = get_selected_color(mouse_x, mouse_y, COLOR_COUNT);
						
						if (color_idx != -1) {
							current_color = color_idx;
							printf("Szín kiválasztva: %d (RGB: %d, %d, %d)\n",
								   color_idx, 
								   palette[color_idx].r,
								   palette[color_idx].g,
								   palette[color_idx].b);
						} else {
							printf("A paletták kívül kattintottál! (X: %d, Y: %d)\n", mouse_x, mouse_y);
						}
					}
					break;

				default:
					break;
			}
		}

		// Rajzolás
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderClear(renderer);

		// Paletta megjelenítése
		draw_palette(renderer, palette, COLOR_COUNT);

		// Szakaszok megjelenítése
		for (int i = 0; i < line_count; i++) {
			draw_line(renderer, &lines[i]);
		}

		// Téglalapok megjelenítése
		for (int i = 0; i < rect_count; i++) {
			draw_rectangle(renderer, &rectangles[i]);
		}

		// Információ megjelenítése
		if (first_point_set) {
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderDrawLine(renderer, first_x - 5, first_y, first_x + 5, first_y);
			SDL_RenderDrawLine(renderer, first_x, first_y - 5, first_x, first_y + 5);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);  // ~60 FPS
	}

	cleanup_sdl(window, renderer);
	printf("Program befejeződött\n");
	return 0;
}
