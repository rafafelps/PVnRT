#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define AMOUNT_BOXES 2
#define MAX_CIRCLES 100

struct Circle {
    Vector2 pos;
    Vector2 speed;
    float radius;
    Color color;
};

typedef struct Circle Circle;

void drawEquation();
Rectangle* initBoxes();
void destroyBoxes(Rectangle* boxes);
void destroyCircles(Circle** circles);

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 450;
int main() {
    InitWindow(screenWidth, screenHeight, "PV = nRT");
    SetTargetFPS(60);

    Rectangle* boxes = initBoxes();
    Circle** circles = (Circle**)malloc(sizeof(Circle*) * AMOUNT_BOXES);
    for (int i = 0; i < AMOUNT_BOXES; i++) {
        circles[i] = (Circle*)malloc(sizeof(Circle) * MAX_CIRCLES);
    }

    unsigned int circleCount[2] = {0};
    unsigned int boxThickness = 2;

    for (int currBox = 0; currBox < AMOUNT_BOXES; currBox++) {
        for (circleCount[currBox] = 0; circleCount[currBox] < MAX_CIRCLES; circleCount[currBox]++) {
            circles[currBox][circleCount[currBox]].radius = 3;
            circles[currBox][circleCount[currBox]].color = (Color){0, 71, 171, 255};
            
            circles[currBox][circleCount[currBox]].pos.x = GetRandomValue(boxes[currBox].x + boxThickness + circles[currBox][circleCount[currBox]].radius, boxes[currBox].x + boxes[currBox].width - boxThickness - circles[currBox][circleCount[currBox]].radius);
            circles[currBox][circleCount[currBox]].pos.y = GetRandomValue(boxes[currBox].y + boxThickness + circles[currBox][circleCount[currBox]].radius, boxes[currBox].y + boxes[currBox].height - boxThickness - circles[currBox][circleCount[currBox]].radius);

            float delta = 0;
            while (delta < 1 || delta > 3) {
                circles[currBox][circleCount[currBox]].speed.x = (float)GetRandomValue(-180, 180) / 60.f;
                circles[currBox][circleCount[currBox]].speed.y = (float)GetRandomValue(-180, 180) / 60.f;

                delta = sqrtf(powf(circles[currBox][circleCount[currBox]].speed.x, 2) + powf(circles[currBox][circleCount[currBox]].speed.y, 2));
            }
        }
    }

    while (!WindowShouldClose()) {
        // Update

        /* Codigo para adicionar mais bolas
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x > boxes[0].x + boxThickness && mousePos.x < boxes[0].x + boxes[0].width - boxThickness &&
            mousePos.y > boxes[0].y + boxThickness && mousePos.y < boxes[0].y + boxes[0].height - boxThickness) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                for (int i = 0; i < 2; i++) {
                    if (circleCount < MAX_CIRCLES) {
                        circles[circleCount].pos = GetMousePosition();
                        float delta = 0;
                        while (delta < 1 || delta > 3) {
                            circles[circleCount].speed.x = (float)GetRandomValue(-180, 180) / 60.f;
                            circles[circleCount].speed.y = (float)GetRandomValue(-180, 180) / 60.f;

                            delta = sqrtf(powf(circles[circleCount].speed.x, 2) + powf(circles[circleCount].speed.y, 2));
                        }
                        circles[circleCount].radius = 3;
                        circles[circleCount].color = (Color){0, 71, 171, 255};

                        circleCount++;
                    }
                }
            }
        }*/

        // Update Circles
        for (int currBox = 0; currBox < AMOUNT_BOXES; currBox++) {
            for (int i = 0; i < circleCount[currBox]; i++) {
                circles[currBox][i].pos.x += circles[currBox][i].speed.x;
                circles[currBox][i].pos.y += circles[currBox][i].speed.y;

                if (circles[currBox][i].pos.x - circles[currBox][i].radius < boxes[currBox].x + boxThickness || circles[currBox][i].pos.x + circles[currBox][i].radius > boxes[currBox].x + boxes[currBox].width - boxThickness)
                    circles[currBox][i].speed.x *= -1;
                if (circles[currBox][i].pos.y - circles[currBox][i].radius < boxes[currBox].y + boxThickness || circles[currBox][i].pos.y + circles[currBox][i].radius > boxes[currBox].y + boxes[currBox].width - boxThickness)
                    circles[currBox][i].speed.y *= -1;
            }
        }
        
        // Render
        BeginDrawing();

        ClearBackground(BLACK);
        drawEquation();

        DrawRectangle(0, 0, screenWidth * 0.3, screenHeight, (Color){189, 181, 213, 255});


        for (int i = 0; i < AMOUNT_BOXES; i++) {
            for (int j = 0; j < circleCount[i]; j++) {
                DrawCircle(circles[i][j].pos.x, circles[i][j].pos.y, circles[i][j].radius, circles[i][j].color);
            }
            DrawRectangleLinesEx(boxes[i], 2, RAYWHITE);
        }

        EndDrawing();
    }

    destroyCircles(circles);
    destroyBoxes(boxes);
    CloseWindow();

    return 0;
}

void drawEquation() {
    DrawText("P = nRT", screenWidth * 0.55, screenHeight * 0.1, 20, RAYWHITE);
    DrawText("V", screenWidth * 0.614, screenHeight * 0.15, 20, RAYWHITE);
    DrawRectangle(screenWidth * 0.595, screenHeight * 0.14 + 1, 47, 2, RAYWHITE);
}

Rectangle* initBoxes() {
    Rectangle* boxes = (Rectangle*)malloc(sizeof(Rectangle) * AMOUNT_BOXES);

    Vector2 center = {screenWidth * 0.6, screenHeight * 0.5};
    boxes[0].width = 150;
    boxes[0].height = 150;
    boxes[0].x = center.x - 25 - (boxes[0].width / 2);
    boxes[0].y = center.y - (boxes[0].height / 2);

    boxes[1] = boxes[0];
    boxes[1].x = center.x + 25 + (boxes[0].width / 2);

    return boxes;
}

void destroyBoxes(Rectangle* boxes) {
    free(boxes);
}

void destroyCircles(Circle** circles) {
    for (int i = 0; i < AMOUNT_BOXES; i++) {
        free(circles[i]);
    }
    free(circles);
}
