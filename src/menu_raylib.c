#include "menu.h"

#include <raylib.h>
#include <stdio.h>

#define MENU_WIDTH 700
#define MENU_HEIGHT 900

typedef struct MenuState {
    GameConfig config;
    int last_score;
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

static void draw_title(const char *title)
{
    int font_size = 42;
    int width = MeasureText(title, font_size);

    DrawText(title, (MENU_WIDTH - width) / 2, 55, font_size, DARKGREEN);
}

static bool main_menu(MenuState *state)
{
    char score_text[100];

    draw_title("Snake");
    snprintf(score_text, sizeof(score_text), "Dernier score : %d", state->last_score);
    DrawText(score_text, 245, 155, 22, DARKGRAY);

    if (button((Rectangle){245.0f, 250.0f, 210.0f, 60.0f}, "Jouer", false)) {
        return true;
    }

    if (button((Rectangle){245.0f, 335.0f, 210.0f, 60.0f}, "Options", false)) {
        state->options = true;
    }

    if (button((Rectangle){245.0f, 420.0f, 210.0f, 60.0f}, "Fermer", false)) {
        CloseWindow();
        return false;
    }

    return false;
}

static void draw_value_control(const char *label, int value, int y, int *target, int step, int minimum)
{
    char value_text[64];

    DrawText(label, 95, y + 10, 22, BLACK);

    if (button((Rectangle){360.0f, (float)y, 55.0f, 45.0f}, "-", false)) {
        if (*target - step >= minimum) {
            *target -= step;
        }
    }

    snprintf(value_text, sizeof(value_text), "%d", value);
    DrawText(value_text, 445, y + 10, 22, DARKGREEN);

    if (button((Rectangle){520.0f, (float)y, 55.0f, 45.0f}, "+", false)) {
        *target += step;
    }
}

static bool options_menu(MenuState *state)
{
    draw_title("Options");

    DrawText("Difficulte", 70, 135, 24, DARKGRAY);
    if (button((Rectangle){70.0f, 175.0f, 160.0f, 50.0f}, "Facile", state->config.speed_coeff == 1)) {
        state->config.speed_coeff = 1;
    }
    if (button((Rectangle){270.0f, 175.0f, 160.0f, 50.0f}, "Moyen", state->config.speed_coeff == 5)) {
        state->config.speed_coeff = 5;
    }
    if (button((Rectangle){470.0f, 175.0f, 160.0f, 50.0f}, "Difficile", state->config.speed_coeff == 10)) {
        state->config.speed_coeff = 10;
    }

    DrawText("Mode", 70, 260, 24, DARKGRAY);
    if (button((Rectangle){70.0f, 300.0f, 160.0f, 50.0f}, "Avec bord", state->config.mode == GAME_WITH_WALLS)) {
        state->config.mode = GAME_WITH_WALLS;
    }
    if (button((Rectangle){270.0f, 300.0f, 160.0f, 50.0f}, "Portails", state->config.mode == GAME_WITH_PORTALS)) {
        state->config.mode = GAME_WITH_PORTALS;
    }
    if (button((Rectangle){470.0f, 300.0f, 160.0f, 50.0f}, "Infini", state->config.mode == GAME_INFINITE)) {
        state->config.mode = GAME_INFINITE;
    }

    draw_value_control("Lignes", state->config.rows, 390, &state->config.rows, 5, 20);
    draw_value_control("Colonnes", state->config.columns, 455, &state->config.columns, 5, 30);
    draw_value_control("Pommes", state->config.apple_count, 520, &state->config.apple_count, 1, 1);
    draw_value_control("Obstacles", state->config.trap_count, 585, &state->config.trap_count, 1, 0);
    draw_value_control("Taille", state->config.snake_length, 650, &state->config.snake_length, 1, 3);

    if (button((Rectangle){70.0f, 770.0f, 180.0f, 55.0f}, "Retour", false)) {
        state->options = false;
    }

    if (button((Rectangle){450.0f, 770.0f, 180.0f, 55.0f}, "Jouer", false)) {
        return true;
    }

    return false;
}

bool menu_run(int last_score, GameConfig *out_config)
{
    MenuState state;

    if (out_config == NULL) {
        return false;
    }

    game_config_default(&state.config);
    state.last_score = last_score;
    state.options = false;

    InitWindow(MENU_WIDTH, MENU_HEIGHT, "Snake - raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        bool play = false;

        BeginDrawing();
        ClearBackground((Color){239, 246, 226, 255});

        if (state.options) {
            play = options_menu(&state);
        } else {
            play = main_menu(&state);
        }

        EndDrawing();

        if (play) {
            game_config_sanitize(&state.config);
            *out_config = state.config;
            CloseWindow();
            return true;
        }

        if (!IsWindowReady()) {
            return false;
        }
    }

    CloseWindow();
    return false;
}
