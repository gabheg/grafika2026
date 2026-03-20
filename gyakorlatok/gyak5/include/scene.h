#ifndef SCENE_H
#define SCENE_H

#include "camera.h"

typedef struct Scene
{
    double rotation;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

/**
 * Draw a colored triangle with yellow, cyan, and magenta corners.
 */
void draw_colored_triangle();

/**
 * Draw a tessellated sphere.
 */
void draw_tessellated_sphere();

/**
 * Draw a checkerboard pattern on the x-y plane (z=0).
 */
void draw_checkerboard();

/**
 * Draw a cylinder approximation using triangle strips.
 */
void draw_cylinder();

/**
 * Draw a cone approximation using triangle fan.
 */
void draw_cone();

#endif /* SCENE_H */
