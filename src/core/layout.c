#include "snake/layout.h"

static int clamp_int(int value, int minimum, int maximum)
{
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

WindowSize layout_window_size(int monitor_width, int monitor_height)
{
    WindowSize window;

    window.width = clamp_int(monitor_width - LAYOUT_MARGIN_HORIZONTAL,
                             LAYOUT_MIN_WIDTH,
                             LAYOUT_MAX_WIDTH);
    window.height = clamp_int(monitor_height - LAYOUT_MARGIN_VERTICAL,
                              LAYOUT_MIN_HEIGHT,
                              LAYOUT_MAX_HEIGHT);
    return window;
}

BoardGeometry layout_board_geometry(WindowSize window, int rows, int columns, int hud_height)
{
    BoardGeometry geometry = {LAYOUT_CELL_SIZE_DEFAULT, 0, 0};
    int available_width;
    int available_height;
    int horizontal_cell_size;
    int vertical_cell_size;

    if (rows <= 0 || columns <= 0) {
        return geometry;
    }

    available_width = window.width;
    available_height = window.height - hud_height;
    if (available_height < 0) {
        available_height = 0;
    }

    horizontal_cell_size = available_width / columns;
    vertical_cell_size = available_height / rows;
    if (vertical_cell_size < horizontal_cell_size) {
        horizontal_cell_size = vertical_cell_size;
    }
    geometry.cell_size = clamp_int(horizontal_cell_size, LAYOUT_CELL_SIZE_MIN, LAYOUT_CELL_SIZE_MAX);

    geometry.origin_x = (window.width - (columns * geometry.cell_size)) / 2;
    if (geometry.origin_x < 0) {
        geometry.origin_x = 0;
    }
    geometry.origin_y = hud_height + (available_height - (rows * geometry.cell_size)) / 2;
    if (geometry.origin_y < hud_height) {
        geometry.origin_y = hud_height;
    }
    return geometry;
}

UiTransform layout_ui_transform(WindowSize window, int design_width, int design_height)
{
    UiTransform transform = {1.0f, 0.0f, 0.0f};
    float horizontal_scale;
    float vertical_scale;

    if (design_width <= 0 || design_height <= 0) {
        return transform;
    }

    horizontal_scale = (float)window.width / (float)design_width;
    vertical_scale = (float)window.height / (float)design_height;
    transform.scale = (vertical_scale < horizontal_scale) ? vertical_scale : horizontal_scale;
    transform.offset_x = ((float)window.width - ((float)design_width * transform.scale)) * 0.5f;
    transform.offset_y = ((float)window.height - ((float)design_height * transform.scale)) * 0.5f;
    return transform;
}
