#include "cuboid.h"
#include <math.h>

/**
 * Beállítja a téglatest éleinek hosszát ellenőrzött módon
 * Csak pozitív értékeket fogad el
 */
void set_size(Cuboid* cuboid, double a, double b, double c)
{
	if (a > 0 && b > 0 && c > 0) {
		cuboid->a = a;
		cuboid->b = b;
		cuboid->c = c;
	}
	else {
		// Hibaüzenet vagy hiányos értékek kezelése
		cuboid->a = 0;
		cuboid->b = 0;
		cuboid->c = 0;
	}
}

/**
 * Kiszámítja a téglatest térfogatát
 * V = a * b * c
 */
double calc_volume(const Cuboid* cuboid)
{
	return cuboid->a * cuboid->b * cuboid->c;
}

/**
 * Kiszámítja a téglatest felszínét
 * S = 2 * (ab + bc + ac)
 */
double calc_surface(const Cuboid* cuboid)
{
	return 2 * (cuboid->a * cuboid->b + 
	            cuboid->b * cuboid->c + 
	            cuboid->a * cuboid->c);
}

/**
 * Ellenőrzi, hogy van-e négyzet alakú lapja
 * A téglatestnek 3 típusú lapja lehet:
 * - a x b
 * - b x c
 * - a x c
 * Ha bármelyiknek egyenlők az oldalai, akkor négyzet
 */
int has_square_face(const Cuboid* cuboid)
{
	double epsilon = 1e-9;
	
	// Ellenőrizzük, hogy van-e két egyenlő él
	// Ha van, akkor valamelyik lap négyzet alakú
	if (fabs(cuboid->a - cuboid->b) < epsilon ||
	    fabs(cuboid->b - cuboid->c) < epsilon ||
	    fabs(cuboid->a - cuboid->c) < epsilon) {
		return 1;  // Van négyzet alakú lap
	}
	
	return 0;  // Nincs négyzet alakú lap
}
