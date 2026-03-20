#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_MODELS 10

/**
 * Model with position and rotation
 */
typedef struct GameObject
{
    Model model;
    vec3 position;
    vec3 rotation;
    vec3 scale;
} GameObject;

typedef struct Scene
{
    GameObject objects[MAX_MODELS];
    int object_count;
    Material material;
    GLuint texture_id;
    int render_mode;  // 0: single, 1: grid 4x4, 2: rotating pair, etc.
    double time_accumulator;
    vec3 light_position;
    float light_intensity;
    int dynamic_lighting;  // 0: static, 1: moving, 2: pulsing
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

#endif /* SCENE_H */
