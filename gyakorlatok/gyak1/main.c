#include "circle.h"
#include "cuboid.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	// Kör tesztelése
	Circle circle;
	double area;
	
	set_circle_data(&circle, 5, 10, 8);
	area = calc_circle_area(&circle);
	
	printf("=== KÖR ADATAI ===\n");
	printf("Kör sugara: 8\n");
	printf("Kör területe: %lf\n\n", area);
	
	// Téglatest tesztelése
	Cuboid cuboid1;
	double volume, surface;
	int has_square;
	
	// Test 1: 3 x 4 x 5 (nincs négyzet lap)
	set_size(&cuboid1, 3, 4, 5);
	volume = calc_volume(&cuboid1);
	surface = calc_surface(&cuboid1);
	has_square = has_square_face(&cuboid1);
	
	printf("=== TÉGLATEST #1 ===\n");
	printf("Élek: a=3, b=4, c=5\n");
	printf("Térfogat: %lf\n", volume);
	printf("Felszín: %lf\n", surface);
	printf("Van négyzet lap: %s\n\n", has_square ? "IGEN" : "NEM");
	
	// Test 2: 5 x 5 x 10 (van négyzet lap)
	Cuboid cuboid2;
	set_size(&cuboid2, 5, 5, 10);
	volume = calc_volume(&cuboid2);
	surface = calc_surface(&cuboid2);
	has_square = has_square_face(&cuboid2);
	
	printf("=== TÉGLATEST #2 ===\n");
	printf("Élek: a=5, b=5, c=10\n");
	printf("Térfogat: %lf\n", volume);
	printf("Felszín: %lf\n", surface);
	printf("Van négyzet lap: %s\n\n", has_square ? "IGEN" : "NEM");
	
	// Test 3: kocka (3 x 3 x 3 - minden lap négyzet)
	Cuboid cuboid3;
	set_size(&cuboid3, 3, 3, 3);
	volume = calc_volume(&cuboid3);
	surface = calc_surface(&cuboid3);
	has_square = has_square_face(&cuboid3);
	
	printf("=== TÉGLATEST #3 (KOCKA) ===\n");
	printf("Élek: a=3, b=3, c=3\n");
	printf("Térfogat: %lf\n", volume);
	printf("Felszín: %lf\n", surface);
	printf("Van négyzet lap: %s\n", has_square ? "IGEN" : "NEM");
	
	return 0;
}
