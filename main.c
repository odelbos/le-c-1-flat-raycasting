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
  Vec2 pos;
  int w, h;
  Vec2 ratio;
} Map;

// Camera in world coordinates
typedef struct {
  Vec2 dir;           // Camera direction
  Vec2 wf, wl, wr;    // Specific points defining the FOV
} Cam;

// Convert world coordinates to map coordinates
Vec2 world_to_map(Map map, Vec2 v) {
  Vec2 a = vec2_mul(v, map.ratio);
  return vec2_add(a, map.pos);
}

void render_map(Map map) {
  DrawRectangle(map.pos.x, map.pos.y, map.w, map.h, BLACK);

  float cell_width = (float)map.w / (float)WORLD_WIDTH;
  float cell_height = (float)map.h / (float)WORLD_HEIGHT;

  // Draw walls
  for (int y = 0; y < WORLD_HEIGHT; y++) {
    for (int x = 0; x < WORLD_WIDTH; x++) {
      int wall = world[y][x];
      if (wall != 0) {
        DrawRectangle(
          (int)(x * cell_width) + map.pos.x, (int)(y * cell_height) + map.pos.y,
          (int)(cell_width), (int)(cell_height),
          wall_colors[wall]);
      }
    }
  }

  int at;
  // Vertical grid lines
  for (int x = 0; x <= WORLD_WIDTH; x++) {
    at = (int)(x * cell_width);
    DrawLine(at + map.pos.x, map.pos.y, at + map.pos.x, map.h + map.pos.y, GRAY);
  }
  // Horizontal grid lines
  for (int y = 0; y <= WORLD_HEIGHT; y++) {
    at = (int)(y * cell_height);
    DrawLine(map.pos.x, at + map.pos.y, map.w + map.pos.x, at + map.pos.y, GRAY);
  }
}

void render_map_camera(Map map, Vec2 player, Cam camera) {
  Vec2 pos = world_to_map(map, player);
  DrawCircle((int)pos.x, (int)pos.y, 5.0f, RED);

  // Draw camera with a FOV of 90°
  Vec2 mf = world_to_map(map, camera.wf);
  DrawCircle((int)mf.x, (int)mf.y, 2.5f, PINK);
  DrawLine(pos.x, pos.y, mf.x, mf.y, PINK);

  // Left FOV point
  Vec2 ml = world_to_map(map, camera.wl);
  DrawCircle((int)ml.x, (int)ml.y, 2.5f, ORANGE);
  DrawLine(pos.x, pos.y, ml.x, ml.y, ORANGE);

  // Right FOV point
  Vec2 mr = world_to_map(map, camera.wr);
  DrawCircle((int)mr.x, (int)mr.y, 2.5f, ORANGE);
  DrawLine(pos.x, pos.y, mr.x, mr.y, ORANGE);

  DrawLine(ml.x, ml.y, mr.x, mr.y, ORANGE);
}

void update_camera(Cam *camera, Vec2 player) {
  camera->wf = vec2_add(player, camera->dir);
  camera->wl = vec2_add(camera->wf, (Vec2){camera->dir.y, -camera->dir.x});
  camera->wr = vec2_add(camera->wf, (Vec2){-camera->dir.y, camera->dir.x});
}

int main(void)
{
  // Define mini map position and size
  Map map = {{20, 20}, 300, 300, {0}};
  map.ratio.x = (float)map.w / (float)WORLD_WIDTH;
  map.ratio.y = (float)map.h / (float)WORLD_HEIGHT;

  Vec2 player = {5.5f, 7.5f};

  Cam camera = {
    {0.0f, -1.0f},    // Camera direction
    {0}, {0}, {0}     // FOV key points
  };

  update_camera(&camera, player);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raycasting in C");
  SetTargetFPS(60);

  while ( ! WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(BACKGROUND);
      render_map(map);
      render_map_camera(map, player, camera);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
