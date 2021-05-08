#include "snake/menu.h"

#include "assets.h"

#include <raylib.h>
#include <stdio.h>

#define MENU_WIDTH 720
#define MENU_HEIGHT 760

typedef enum MenuAction {
    MENU_ACTION_NONE = 0,
    MENU_ACTION_PLAY,
    MENU_ACTION_QUIT
} MenuAction;

typedef struct MenuState {
    GameConfig config;
    int last_score;
    int highscore;
    bool options;
} MenuState;

static bool button(Rectangle bounds, const char *label, bool selected)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, bounds);
    Color fill = selected ? ORANGE : (hover ? LIGHTGRAY : RAYWHITE);
    int font_size = 22;
    int text_width = MeasureText(label, font_size);

    DrawRectangleRec(bounds, fill);
    DrawRectangleLinesEx(bounds, 2.0f, DARKGREEN);
    DrawText(label,
             (int)(bounds.x + (bounds.width - (float)text_width) / 2.0f),
             (int)(bounds.y + (bounds.height - (float)font_size) / 2.0f),
             font_size,
             BLACK);

    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void draw_background(void)
{
    const AppAssets *assets = assets_get();

    ClearBackground((Color){239, 246, 226, 255});
    if (IsTextureValid(assets->menu_background)) {
        Rectangle source = {0.0f,
                            0.0f,
                            (float)assets->menu_background.width,
                            (float)assets->menu_background.height};
        Rectangle destination = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};

        DrawTexturePro(assets->menu_background,
                       source,
                       destination,
                       (Vector2){0.0f, 0.0f},
                       0.0f,
                       WHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.84f));
    }
}

static void draw_title(const char *title)
{
    int font_size = 42;
    int width = MeasureText(title, font_size);

    DrawText(title, (MENU_WIDTH - width) / 2, 35, font_size, DARKGREEN);
}

static MenuAction draw_main_menu(MenuState *state)
{
    char text[128];

    draw_title("Snake");
    (void)snprintf(text, sizeof(text), "Dernier score : %d", state->last_score);
    DrawText(text, 255, 105, 20, DARKGRAY);
    (void)snprintf(text, sizeof(text), "Meilleur score : %d", state->highscore);
    DrawText(text, 245, 132, 20, DARKGREEN);

    if (button((Rectangle){255.0f, 220.0f, 210.0f, 58.0f}, "Jouer", false)) {
        return MENU_ACTION_PLAY;
    }
    if (button((Rectangle){255.0f, 300.0f, 210.0f, 58.0f}, "Options", false)) {
        state->options = true;
        return MENU_ACTION_NONE;
    }
    if (button((Rectangle){255.0f, 380.0f, 210.0f, 58.0f}, "Fermer", false)) {
        return MENU_ACTION_QUIT;
    }

    DrawText("Fleches ou WASD en jeu - Echap pour revenir au menu", 130, 710, 16, DARKGRAY);
    return MENU_ACTION_NONE;
}

static void adjust_value_control(const char *label,
                                 int y,
                                 int *value,
                                 int step,
                                 int minimum,
                                 int maximum)
{
    char value_text[32];

    DrawText(label, 95, y + 10, 22, BLACK);

    if (button((Rectangle){360.0f, (float)y, 55.0f, 45.0f}, "-", false) &&
        *value - step >= minimum) {
        *value -= step;
    }

    (void)snprintf(value_text, sizeof(value_text), "%d", *value);
    DrawText(value_text, 445, y + 10, 22, DARKGREEN);

    if (button((Rectangle){520.0f, (float)y, 55.0f, 45.0f}, "+", false) &&
        *value + step <= maximum) {
        *value += step;
    }
}

static MenuAction draw_options_menu(MenuState *state)
{
    draw_title("Options");

    DrawText("Difficulte", 70, 95, 24, DARKGRAY);
    if (button((Rectangle){70.0f, 135.0f, 160.0f, 48.0f},
               "Facile",
               state->config.speed_coeff == 1)) {
        state->config.speed_coeff = 1;
    }
    if (button((Rectangle){280.0f, 135.0f, 160.0f, 48.0f},
               "Moyen",
               state->config.speed_coeff == 5)) {
        state->config.speed_coeff = 5;
    }
    if (button((Rectangle){490.0f, 135.0f, 160.0f, 48.0f},
               "Difficile",
               state->config.speed_coeff == 10)) {
        state->config.speed_coeff = 10;
    }

    DrawText("Mode", 70, 205, 24, DARKGRAY);
    if (button((Rectangle){70.0f, 245.0f, 160.0f, 48.0f},
               "Classique",
               state->config.mode == GAME_WITH_WALLS)) {
        state->config.mode = GAME_WITH_WALLS;
    }
    if (button((Rectangle){280.0f, 245.0f, 160.0f, 48.0f},
               "Portails",
               state->config.mode == GAME_WITH_PORTALS)) {
        state->config.mode = GAME_WITH_PORTALS;
    }
    if (button((Rectangle){490.0f, 245.0f, 160.0f, 48.0f},
               "Infini",
               state->config.mode == GAME_INFINITE)) {
        state->config.mode = GAME_INFINITE;
    }

    adjust_value_control("Lignes", 335, &state->config.rows, 5, 24, 100);
    adjust_value_control("Colonnes", 395, &state->config.columns, 5, 30, 150);
    adjust_value_control("Pommes", 455, &state->config.apple_count, 1, 1, 100);
    adjust_value_control("Obstacles", 515, &state->config.trap_count, 1, 0, 100);
    adjust_value_control("Taille", 575, &state->config.snake_length, 1, 3, 100);

    if (button((Rectangle){70.0f, 675.0f, 180.0f, 50.0f}, "Retour", false)) {
        state->options = false;
    }
    if (button((Rectangle){470.0f, 675.0f, 180.0f, 50.0f}, "Jouer", false)) {
        return MENU_ACTION_PLAY;
    }

    return MENU_ACTION_NONE;
}

bool menu_run(int last_score, int highscore, GameConfig *out_config)
{
    MenuState state;

    if (out_config == NULL || !IsWindowReady()) {
        return false;
    }

    state.config = *out_config;
    game_config_sanitize(&state.config);
    state.last_score = last_score;
    state.highscore = highscore;
    state.options = false;

    SetWindowSize(MENU_WIDTH, MENU_HEIGHT);
    SetWindowTitle("Snake - Menu");

    while (!WindowShouldClose()) {
        MenuAction action;

        if (IsKeyPressed(KEY_ESCAPE)) {
            if (state.options) {
                state.options = false;
            } else {
                return false;
            }
        }

        BeginDrawing();
        draw_background();
        action = state.options ? draw_options_menu(&state) : draw_main_menu(&state);
        EndDrawing();

        if (action == MENU_ACTION_PLAY) {
            game_config_sanitize(&state.config);
            *out_config = state.config;
            return true;
        }
        if (action == MENU_ACTION_QUIT) {
            return false;
        }
    }

    return false;
}
