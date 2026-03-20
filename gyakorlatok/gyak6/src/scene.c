#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <GL/gl.h>
#include <math.h>

void init_scene(Scene* scene)
{
    int i;
    
    scene->object_count = 1;
    scene->render_mode = 0;
    scene->time_accumulator = 0;
    scene->light_intensity = 1.0f;
    scene->dynamic_lighting = 0;
    scene->light_position.x = 0.0f;
    scene->light_position.y = 2.0f;
    scene->light_position.z = 10.0f;
    
    // Load default model (cube)
    load_model(&(scene->objects[0].model), "assets/models/cube.obj");
    scene->objects[0].position.x = 0;
    scene->objects[0].position.y = 0;
    scene->objects[0].position.z = 0;
    scene->objects[0].rotation.x = 0;
    scene->objects[0].rotation.y = 0;
    scene->objects[0].rotation.z = 0;
    scene->objects[0].scale.x = 1;
    scene->objects[0].scale.y = 1;
    scene->objects[0].scale.z = 1;
    
    // Initialize rest of objects
    for (i = 1; i < MAX_MODELS; i++) {
        scene->objects[i].model.n_vertices = 0;
        scene->objects[i].model.n_triangles = 0;
        scene->objects[i].position.x = 0;
        scene->objects[i].position.y = 0;
        scene->objects[i].position.z = 0;
        scene->objects[i].rotation.x = 0;
        scene->objects[i].rotation.y = 0;
        scene->objects[i].rotation.z = 0;
        scene->objects[i].scale.x = 1;
        scene->objects[i].scale.y = 1;
        scene->objects[i].scale.z = 1;
    }
    
    scene->texture_id = load_texture("assets/textures/cube.png");
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    // This will be overridden in render_scene with dynamic values
}

void set_lighting_with_scene(const Scene* scene)
{
    float ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float position[] = { scene->light_position.x, scene->light_position.y, scene->light_position.z, 1.0f };

    // Apply light intensity
    diffuse_light[0] *= scene->light_intensity;
    diffuse_light[1] *= scene->light_intensity;
    diffuse_light[2] *= scene->light_intensity;
    specular_light[0] *= scene->light_intensity;
    specular_light[1] *= scene->light_intensity;
    specular_light[2] *= scene->light_intensity;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, double elapsed_time)
{
    // Auto-rotate models in certain modes
    if (scene->render_mode == 0) {
        // Single model: auto-rotate
        scene->objects[0].rotation.y += (float)(elapsed_time * 60.0);
    }
    else if (scene->render_mode == 2 && scene->object_count >= 2) {
        // Two models: auto-rotate second one
        scene->objects[1].rotation.y += (float)(elapsed_time * 60.0);
    }
    
    scene->time_accumulator += elapsed_time;
    
    // Dynamic lighting effects
    if (scene->dynamic_lighting == 1) {
        // Moving light in circle
        float circle_radius = 3.0f;
        scene->light_position.x = (float)(circle_radius * cos(scene->time_accumulator * 2.0));
        scene->light_position.z = (float)(circle_radius * sin(scene->time_accumulator * 2.0) + 5.0f);
    }
    else if (scene->dynamic_lighting == 2) {
        // Pulsing light intensity
        float pulse = 0.5f + 0.5f * (float)sin(scene->time_accumulator * 3.14159f);
        scene->light_intensity = 0.3f + pulse * 0.7f;
    }
}

void render_scene(const Scene* scene)
{
    int i, row, col;
    
    set_material(&(scene->material));
    set_lighting_with_scene(scene);
    draw_origin();
    
    if (scene->render_mode == 0) {
        // Single model
        glPushMatrix();
        glTranslatef(scene->objects[0].position.x, scene->objects[0].position.y, scene->objects[0].position.z);
        glRotatef(scene->objects[0].rotation.x, 1, 0, 0);
        glRotatef(scene->objects[0].rotation.y, 0, 1, 0);
        glRotatef(scene->objects[0].rotation.z, 0, 0, 1);
        glScalef(scene->objects[0].scale.x, scene->objects[0].scale.y, scene->objects[0].scale.z);
        draw_model(&(scene->objects[0].model));
        glPopMatrix();
    }
    else if (scene->render_mode == 1) {
        // 4x4 Grid
        for (row = 0; row < 4; row++) {
            for (col = 0; col < 4; col++) {
                glPushMatrix();
                glTranslatef(-1.5f + col * 1.0f, -1.5f + row * 1.0f, 0);
                draw_model(&(scene->objects[0].model));
                glPopMatrix();
            }
        }
    }
    else if (scene->render_mode == 2 && scene->object_count >= 2) {
        // Two models, one rotating
        glPushMatrix();
        glTranslatef(scene->objects[0].position.x, scene->objects[0].position.y, scene->objects[0].position.z);
        glRotatef(scene->objects[0].rotation.x, 1, 0, 0);
        glRotatef(scene->objects[0].rotation.y, 0, 1, 0);
        glRotatef(scene->objects[0].rotation.z, 0, 0, 1);
        draw_model(&(scene->objects[0].model));
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(scene->objects[1].position.x, scene->objects[1].position.y, scene->objects[1].position.z);
        glRotatef(scene->objects[1].rotation.x, 1, 0, 0);
        glRotatef(scene->objects[1].rotation.y, 0, 1, 0);
        glRotatef(scene->objects[1].rotation.z, 0, 0, 1);
        draw_model(&(scene->objects[1].model));
        glPopMatrix();
    }
    else if (scene->render_mode >= 3) {
        // Multiple objects
        for (i = 0; i < scene->object_count; i++) {
            if (scene->objects[i].model.n_triangles > 0) {
                glPushMatrix();
                glTranslatef(scene->objects[i].position.x, scene->objects[i].position.y, scene->objects[i].position.z);
                glRotatef(scene->objects[i].rotation.x, 1, 0, 0);
                glRotatef(scene->objects[i].rotation.y, 0, 1, 0);
                glRotatef(scene->objects[i].rotation.z, 0, 0, 1);
                glScalef(scene->objects[i].scale.x, scene->objects[i].scale.y, scene->objects[i].scale.z);
                draw_model(&(scene->objects[i].model));
                glPopMatrix();
            }
        }
    }
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
