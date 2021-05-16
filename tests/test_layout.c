#include "test.h"

#include "snake/layout.h"

#include <assert.h>

static float abs_float(float value)
{
    return value < 0.0f ? -value : value;
}

static int close_enough(float left, float right)
{
    return abs_float(left - right) < 0.001f;
}

static void test_window_size(void)
{
    WindowSize window;

    window = layout_window_size(1366, 768);
    assert(window.width == 1318);
    assert(window.height == 648);

    window = layout_window_size(1920, 1080);
    assert(window.width == 1872);
    assert(window.height == 960);

    window = layout_window_size(320, 200);
    assert(window.width == LAYOUT_MIN_WIDTH);
    assert(window.height == LAYOUT_MIN_HEIGHT);

    window = layout_window_size(3840, 2160);
    assert(window.width == LAYOUT_MAX_WIDTH);
    assert(window.height == LAYOUT_MAX_HEIGHT);
}

static void test_board_geometry_large_board(void)
{
    BoardGeometry geometry = layout_board_geometry(layout_window_size(1366, 768), 40, 60, 96);

    assert(geometry.cell_size == 13);
    assert(geometry.origin_x == (1318 - (60 * 13)) / 2);
    assert(geometry.origin_y == 96 + ((648 - 96) - (40 * 13)) / 2);
}

static void test_board_geometry_small_board(void)
{
    WindowSize window = layout_window_size(1920, 1080);
    BoardGeometry geometry = layout_board_geometry(window, 5, 5, 96);

    assert(geometry.cell_size == LAYOUT_CELL_SIZE_MAX);
    assert(geometry.origin_x == (1872 - (5 * LAYOUT_CELL_SIZE_MAX)) / 2);
    assert(geometry.origin_y == 96 + ((960 - 96) - (5 * LAYOUT_CELL_SIZE_MAX)) / 2);
}

static void test_board_geometry_invalid_board(void)
{
    BoardGeometry geometry = layout_board_geometry(layout_window_size(1366, 768), 0, 0, 96);

    assert(geometry.cell_size == LAYOUT_CELL_SIZE_DEFAULT);
    assert(geometry.origin_x == 0);
    assert(geometry.origin_y == 0);
}

static void test_ui_transform(void)
{
    UiTransform transform = layout_ui_transform(layout_window_size(1366, 768), 720, 760);

    assert(close_enough(transform.scale, 648.0f / 760.0f));
    assert(close_enough(transform.offset_x, (1318.0f - (720.0f * transform.scale)) * 0.5f));
    assert(close_enough(transform.offset_y, 0.0f));

    transform = layout_ui_transform(layout_window_size(1920, 1080), 720, 760);
    assert(close_enough(transform.scale, 960.0f / 760.0f));
    assert(close_enough(transform.offset_x, (1872.0f - (720.0f * transform.scale)) * 0.5f));
    assert(close_enough(transform.offset_y, 0.0f));
}

void test_layout_suite(void)
{
    test_window_size();
    test_board_geometry_large_board();
    test_board_geometry_small_board();
    test_board_geometry_invalid_board();
    test_ui_transform();
}
