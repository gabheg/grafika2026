#include "app.h"

#include <SDL2/SDL_image.h>
#include <obj/load.h>
#include <stdio.h>

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Cube!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 10
    );
}

void handle_app_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            // Scene mode selection
            case SDL_SCANCODE_1:
                app->scene.render_mode = 0;
                printf("[MODE] Single model\n");
                break;
            case SDL_SCANCODE_2:
                app->scene.render_mode = 1;
                printf("[MODE] 4x4 grid\n");
                break;
            case SDL_SCANCODE_3:
                if (app->scene.object_count < 2) {
                    load_model(&(app->scene.objects[1].model), "assets/models/cat.obj");
                    app->scene.objects[1].position.x = 2.0f;
                    app->scene.object_count = 2;
                }
                app->scene.render_mode = 2;
                printf("[MODE] Two models (rotating)\n");
                break;
            case SDL_SCANCODE_4:
                if (app->scene.object_count < 2) {
                    load_model(&(app->scene.objects[1].model), "assets/models/cat.obj");
                    app->scene.objects[1].position.x = 2.0f;
                    app->scene.object_count = 2;
                }
                app->scene.render_mode = 3;
                printf("[MODE] Two models (interactive)\n");
                break;
            // Object 0 rotation
            case SDL_SCANCODE_Q:
                app->scene.objects[0].rotation.y -= 5;
                break;
            case SDL_SCANCODE_E:
                app->scene.objects[0].rotation.y += 5;
                break;
            // Object 0 position (also with mouse)
            case SDL_SCANCODE_LEFT:
                app->scene.objects[0].position.x -= 0.1f;
                break;
            case SDL_SCANCODE_RIGHT:
                app->scene.objects[0].position.x += 0.1f;
                break;
            case SDL_SCANCODE_UP:
                app->scene.objects[0].position.y += 0.1f;
                break;
            case SDL_SCANCODE_DOWN:
                app->scene.objects[0].position.y -= 0.1f;
                break;
            // Object 1 rotation (U/I keys)
            case SDL_SCANCODE_U:
                if (app->scene.object_count >= 2) app->scene.objects[1].rotation.y -= 5;
                break;
            case SDL_SCANCODE_I:
                if (app->scene.object_count >= 2) app->scene.objects[1].rotation.y += 5;
                break;
            // Object 1 position (O/P, J/K, L/; keys)
            case SDL_SCANCODE_O:
                if (app->scene.object_count >= 2) app->scene.objects[1].position.x -= 0.1f;
                break;
            case SDL_SCANCODE_P:
                if (app->scene.object_count >= 2) app->scene.objects[1].position.x += 0.1f;
                break;
            case SDL_SCANCODE_J:
                if (app->scene.object_count >= 2) app->scene.objects[1].position.y += 0.1f;
                break;
            case SDL_SCANCODE_K:
                if (app->scene.object_count >= 2) app->scene.objects[1].position.y -= 0.1f;
                break;
            // Lighting and material controls
            case SDL_SCANCODE_KP_PLUS:
            case SDL_SCANCODE_EQUALS:
                app->scene.material.diffuse.red += 0.1f;
                if (app->scene.material.diffuse.red > 1.0f) app->scene.material.diffuse.red = 1.0f;
                printf("[MATERIAL] Diffuse Red: %.2f\n", app->scene.material.diffuse.red);
                break;
            case SDL_SCANCODE_KP_MINUS:
            case SDL_SCANCODE_MINUS:
                app->scene.material.diffuse.red -= 0.1f;
                if (app->scene.material.diffuse.red < 0.0f) app->scene.material.diffuse.red = 0.0f;
                printf("[MATERIAL] Diffuse Red: %.2f\n", app->scene.material.diffuse.red);
                break;
            // Lighting presets
            case SDL_SCANCODE_F2:
                // Ambiens csak
                app->scene.material.ambient.red = 0.5f;
                app->scene.material.ambient.green = 0.5f;
                app->scene.material.ambient.blue = 0.5f;
                app->scene.material.diffuse.red = 0.0f;
                app->scene.material.diffuse.green = 0.0f;
                app->scene.material.diffuse.blue = 0.0f;
                app->scene.material.specular.red = 0.0f;
                app->scene.material.specular.green = 0.0f;
                app->scene.material.specular.blue = 0.0f;
                printf("[LIGHTING] Ambient only\n");
                break;
            case SDL_SCANCODE_F3:
                // Diffuse csak
                app->scene.material.ambient.red = 0.0f;
                app->scene.material.ambient.green = 0.0f;
                app->scene.material.ambient.blue = 0.0f;
                app->scene.material.diffuse.red = 1.0f;
                app->scene.material.diffuse.green = 1.0f;
                app->scene.material.diffuse.blue = 1.0f;
                app->scene.material.specular.red = 0.0f;
                app->scene.material.specular.green = 0.0f;
                app->scene.material.specular.blue = 0.0f;
                printf("[LIGHTING] Diffuse only\n");
                break;
            case SDL_SCANCODE_F4:
                // Specular csak
                app->scene.material.ambient.red = 0.0f;
                app->scene.material.ambient.green = 0.0f;
                app->scene.material.ambient.blue = 0.0f;
                app->scene.material.diffuse.red = 0.0f;
                app->scene.material.diffuse.green = 0.0f;
                app->scene.material.diffuse.blue = 0.0f;
                app->scene.material.specular.red = 0.8f;
                app->scene.material.specular.green = 0.8f;
                app->scene.material.specular.blue = 0.8f;
                app->scene.material.shininess = 100.0f;
                printf("[LIGHTING] Specular only (shiny)\n");
                break;
            case SDL_SCANCODE_F5:
                // Réz hatás (fémek egyike)
                app->scene.material.ambient.red = 0.327f;
                app->scene.material.ambient.green = 0.258f;
                app->scene.material.ambient.blue = 0.080f;
                app->scene.material.diffuse.red = 0.780f;
                app->scene.material.diffuse.green = 0.568f;
                app->scene.material.diffuse.blue = 0.113f;
                app->scene.material.specular.red = 0.992f;
                app->scene.material.specular.green = 0.941f;
                app->scene.material.specular.blue = 0.808f;
                app->scene.material.shininess = 27.90f;
                printf("[LIGHTING] Copper material\n");
                break;
            case SDL_SCANCODE_F6:
                // Arany hatás
                app->scene.material.ambient.red = 0.247f;
                app->scene.material.ambient.green = 0.199f;
                app->scene.material.ambient.blue = 0.075f;
                app->scene.material.diffuse.red = 0.751f;
                app->scene.material.diffuse.green = 0.606f;
                app->scene.material.diffuse.blue = 0.226f;
                app->scene.material.specular.red = 0.628f;
                app->scene.material.specular.green = 0.555f;
                app->scene.material.specular.blue = 0.366f;
                app->scene.material.shininess = 51.20f;
                printf("[LIGHTING] Gold material\n");
                break;
            // Light position controls
            case SDL_SCANCODE_R:
                app->scene.light_position.x += 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            case SDL_SCANCODE_T:
                app->scene.light_position.x -= 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            case SDL_SCANCODE_Y:
                app->scene.light_position.y += 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            case SDL_SCANCODE_H:
                app->scene.light_position.y -= 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            case SDL_SCANCODE_G:
                app->scene.light_position.z += 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            case SDL_SCANCODE_B:
                app->scene.light_position.z -= 0.5f;
                printf("[LIGHT] X: %.2f, Y: %.2f, Z: %.2f\n", 
                    app->scene.light_position.x, app->scene.light_position.y, app->scene.light_position.z);
                break;
            // Light intensity controls
            case SDL_SCANCODE_KP_8:
                app->scene.light_intensity += 0.1f;
                if (app->scene.light_intensity > 2.0f) app->scene.light_intensity = 2.0f;
                printf("[LIGHT] Intensity: %.2f\n", app->scene.light_intensity);
                break;
            case SDL_SCANCODE_KP_2:
                app->scene.light_intensity -= 0.1f;
                if (app->scene.light_intensity < 0.0f) app->scene.light_intensity = 0.0f;
                printf("[LIGHT] Intensity: %.2f\n", app->scene.light_intensity);
                break;
            // Dynamic lighting modes
            case SDL_SCANCODE_F7:
                app->scene.dynamic_lighting = 0;
                printf("[LIGHT] Static light\n");
                break;
            case SDL_SCANCODE_F8:
                app->scene.dynamic_lighting = 1;
                printf("[LIGHT] Moving light (circle)\n");
                break;
            case SDL_SCANCODE_F9:
                app->scene.dynamic_lighting = 2;
                printf("[LIGHT] Pulsing light intensity\n");
                break;
            case SDL_SCANCODE_F1:
                printf("[HELP] Keybindings:\n");
                printf("  1/2/3/4 - Mode selection (single/grid/rotating/interactive)\n");
                printf("  Q/E - Rotate obj 0, U/I - Rotate obj 1\n");
                printf("  Arrows - Move obj 0, O/P/J/K - Move obj 1\n");
                printf("  W/A/S/D - Camera, Mouse - Camera rotation\n");
                printf("  +/- - Material color change\n");
                break;
            // Camera movement
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), elapsed_time);
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
