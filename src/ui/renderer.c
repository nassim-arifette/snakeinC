#include "snake/renderer.h"

#include "assets.h"

#include <raylib.h>
#include <stdio.h>

#define DEFAULT_CELL_SIZE 20
#define MIN_CELL_SIZE 6
#define HUD_HEIGHT 96
#define WINDOW_MARGIN 48

static int cell_size = DEFAULT_CELL_SIZE;

static const Color BOARD_LIGHT = {170, 215, 80, 255};
static const Color BOARD_DARK = {162, 209, 73, 255};
static const Color WALL_COLOR = {87, 138, 52, 255};
static const Color HUD_COLOR = {232, 242, 216, 255};

static Color board_color(int row, int column)
{
    return ((row % 2) == (column % 2)) ? BOARD_LIGHT : BOARD_DARK;
}

static void draw_texture_in_cell(Texture2D texture,
                                 int row,
                                 int column,
                                 Color fallback)
{
    float x = (float)(column * cell_size);
    float y = (float)(HUD_HEIGHT + row * cell_size);
    Rectangle destination = {x, y, (float)cell_size, (float)cell_size};

    if (IsTextureValid(texture)) {
        Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        DrawTexturePro(texture, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        DrawRectangleRec(destination, fallback);
    }
}

static void draw_cell(const Game *game, int row, int column)
{
    const AppAssets *assets = assets_get();
    CellType cell = board_get(&game->board, row, column);
    int x = column * cell_size;
    int y = HUD_HEIGHT + row * cell_size;

    switch (cell) {
    case CELL_WALL:
        DrawRectangle(x, y, cell_size, cell_size, WALL_COLOR);
        break;
    case CELL_EMPTY:
        DrawRectangle(x, y, cell_size, cell_size, board_color(row, column));
        break;
    case CELL_SNAKE:
        DrawRectangle(x, y, cell_size, cell_size, BLUE);
        break;
    case CELL_APPLE:
        DrawRectangle(x, y, cell_size, cell_size, board_color(row, column));
        draw_texture_in_cell(assets->apple, row, column, RED);
        break;
    case CELL_TRAP:
        DrawRectangle(x, y, cell_size, cell_size, board_color(row, column));
        draw_texture_in_cell(assets->bomb, row, column, BLACK);
        break;
    case CELL_PORTAL_TOP:
    case CELL_PORTAL_BOTTOM:
    case CELL_PORTAL_LEFT:
    case CELL_PORTAL_RIGHT:
    case CELL_PORTAL_WRAP_LEFT:
    case CELL_PORTAL_WRAP_RIGHT:
        DrawRectangle(x, y, cell_size, cell_size, board_color(row, column));
        draw_texture_in_cell(assets->portal, row, column, PURPLE);
        break;
    default:
        DrawRectangle(x, y, cell_size, cell_size, MAGENTA);
        break;
    }
}

static void draw_hud(const Game *game, int highscore)
{
    const AppAssets *assets = assets_get();
    char buffer[64];
    int width = GetScreenWidth();
    int font_size = width < 720 ? 16 : 20;

    DrawRectangle(0, 0, width, HUD_HEIGHT, HUD_COLOR);
    DrawLine(0, HUD_HEIGHT - 1, width, HUD_HEIGHT - 1, WALL_COLOR);

    if (IsTextureValid(assets->apple)) {
        Rectangle source = {0.0f, 0.0f, (float)assets->apple.width, (float)assets->apple.height};
        Rectangle destination = {16.0f, 24.0f, 36.0f, 36.0f};
        DrawTexturePro(assets->apple, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }
    (void)snprintf(buffer, sizeof(buffer), "Pommes: %d", game->apples_remaining);
    DrawText(buffer, 60, 32, font_size, BLACK);

    if (IsTextureValid(assets->trophy)) {
        Rectangle source = {0.0f, 0.0f, (float)assets->trophy.width, (float)assets->trophy.height};
        Rectangle destination = {(float)(width / 4), 24.0f, 36.0f, 36.0f};
        DrawTexturePro(assets->trophy, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }
    (void)snprintf(buffer, sizeof(buffer), "Record: %d", highscore);
    DrawText(buffer, width / 4 + 44, 32, font_size, BLACK);

    (void)snprintf(buffer, sizeof(buffer), "Score: %d", game->score);
    DrawText(buffer, width / 2 + 20, 32, font_size, BLACK);

    (void)snprintf(buffer, sizeof(buffer), "Niveau: %d", game->level);
    DrawText(buffer, width * 3 / 4, 32, font_size, BLACK);

    if (!game->started && !game_is_over(game)) {
        const char *message = "Fleches/WASD ou Espace pour commencer";
        int message_width = MeasureText(message, 16);
        DrawText(message, (width - message_width) / 2, 70, 16, DARKGRAY);
    }
}

bool renderer_init(void)
{
    return assets_load();
}

void renderer_shutdown(void)
{
    assets_unload();
}

void renderer_prepare_game(const Game *game)
{
    int monitor;
    int max_width;
    int max_height;
    int horizontal_cell_size;
    int vertical_cell_size;
    int window_width;
    int window_height;

    if (game == NULL || !IsWindowReady()) {
        return;
    }

    monitor = GetCurrentMonitor();
    max_width = GetMonitorWidth(monitor) - WINDOW_MARGIN;
    max_height = GetMonitorHeight(monitor) - HUD_HEIGHT - WINDOW_MARGIN;
    horizontal_cell_size = max_width / game->board.columns;
    vertical_cell_size = max_height / game->board.rows;

    cell_size = DEFAULT_CELL_SIZE;
    if (horizontal_cell_size < cell_size) {
        cell_size = horizontal_cell_size;
    }
    if (vertical_cell_size < cell_size) {
        cell_size = vertical_cell_size;
    }
    if (cell_size < MIN_CELL_SIZE) {
        cell_size = MIN_CELL_SIZE;
    }

    window_width = game->board.columns * cell_size;
    window_height = HUD_HEIGHT + game->board.rows * cell_size;
    SetWindowSize(window_width, window_height);
    SetWindowTitle("Snake");
}

void renderer_draw_game(const Game *game, int highscore)
{
    int row;
    int column;

    if (game == NULL || !IsWindowReady()) {
        return;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_hud(game, highscore);

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            draw_cell(game, row, column);
        }
    }

    EndDrawing();
}
