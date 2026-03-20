#include "game.h"

#include <GL/gl.h>

#include <stdio.h>

// Helper function to render a single digit using GL_QUADS
static void render_digit(int digit, float x, float y, float size)
{
    // Simple digit rendering using rectangles
    // Each digit is drawn as a 7-segment display style
    float w = size / 2;
    float h = size;
    
    glColor3f(1.0f, 1.0f, 1.0f);  // White color
    
    switch (digit) {
    case 0:
        glBegin(GL_QUADS);
        // Top
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h * 0.15f);
        glVertex2f(x, y + h * 0.15f);
        // Left
        glVertex2f(x, y);
        glVertex2f(x + w * 0.2f, y);
        glVertex2f(x + w * 0.2f, y + h);
        glVertex2f(x, y + h);
        // Right
        glVertex2f(x + w * 0.8f, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x + w * 0.8f, y + h);
        // Bottom
        glVertex2f(x, y + h * 0.85f);
        glVertex2f(x + w, y + h * 0.85f);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        break;
    case 1:
        glBegin(GL_QUADS);
        glVertex2f(x + w * 0.6f, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x + w * 0.6f, y + h);
        glEnd();
        break;
    case 2:
        glBegin(GL_QUADS);
        // Top
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h * 0.15f);
        glVertex2f(x, y + h * 0.15f);
        // Top right
        glVertex2f(x + w * 0.8f, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h * 0.45f);
        glVertex2f(x + w * 0.8f, y + h * 0.45f);
        // Middle
        glVertex2f(x, y + h * 0.4f);
        glVertex2f(x + w, y + h * 0.4f);
        glVertex2f(x + w, y + h * 0.6f);
        glVertex2f(x, y + h * 0.6f);
        // Bottom left
        glVertex2f(x, y + h * 0.55f);
        glVertex2f(x + w * 0.2f, y + h * 0.55f);
        glVertex2f(x + w * 0.2f, y + h);
        glVertex2f(x, y + h);
        // Bottom
        glVertex2f(x, y + h * 0.85f);
        glVertex2f(x + w, y + h * 0.85f);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        break;
    default:
        // Render as a simple digit placeholder
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        break;
    }
}

// Render score display
static void render_score(int score, float x, float y, float size)
{
    if (score >= 10) {
        // Two digits
        render_digit(score / 10, x - size, y, size);
        render_digit(score % 10, x + size * 0.5f, y, size);
    } else {
        // Single digit
        render_digit(score, x, y, size);
    }
}

void init_game(Game* game, int width, int height)
{
    game->is_running = false;
    game->width = width;
    game->height = height;
    if (init_sdl(game) == false) {
        return;
    }
    init_opengl(game);
    init_pong(&(game->pong), width, height);
    game->last_update_time = (double)SDL_GetTicks() / 1000;
    game->is_running = true;
}

void destroy_game(Game* game)
{
    if (game->gl_context != NULL) {
        SDL_GL_DeleteContext(game->gl_context);
    }

    if (game->window != NULL) {
        SDL_DestroyWindow(game->window);
    }

    SDL_Quit();
}

void handle_game_events(Game* game)
{
    SDL_Event event;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                game->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_left_pad_speed(&(game->pong), -100);
                break;
            case SDL_SCANCODE_S:
                set_left_pad_speed(&(game->pong), +100);
                break;
            case SDL_SCANCODE_UP:
                // Increase ball size
                resize_ball(&(game->pong), 5);
                break;
            case SDL_SCANCODE_DOWN:
                // Decrease ball size
                resize_ball(&(game->pong), -5);
                break;
            case SDL_SCANCODE_KP_PLUS:
            case SDL_SCANCODE_EQUALS:
                // Increase ball size
                resize_ball(&(game->pong), 5);
                break;
            case SDL_SCANCODE_MINUS:
                // Decrease ball size
                resize_ball(&(game->pong), -5);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_left_pad_speed(&(game->pong), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            set_right_pad_position(&(game->pong), y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&x, &y);
                set_ball_position(&(game->pong), x, y);
            }
            break;
        case SDL_QUIT:
            game->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_game(Game* game)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - game->last_update_time;
    game->last_update_time = current_time;

    update_pong(&(game->pong), elapsed_time);
}

void render_game(Game* game)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_pong(&(game->pong));
    
    // Render scores
    render_score(game->pong.left_score, game->width / 4, 30, 30);
    render_score(game->pong.right_score, 3 * game->width / 4, 30, 30);
    
    SDL_GL_SwapWindow(game->window);
}

bool init_sdl(Game* game)
{
    int error_code;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(
        "Pong!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        game->width, game->height,
        SDL_WINDOW_OPENGL);
    if (game->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return false;
    }

    game->gl_context = SDL_GL_CreateContext(game->window);
    if (game->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return false;
    }

    return true;
}

void init_opengl(Game* game)
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, game->width, game->height, 0, -200, 200);
    glViewport(0, 0, game->width, game->height);
}
