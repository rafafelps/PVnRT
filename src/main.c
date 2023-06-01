#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define AMOUNT_BOXES 2
#define MAX_CIRCLES 100

struct Circle {
    Vector2 pos;
    Vector2 speed;
    float radius;
    Color color;
};

typedef struct Circle Circle;

void drawEquation(int mode);
void drawButtonsLabel(Rectangle* buttons);
void changeMode(int mode, Circle** circles, Rectangle* boxes, unsigned int* circleCount, unsigned int boxThickness);
Rectangle* initBoxes();
Rectangle* initButtons();
void destroyBoxes(Rectangle* boxes);
void destroyCircles(Circle** circles);
void destroyButtons(Rectangle* buttons);

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 450;

int main() {
    InitWindow(screenWidth, screenHeight, "PV = nRT");
    SetTargetFPS(60);

    Rectangle* boxes = initBoxes();
    Circle** circles = (Circle**)malloc(sizeof(Circle*) * AMOUNT_BOXES);
    for (int i = 0; i < AMOUNT_BOXES; i++) {
        if (i) {
            circles[i] = (Circle*)malloc(sizeof(Circle) * MAX_CIRCLES * 2);
        } else {
            circles[i] = (Circle*)malloc(sizeof(Circle) * MAX_CIRCLES);
        }
    }
    Rectangle* buttons = initButtons();

    unsigned int* circleCount = (unsigned int*)calloc(2, sizeof(unsigned int));
    unsigned int boxThickness = 2;

    unsigned int mode = 0;
    changeMode(0, circles, boxes, circleCount, boxThickness);

    unsigned int frameCount = 1;
    unsigned int pressureCount[2] = {0};
    char p[50] = {'\0'};
    char p2[50] = {'\0'};
    p[0] = 'P'; p2[0] = 'P';
    p[1] = ':'; p2[1] = ':';
    p[2] = ' '; p2[2] = ' ';
    while (!WindowShouldClose()) {
        frameCount++;

        // Detects button clicks
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 4; i++) {
                if (mousePos.x >= buttons[i].x && mousePos.x <= buttons[i].x + buttons[i].width &&
                    mousePos.y >= buttons[i].y && mousePos.y <= buttons[i].y + buttons[i].height) {
                    mode = i;
                    changeMode(mode, circles, boxes, circleCount, boxThickness);
                    frameCount = 1;
                    pressureCount[0] = 0;
                    pressureCount[1] = 0;
                }
            }
        }

        // Update Circles
        for (int currBox = 0; currBox < AMOUNT_BOXES; currBox++) {
            for (int i = 0; i < circleCount[currBox]; i++) {
                circles[currBox][i].pos.x += circles[currBox][i].speed.x;
                circles[currBox][i].pos.y += circles[currBox][i].speed.y;

                if (circles[currBox][i].pos.x - circles[currBox][i].radius < boxes[currBox].x + boxThickness || circles[currBox][i].pos.x + circles[currBox][i].radius > boxes[currBox].x + boxes[currBox].width - boxThickness) {
                    circles[currBox][i].speed.x *= -1;
                    pressureCount[currBox]++;    
                }
                if (circles[currBox][i].pos.y - circles[currBox][i].radius < boxes[currBox].y + boxThickness || circles[currBox][i].pos.y + circles[currBox][i].radius > boxes[currBox].y + boxes[currBox].width - boxThickness) {
                    circles[currBox][i].speed.y *= -1;
                    pressureCount[currBox]++;
                }
            }
        }
        
        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        // UI
        drawEquation(mode);
        if (mode == 1) {
            DrawText("n", screenWidth / 2 + 15, screenHeight * 0.25, 20, RAYWHITE);
            DrawText("2n", screenWidth * 0.75 + 10, screenHeight * 0.25, 20, RAYWHITE);
        } else if (mode == 2) {
            DrawText("T", screenWidth / 2 + 15, screenHeight * 0.25, 20, RAYWHITE);
            DrawText("2T", screenWidth * 0.75 + 10, screenHeight * 0.25, 20, RAYWHITE);
        } else if (mode == 3) {
            DrawText("V", screenWidth / 2 + 15, screenHeight * 0.25, 20, RAYWHITE);
            DrawText("V/2", screenWidth * 0.75, screenHeight * 0.25, 20, RAYWHITE);
        } else {
            DrawText("=", (0.7 / 2 + 0.3) * screenWidth - 3, screenHeight * 0.5 - 3 , 20, RAYWHITE);
        }
        
        char s[50];
        float pressureGauge[2] = {0};
        if (frameCount >= 61) {
            p[3] = '\0'; p2[3] = '\0';
            pressureGauge[0] = (float)(pressureCount[0]) / frameCount;
            gcvt(pressureGauge[0] + (4.5 * pressureGauge[0]), 4, s);
            strcat(p, s);
            pressureGauge[1] = (float)(pressureCount[1]) / frameCount;
            if (mode == 3) {
                pressureGauge[1] = 2 * pressureGauge[0] + (GetRandomValue(0, RAND_MAX) / ((float)(RAND_MAX) * 10));
            }
            gcvt(pressureGauge[1] + (4.5 * pressureGauge[1]), 4, s);
            strcat(p2, s);

            pressureCount[0] = 0;
            pressureCount[1] = 0;
            frameCount = 1;
        }
        if (p[6] == '\0') { p[6] = '0'; p[7] = '0'; p[8] = '\0'; }
        if (p2[6] == '\0') { p2[6] = '0'; p2[7] = '0'; p2[8] = '\0'; }
        DrawText(p, 390, 330, 20, RAYWHITE);
        DrawText(p2, 585, 330, 20, RAYWHITE);

        // Left panel with buttons
        DrawRectangle(0, 0, screenWidth * 0.3, screenHeight, (Color){73, 95, 255, 255});
        for (int i = 0; i < 4; i++) {
            DrawRectangleRounded(buttons[i], 0.2, 4, (Color){117, 137, 255, 255});
        }
        drawButtonsLabel(buttons);

        // Circles and boxes
        for (int i = 0; i < AMOUNT_BOXES; i++) {
            for (int j = 0; j < circleCount[i]; j++) {
                DrawCircle(circles[i][j].pos.x, circles[i][j].pos.y, circles[i][j].radius, circles[i][j].color);
            }
            DrawRectangleLinesEx(boxes[i], 2, RAYWHITE);
        }

        EndDrawing();
    }

    destroyButtons(buttons);
    destroyCircles(circles);
    destroyBoxes(boxes);
    CloseWindow();

    return 0;
}

void drawEquation(int mode) {
    Color PColor = RAYWHITE;
    Color nColor = RAYWHITE;
    Color RColor = RAYWHITE;
    Color TColor = RAYWHITE;
    Color VColor = RAYWHITE;

    if (mode == 1) {
        PColor = GREEN;
        nColor = GREEN;
    } else if (mode == 2) {
        PColor = GREEN;
        TColor = GREEN;
    } else if (mode == 3) {
        PColor = GREEN;
        VColor = RED;
    }

    DrawText("P =", (0.7 / 2 + 0.3) * screenWidth - 42, screenHeight * 0.1, 20, RAYWHITE);
    DrawText("P", (0.7 / 2 + 0.3) * screenWidth - 42, screenHeight * 0.1, 20, PColor);
    DrawText("n", (0.7 / 2 + 0.3) * screenWidth - 2, screenHeight * 0.1, 20, nColor);
    DrawText("R", (0.7 / 2 + 0.3) * screenWidth + 12, screenHeight * 0.1, 20, RColor);
    DrawText("T", (0.7 / 2 + 0.3) * screenWidth + 28, screenHeight * 0.1, 20, TColor);
    DrawText("V", (0.7 / 2 + 0.3) * screenWidth + 13, screenHeight * 0.1 + 21, 20, VColor);
    DrawRectangle((0.7 / 2 + 0.3) * screenWidth - 3, screenHeight * 0.1 + 19, 46, 2, RAYWHITE);
}

void drawButtonsLabel(Rectangle* buttons) {
    DrawText("Equal (=)", buttons[0].x + 19, buttons[0].y + (buttons[0].height / 2) - 15, 35, (Color){170, 183, 255, 255});
    DrawText("Atoms (n)", buttons[1].x + 16, buttons[1].y + (buttons[1].height / 2) - 15, 35, (Color){170, 183, 255, 255});
    DrawText("Temp. (T)", buttons[2].x + 13, buttons[2].y + (buttons[2].height / 2) - 15, 35, (Color){170, 183, 255, 255});
    DrawText("Vol. (V)", buttons[3].x + 31, buttons[3].y + (buttons[3].height / 2) - 15, 35, (Color){170, 183, 255, 255});
}

void changeMode(int mode, Circle** circles, Rectangle* boxes, unsigned int* circleCount, unsigned int boxThickness) {
    for (int i = 0; i < AMOUNT_BOXES; i++) { circleCount[i] = 0; }
    boxes[1] = boxes[0];
    boxes[1].x = (0.7 / 2 + 0.3) * screenWidth + 25;
    
    if (mode == 1) {
        for (int currBox = 0; currBox < AMOUNT_BOXES; currBox++) {
            unsigned int circleLim = MAX_CIRCLES;
            if (currBox) { circleLim *= 2; }
            for (circleCount[currBox] = 0; circleCount[currBox] < circleLim; circleCount[currBox]++) {
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
    } else if (mode == 2) {
        for (int currBox = 0; currBox < AMOUNT_BOXES; currBox++) {
            for (circleCount[currBox] = 0; circleCount[currBox] < MAX_CIRCLES; circleCount[currBox]++) {
                circles[currBox][circleCount[currBox]].radius = 3;
                circles[currBox][circleCount[currBox]].color = (Color){0, 71, 171, 255};
                
                circles[currBox][circleCount[currBox]].pos.x = GetRandomValue(boxes[currBox].x + boxThickness + circles[currBox][circleCount[currBox]].radius, boxes[currBox].x + boxes[currBox].width - boxThickness - circles[currBox][circleCount[currBox]].radius);
                circles[currBox][circleCount[currBox]].pos.y = GetRandomValue(boxes[currBox].y + boxThickness + circles[currBox][circleCount[currBox]].radius, boxes[currBox].y + boxes[currBox].height - boxThickness - circles[currBox][circleCount[currBox]].radius);

                if (currBox) {
                    circles[currBox][circleCount[currBox]].speed.x = 2 * circles[currBox-1][circleCount[currBox]].speed.x;
                    circles[currBox][circleCount[currBox]].speed.y = 2 * circles[currBox-1][circleCount[currBox]].speed.y;
                    continue;
                }

                float delta = 0;
                while (delta < 1 || delta > 3) {
                    circles[currBox][circleCount[currBox]].speed.x = (float)GetRandomValue(-180, 180) / 60.f;
                    circles[currBox][circleCount[currBox]].speed.y = (float)GetRandomValue(-180, 180) / 60.f;

                    delta = sqrtf(powf(circles[currBox][circleCount[currBox]].speed.x, 2) + powf(circles[currBox][circleCount[currBox]].speed.y, 2));
                }
            }
        }
    } else if (mode == 3) {
        boxes[1].width /= sqrtf(2);
        boxes[1].height /= sqrtf(2);
        boxes[1].x += boxes[1].width / 4 - 5;
        boxes[1].y += boxes[1].height / 4 - 5;
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
    } else {
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
    }
}

Rectangle* initBoxes() {
    Rectangle* boxes = (Rectangle*)malloc(sizeof(Rectangle) * AMOUNT_BOXES);

    Vector2 center = {(0.7 / 2 + 0.3) * screenWidth, screenHeight * 0.5};
    boxes[0].width = 150;
    boxes[0].height = 150;
    boxes[0].x = center.x - 25 - boxes[0].width;
    boxes[0].y = center.y - (boxes[0].height / 2);

    boxes[1] = boxes[0];
    boxes[1].x = center.x + 25;

    return boxes;
}

Rectangle* initButtons() {
    Rectangle* buttons = (Rectangle*)malloc(sizeof(Rectangle) * 4);

    unsigned int margin = 25;
    float width = (screenWidth * 0.3) - (2 * margin);
    float height = ((screenHeight - (5 * margin)) / 4);
    for (int i = 0; i < 4; i++) {
        buttons[i].width = width;
        buttons[i].height = height;
        buttons[i].x = margin;
        buttons[i].y = (margin * (i + 1)) + (buttons[i].height * i);
    }

    return buttons;
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

void destroyButtons(Rectangle* buttons) {
    free(buttons);
}
