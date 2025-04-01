#include <raylib.h>

#define BACKGROUND CLITERAL(Color){20, 20, 20, 255}

#define FACTOR         60
#define WINDOW_WIDTH   16*FACTOR
#define WINDOW_HEIGHT  9*FACTOR

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
