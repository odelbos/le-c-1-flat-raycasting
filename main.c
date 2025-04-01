#include <raylib.h>

#define BACKGROUND CLITERAL(Color){20, 20, 20, 255}

#define FACTOR         60
#define WINDOW_WIDTH   16*FACTOR
#define WINDOW_HEIGHT  9*FACTOR

#define WORLD_WIDTH    8
#define WORLD_HEIGHT   8

int world[WORLD_HEIGHT][WORLD_WIDTH] = {
  {1, 0, 0, 0, 0, 0, 2, 3},
  {0, 0, 1, 2, 1, 0, 0, 1},
  {0, 0, 0, 0, 1, 0, 0, 1},
  {0, 0, 0, 0, 3, 0, 0, 3},
  {0, 0, 0, 2, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raycasting in C");
  SetTargetFPS(60);

  while ( ! WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(BACKGROUND);
      DrawRectangle(100, 100, 50, 50, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
