#include <stdlib.h>
#include <raylib.h>
#include <math.h>

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
Rectangle* initBoxes(unsigned int amountBoxes);
void destroyBoxes(Rectangle* boxes);

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 450;
int main() {
    InitWindow(screenWidth, screenHeight, "PV = nRT");
    SetTargetFPS(60);

    Rectangle* boxes = initBoxes(AMOUNT_BOXES);
    Circle* circles = (Circle*)malloc(sizeof(Circle) * MAX_CIRCLES);
    unsigned int circleCount = 0;
    unsigned int boxThickness = 2;

    while (!WindowShouldClose()) {
        // Update
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x > boxes[0].x + boxThickness && mousePos.x < boxes[0].x + boxes[0].width - boxThickness &&
            mousePos.y > boxes[0].y + boxThickness && mousePos.y < boxes[0].y + boxes[0].height - boxThickness) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                for (int i = 0; i < 2; i++) {
                    if (circleCount < MAX_CIRCLES) {
                        circles[circleCount].pos = GetMousePosition();
                        float delta = 0;
                        while (delta <= 0.5) {
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
        }

        // Update Circles
        for (int i = 0; i < circleCount; i++) {
            circles[i].pos.x += circles[i].speed.x;
            circles[i].pos.y += circles[i].speed.y;

            if (circles[i].pos.x - circles[i].radius < boxes[0].x + boxThickness || circles[i].pos.x + circles[i].radius > boxes[0].x + boxes[0].width - boxThickness)
                circles[i].speed.x *= -1;
            if (circles[i].pos.y - circles[i].radius < boxes[0].y + boxThickness || circles[i].pos.y + circles[i].radius > boxes[0].y + boxes[0].width - boxThickness)
                circles[i].speed.y *= -1;
        }
        
        // Render
        BeginDrawing();

        ClearBackground(BLACK);
        drawEquation();

        DrawRectangle(0, 0, screenWidth * 0.3, screenHeight, (Color){189, 181, 213, 255});

        for (int i = 0; i < circleCount; i++) {
            DrawCircle(circles[i].pos.x, circles[i].pos.y, circles[i].radius, circles[i].color);
        }

        for (int i = 0; i < AMOUNT_BOXES; i++) {
            DrawRectangleLinesEx(boxes[i], 2, RAYWHITE);
        }

        EndDrawing();
    }

    destroyBoxes(boxes);
    CloseWindow();

    return 0;
}

void drawEquation() {
    DrawText("P = nRT", screenWidth * 0.55, screenHeight * 0.1, 20, RAYWHITE);
    DrawText("V", screenWidth * 0.614, screenHeight * 0.15, 20, RAYWHITE);
    DrawRectangle(screenWidth * 0.595, screenHeight * 0.14 + 1, 47, 2, RAYWHITE);
}

Rectangle* initBoxes(unsigned int amountBoxes) {
    Rectangle* boxes = (Rectangle*)malloc(sizeof(Rectangle) * amountBoxes);

    boxes[0].x = screenWidth * 0.4;
    boxes[0].y = screenHeight * 0.3;
    boxes[0].width = 150;
    boxes[0].height = 150;

    boxes[1] = boxes[0];
    boxes[1].x += boxes[1].width + 50;

    return boxes;
}

void destroyBoxes(Rectangle* boxes) {
    free(boxes);
}
