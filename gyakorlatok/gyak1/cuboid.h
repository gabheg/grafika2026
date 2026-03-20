#ifndef CUBOID_H
#define CUBOID_H

/**
 * Data structure for a cuboid (rectangular prism)
 * Tartalmazza a téglatest 3 élének hosszát
 */
typedef struct Cuboid
{
	double a;  // első él hossza
	double b;  // második él hossza
	double c;  // harmadik él hossza
} Cuboid;

/**
 * Ellenőrzött mód a téglatest éleinek beállítására
 * Csak pozitív értékeket fogad el
 */
void set_size(Cuboid* cuboid, double a, double b, double c);

/**
 * Kiszámítja a téglatest térfogatát
 */
double calc_volume(const Cuboid* cuboid);

/**
 * Kiszámítja a téglatest felszínét
 */
double calc_surface(const Cuboid* cuboid);

/**
 * Ellenőrzi, hogy van-e négyzet alakú lapja a téglatestnek
 * Visszatér 1-gyel (igaz) ha van, 0-val (hamis) ha nincs
 */
int has_square_face(const Cuboid* cuboid);

#endif // CUBOID_H
