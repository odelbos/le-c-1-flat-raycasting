#include <stdio.h>
#include <math.h>
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

Vec2 vec2_scale(Vec2 v, float factor) {
  return (Vec2){v.x*factor, v.y*factor};
}

float vec2_square_len(Vec2 v) {
  return v.x * v.x + v.y * v.y;
}

Vec2 vec2_rotate(Vec2 v, float angle) {
  float ca = cosf(angle);
  float sa = sinf(angle);
  return (Vec2){v.x*ca - v.y*sa, v.x*sa + v.y*ca};
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

typedef struct {
  Vec2 vp, ct;
  int cell_x, cell_y, wall;
} CastResult;

// Convert world coordinates to map coordinates
Vec2 world_to_map(Map map, Vec2 v) {
  return vec2_add(vec2_mul(v, map.ratio), map.pos);
}

void is_ray_hit_wall(CastResult *res, Vec2 p, int step_x, int step_y) {
  Vec2 veps = {(float)step_x * 0.00001f, (float)step_y * 0.00001f};
  p = vec2_add(p, veps);

  int x = (int)floor(p.x);
  int y = (int)floor(p.y);
  if (p.x >= 0 && p.x < 8 && p.y >= 0 && p.y < 8) {
    res->cell_x = x;
    res->cell_y = y;
    res->wall = world[y][x];
  }
}

CastResult cast_ray(Map map, Vec2 player, Cam camera, float factor) {

  CastResult res = {0};

  res.vp = vec2_sub(camera.wr, camera.wl);
  res.vp = vec2_scale(res.vp, factor);
  res.vp = vec2_add(res.vp, camera.wl);
  Vec2 ray_dir = vec2_sub(res.vp, player);

  Vec2 mp = world_to_map(map, res.vp);
  DrawCircle((int)mp.x, (int)mp.y, 2.5f, GREEN);

  // NOTE:
  // x1 = player.x
  // y1 = player.y
  // --
  // x2 = wp.x
  // y2 = wp.y
  // --
  // y = m x + b
  // x = (y - b) / m
  // m = (y2 - y1) / (x2 - x1)
  // --
  // b = y1 - ((y2 - y1) / (x2 - x1)) x1
  // b = y1 - m * x1

  float m = (res.vp.y - player.y) / (res.vp.x - player.x);
  float b = player.y - m * player.x;

  int step_x = 1;
  int step_y = 1;
  int sx, sy;

  if (ray_dir.x >= 0) {
    sx = ceil(res.vp.x);
  }
  else {
    step_x = -1;
    sx = floor(res.vp.x);
  }

  if (ray_dir.y >= 0) {
    sy = ceil(res.vp.y);
  }
  else {
    step_y = -1;
    sy = floor(res.vp.y);
  }

  float x, y;
  Vec2 xt, yt, xd, yd;
  int max = 0;

  while (max < 15 && res.wall == 0) {
    y = m * (float)sx + b;
    xt = (Vec2){(float)sx, y};

    x = ((float)sy - b) / m;
    yt = (Vec2){x, (float)sy};

    xd = vec2_sub(xt, res.vp);
    yd = vec2_sub(yt, res.vp);
    float xd_len = vec2_square_len(xd);
    float yd_len = vec2_square_len(yd);

    if (xd_len <= yd_len) {
      if (xt.x > 0 && xt.x <= 8 && xt.y > 0 && xt.y <= 8) {
        mp = world_to_map(map, xt);
        DrawCircle((int)mp.x, (int)mp.y, 2.5f, YELLOW);
        is_ray_hit_wall(&res, xt, step_x, step_y);
        sx += step_x;
      }
    }
    else {
      if (yt.x > 0 && yt.x <= 8 && yt.y > 0 && yt.y <= 8) {
        mp = world_to_map(map, yt);
        DrawCircle((int)mp.x, (int)mp.y, 2.5f, BLUE);
        is_ray_hit_wall(&res, yt, step_x, step_y);
        sy += step_y;
      }
    }

    max++;
  }

  return res;
}

void render_map(Map map) {
  DrawRectangle(map.pos.x, map.pos.y, map.w, map.h, BLACK);

  // TODO: Do not compute cell w/h on each frame
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

  cast_ray(map, player, camera, 0.25f);
}

void update_camera(Cam *camera, Vec2 player) {
  camera->wf = vec2_add(player, camera->dir);
  camera->wl = vec2_add(camera->wf, (Vec2){camera->dir.y, -camera->dir.x});
  camera->wr = vec2_add(camera->wf, (Vec2){-camera->dir.y, camera->dir.x});
}

int main(void)
{
  Vec2 move_speed = {0.025f, 0.025f};
  float rot_speed = 0.025f;

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
    if (IsKeyDown(KEY_UP)) {
      player = vec2_add(player, vec2_mul(camera.dir, move_speed));
      update_camera(&camera, player);
    }
    if (IsKeyDown(KEY_DOWN)) {
      player = vec2_sub(player, vec2_mul(camera.dir, move_speed));
      update_camera(&camera, player);
    }

    if (IsKeyDown(KEY_LEFT)) {
      camera.dir = vec2_rotate(camera.dir, -rot_speed);
      update_camera(&camera, player);
    }
    if (IsKeyDown(KEY_RIGHT)) {
      camera.dir = vec2_rotate(camera.dir, rot_speed);
      update_camera(&camera, player);
    }

    BeginDrawing();
      ClearBackground(BACKGROUND);
      render_map(map);
      render_map_camera(map, player, camera);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
