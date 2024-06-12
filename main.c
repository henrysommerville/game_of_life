#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// define width and height and number of cells per width
#define WIDTH 400
#define HEIGHT 400
#define NUM_OF_CELLS 40

#define UPDATE_TIME 10

void draw_grid_lines(void) {
    for (int r = 0; r < WIDTH; r++) {
        Vector2 Start = {r * HEIGHT / NUM_OF_CELLS, 0};
        Vector2 End = {r * HEIGHT / NUM_OF_CELLS, HEIGHT};
        DrawLineV(Start, End, BLACK);
    }
    for (int r = 0; r < HEIGHT; r++) {
        Vector2 Start = {0, r * WIDTH / NUM_OF_CELLS};
        Vector2 End = {WIDTH, r * WIDTH / NUM_OF_CELLS};
        DrawLineV(Start, End, BLACK);
    }
}

int calculate_wrapped_value(int index) {
    return ((index) >= 0) ? (index % NUM_OF_CELLS) : (NUM_OF_CELLS + (index % NUM_OF_CELLS)) % NUM_OF_CELLS;
}

int calculate_live_neighbours(int Grid[NUM_OF_CELLS][NUM_OF_CELLS], int r, int c) {
    int live_neighbours = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int row = calculate_wrapped_value(r + i);
            int col = calculate_wrapped_value(c + j);
            if (row == r && col == c) {
                continue;
            } else if (Grid[row][col] == 1) {
                live_neighbours++;
            }
        }
    }
    return live_neighbours;
}

void redraw_grid(int Grid[NUM_OF_CELLS][NUM_OF_CELLS]) {
    for (int r = 0; r < NUM_OF_CELLS; r++) {
        for (int c = 0; c < NUM_OF_CELLS; c++) {
            if ((Grid[r][c]) == 1) {
                Vector2 pos = {r * ((WIDTH) / NUM_OF_CELLS), c * ((HEIGHT) / NUM_OF_CELLS)};
                Vector2 size = {WIDTH / NUM_OF_CELLS, HEIGHT / NUM_OF_CELLS};
                DrawRectangleV(pos, size, RED);
            }
        }
    }
}
int main() {
    int Grid[NUM_OF_CELLS][NUM_OF_CELLS] = {0};
    bool play = false;
    bool paused = false;
    bool updateReady = true;
    int last_update = 0;

    InitWindow(WIDTH, HEIGHT, "TEST");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        draw_grid_lines();
        if (!play) {
            redraw_grid(Grid);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int x = GetMouseX();
                int y = GetMouseY();
                int grid_row = x * NUM_OF_CELLS / WIDTH;
                int grid_col = y * NUM_OF_CELLS / HEIGHT;
                Grid[grid_row][grid_col] = !Grid[grid_row][grid_col];
            }

            if (IsKeyPressed(KEY_SPACE)) {
                play = true;
            }

        } else if (!paused) {
            // game loop
            if (updateReady) {
                for (int r = 0; r < NUM_OF_CELLS; r++) {
                    for (int c = 0; c < NUM_OF_CELLS; c++) {
                        // 1. Any live cell with fewer than two live neighbors dies
                        // 2. Any live cell with two or three live neighbors lives
                        // 3. Any live cell with more than three live neighbours dies
                        // 4. Any dead cell with exactly three live neighbors is reborn
                        int live_neighbours = calculate_live_neighbours(Grid, r, c);
                        if (live_neighbours < 2 && Grid[r][c] == 1) {
                            Grid[r][c] = 0;
                        } else if (live_neighbours <= 3 && Grid[r][c] == 1) {
                            Grid[r][c] = 1;
                        } else if (live_neighbours > 3 && Grid[r][c] == 1) {
                            Grid[r][c] = 0;
                        } else if (live_neighbours == 3 && Grid[r][c] == 0) {
                            Grid[r][c] = 1;
                        }
                    }
                }

                redraw_grid(Grid);
            }
            if (IsKeyPressed(KEY_SPACE)) {
                play = false;
            }
            if (time(NULL) - last_update > UPDATE_TIME) {
                updateReady = true;
                last_update = time(NULL);
            }
        }
        EndDrawing();
    }
    printf("Hello World!\n");
    return 0;
}
