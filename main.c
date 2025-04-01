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

Color wall_colors[] = {
  RED,
  BLUE,
  GREEN,
  MAGENTA
};

typedef struct {
  float x, y;
} Vec2;

Vec2 vec2_add(Vec2 a, Vec2 b) {
  return (Vec2){a.x + b.x, a.y + b.y};
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
  return (Vec2){a.x - b.x, a.y - b.y};
}

Vec2 vec2_mul(Vec2 a, Vec2 b) {
  return (Vec2){a.x * b.x, a.y * b.y};
}

typedef struct {
  int x, y, w, h;
} Map;

// Convert world coordinates to map coordinates
Vec2 world_to_map(Map map, Vec2 v) {
  float x = (float)map.w / (float)WORLD_WIDTH * v.x;
  float y = (float)map.w / (float)WORLD_HEIGHT * v.y;
  return (Vec2){x + map.x, y + map.y};
}

void render_map(Map map) {
  DrawRectangle(map.x, map.y, map.w, map.h, BLACK);

  float cell_width = (float)map.w / (float)WORLD_WIDTH;
  float cell_height = (float)map.h / (float)WORLD_HEIGHT;

  // Draw walls
  for (int y = 0; y < WORLD_HEIGHT; y++) {
    for (int x = 0; x < WORLD_WIDTH; x++) {
      int wall = world[y][x];
      if (wall != 0) {
        DrawRectangle(
          (int)(x * cell_width) + map.x, (int)(y * cell_height) + map.y,
          (int)(cell_width), (int)(cell_height),
          wall_colors[wall]);
      }
    }
  }

  int at;
  // Vertical grid lines
  for (int x = 0; x <= WORLD_WIDTH; x++) {
    at = (int)(x * cell_width);
    DrawLine(at + map.x, map.y, at + map.x, map.h + map.y, GRAY);
  }
  // Horizontal grid lines
  for (int y = 0; y <= WORLD_HEIGHT; y++) {
    at = (int)(y * cell_height);
    DrawLine(map.x, at + map.y, map.w + map.x, at + map.y, GRAY);
  }
}

void render_map_player(Map map, Vec2 player) {
  Vec2 pos = world_to_map(map, player);
  DrawCircle((int)pos.x, (int)pos.y, 5.0f, RED);
}

int main(void)
{
  // Define mini map position and size
  Map map = {20, 20, 300, 300};

  Vec2 player = {5.5f, 6.5f};

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raycasting in C");
  SetTargetFPS(60);

  while ( ! WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(BACKGROUND);
      render_map(map);
      render_map_player(map, player);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
