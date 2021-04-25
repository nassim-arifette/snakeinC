#include "renderer.h"

#include <raylib.h>
#include <stdio.h>

#define CELL_SIZE 20

typedef struct RaylibAssets {
    Texture2D apple;
    Texture2D apple_top;
    Texture2D trophy;
    Texture2D portal;
    Texture2D bomb;
} RaylibAssets;

static RaylibAssets assets;

static const Color BOARD_LIGHT = {170, 215, 80, 255};
static const Color BOARD_DARK = {162, 209, 73, 255};
static const Color WALL_COLOR = {87, 138, 52, 255};

static Color board_color(int row, int column)
{
    return ((row % 2) == (column % 2)) ? BOARD_LIGHT : BOARD_DARK;
}

static void draw_texture_in_cell(Texture2D texture, int row, int column, Color fallback)
{
    Rectangle destination = {
        (float)(CELL_SIZE * column),
        (float)(CELL_SIZE * row),
        (float)CELL_SIZE,
        (float)CELL_SIZE
    };

    if (texture.id != 0U) {
        Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        DrawTexturePro(texture, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        DrawRectangleRec(destination, fallback);
    }
}

static void draw_cell(const Game *game, int row, int column)
{
    CellType cell = board_get(&game->board, row, column);
    int x = CELL_SIZE * column;
    int y = CELL_SIZE * row;

    switch (cell) {
    case CELL_WALL:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, WALL_COLOR);
        break;
    case CELL_EMPTY:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, board_color(row, column));
        break;
    case CELL_SNAKE:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, BLUE);
        break;
    case CELL_APPLE:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, board_color(row, column));
        draw_texture_in_cell(assets.apple, row, column, RED);
        break;
    case CELL_TRAP:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, board_color(row, column));
        draw_texture_in_cell(assets.bomb, row, column, BLACK);
        break;
    case CELL_PORTAL_TOP:
    case CELL_PORTAL_BOTTOM:
    case CELL_PORTAL_LEFT:
    case CELL_PORTAL_RIGHT:
    case CELL_PORTAL_WRAP_LEFT:
    case CELL_PORTAL_WRAP_RIGHT:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, board_color(row, column));
        draw_texture_in_cell(assets.portal, row, column, PURPLE);
        break;
    default:
        DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, MAGENTA);
        break;
    }
}

static void draw_hud(const Game *game, int highscore)
{
    char buffer[100];

    DrawRectangle(0, 0, game->board.columns * CELL_SIZE, 110, WALL_COLOR);

    if (assets.apple_top.id != 0U) {
        DrawTextureEx(assets.apple_top, (Vector2){10.0f, 30.0f}, 0.0f, 1.0f, WHITE);
    }
    snprintf(buffer, sizeof(buffer), "Pommes: %d", game->apples_remaining);
    DrawText(buffer, 50, 48, 22, BLACK);

    if (assets.trophy.id != 0U) {
        DrawTextureEx(assets.trophy, (Vector2){190.0f, 30.0f}, 0.0f, 1.0f, WHITE);
    }
    snprintf(buffer, sizeof(buffer), "Record: %d", highscore);
    DrawText(buffer, 235, 48, 22, BLACK);

    snprintf(buffer, sizeof(buffer), "Score: %d", game->score);
    DrawText(buffer, 410, 48, 22, BLACK);

    if (game->mode == GAME_WITH_WALLS || game->mode == GAME_WITH_PORTALS) {
        snprintf(buffer, sizeof(buffer), "Niveau: %d", game->level);
        DrawText(buffer, 560, 48, 22, BLACK);
    }
}

void renderer_open(const Game *game)
{
    if (game == NULL) {
        return;
    }

    InitWindow(CELL_SIZE * game->board.columns,
               CELL_SIZE * game->board.rows,
               "Snake - raylib");

    assets.apple = LoadTexture("pomme.png");
    assets.apple_top = LoadTexture("pommetop.png");
    assets.trophy = LoadTexture("trophee.png");
    assets.portal = LoadTexture("portal.png");
    assets.bomb = LoadTexture("bomb.png");
}

void renderer_draw(const Game *game, int highscore)
{
    int row;
    int column;

    if (game == NULL || !IsWindowReady()) {
        return;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            draw_cell(game, row, column);
        }
    }

    draw_hud(game, highscore);
    EndDrawing();
}

static void unload_if_ready(Texture2D texture)
{
    if (texture.id != 0U) {
        UnloadTexture(texture);
    }
}

void renderer_close(void)
{
    if (!IsWindowReady()) {
        return;
    }

    unload_if_ready(assets.apple);
    unload_if_ready(assets.apple_top);
    unload_if_ready(assets.trophy);
    unload_if_ready(assets.portal);
    unload_if_ready(assets.bomb);
    assets = (RaylibAssets){0};

    CloseWindow();
}
