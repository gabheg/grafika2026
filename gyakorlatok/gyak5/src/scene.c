#include "scene.h"

#include <GL/gl.h>
#include <math.h>

void init_scene(Scene* scene)
{
    scene->rotation = 0.0;
}

void update_scene(Scene* scene)
{
    scene->rotation += 1.0;
    if (scene->rotation > 360.0) {
        scene->rotation -= 360.0;
    }
}

void render_scene(const Scene* scene)
{
    draw_origin();
    draw_checkerboard();
    draw_colored_triangle();
    
    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.5);
    glRotatef(((Scene*)scene)->rotation, 1, 1, 1);
    glTranslatef(-0.5, -0.5, -0.5);
    draw_tessellated_sphere();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.3, 0.3, 0.3);
    draw_cylinder();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.7, 0.3, 0.3);
    draw_cone();
    glPopMatrix();
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

void draw_colored_triangle()
{
    glBegin(GL_TRIANGLES);
    
    glColor3f(1, 1, 0);
    glVertex3f(0.5, 0.5, 0.8);
    
    glColor3f(0, 1, 1);
    glVertex3f(0.2, 0.2, 0.5);
    
    glColor3f(1, 0, 1);
    glVertex3f(0.8, 0.2, 0.5);
    
    glEnd();
}

void draw_tessellated_sphere()
{
    int stacks = 8;
    int slices = 8;
    float radius = 0.15;
    int i, j;
    float x, y, z;
    float lat, lon;
    float lat_step = M_PI / stacks;
    float lon_step = 2.0 * M_PI / slices;
    
    glColor3f(0.7, 0.7, 0.7);
    
    for (i = 0; i < stacks; i++) {
        lat = -M_PI / 2.0 + i * lat_step;
        
        glBegin(GL_TRIANGLE_STRIP);
        for (j = 0; j <= slices; j++) {
            lon = j * lon_step;
            
            x = radius * cos(lat) * cos(lon);
            y = radius * cos(lat) * sin(lon);
            z = radius * sin(lat);
            glVertex3f(0.5 + x, 0.5 + y, 0.5 + z);
            
            x = radius * cos(lat + lat_step) * cos(lon);
            y = radius * cos(lat + lat_step) * sin(lon);
            z = radius * sin(lat + lat_step);
            glVertex3f(0.5 + x, 0.5 + y, 0.5 + z);
        }
        glEnd();
    }
}

void draw_checkerboard()
{
    int squareSize = 5;
    int i, j;
    float x, y;
    
    glDisable(GL_DEPTH_TEST);
    
    for (i = 0; i < squareSize; i++) {
        for (j = 0; j < squareSize; j++) {
            x = (float)i / squareSize;
            y = (float)j / squareSize;
            
            if ((i + j) % 2 == 0) {
                glColor3f(0.2, 0.2, 0.2);
            } else {
                glColor3f(0.8, 0.8, 0.8);
            }
            
            glBegin(GL_QUADS);
            glVertex3f(x, y, 0);
            glVertex3f(x + 1.0f / squareSize, y, 0);
            glVertex3f(x + 1.0f / squareSize, y + 1.0f / squareSize, 0);
            glVertex3f(x, y + 1.0f / squareSize, 0);
            glEnd();
        }
    }
    
    glEnable(GL_DEPTH_TEST);
}

void draw_cylinder()
{
    int slices = 12;
    int stacks = 3;
    float radius = 0.1;
    float height = 0.2;
    int i, j;
    float angle, angle_step = 2.0 * M_PI / slices;
    float x1, y1, x2, y2;
    float z;
    
    glColor3f(0.5, 0.5, 0.8);
    
    for (j = 0; j < stacks; j++) {
        z = j * height / stacks;
        float z_next = (j + 1) * height / stacks;
        
        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i <= slices; i++) {
            angle = i * angle_step;
            x1 = radius * cos(angle);
            y1 = radius * sin(angle);
            x2 = radius * cos(angle);
            y2 = radius * sin(angle);
            
            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z_next);
        }
        glEnd();
    }
}

void draw_cone()
{
    int slices = 12;
    float radius = 0.1;
    float height = 0.2;
    int i;
    float angle, angle_step = 2.0 * M_PI / slices;
    float x, y;
    
    glColor3f(0.8, 0.5, 0.5);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, height);
    
    for (i = 0; i <= slices; i++) {
        angle = i * angle_step;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    
    for (i = slices; i >= 0; i--) {
        angle = i * angle_step;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0);
    }
    glEnd();
}
