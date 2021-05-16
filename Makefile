TARGET := build/bin/snake
TEST_TARGET := build/bin/test_core
SANITIZE_TEST_TARGET := build/bin/test_core_sanitize

CC ?= gcc
PKG_CONFIG ?= pkg-config

CPPFLAGS := -Iinclude
BASE_CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
	-Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wundef -g
CFLAGS := $(BASE_CFLAGS) -O2 $(EXTRA_CFLAGS)
LDFLAGS := $(EXTRA_LDFLAGS)

RAYLIB_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)
ifeq ($(strip $(RAYLIB_LIBS)),)
RAYLIB_LIBS := -lraylib -lm
endif

CORE_SRCS := \
	src/core/board.c \
	src/core/snake.c \
	src/core/game.c \
	src/core/level.c \
	src/core/layout.c

APP_SRCS := \
	src/main.c \
	src/app/app.c \
	src/platform/input.c \
	src/persistence/score.c \
	src/ui/assets.c \
	src/ui/menu.c \
	src/ui/renderer.c

SRCS := $(CORE_SRCS) $(APP_SRCS)
OBJS := $(patsubst %.c,build/obj/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

TEST_SRCS := \
	tests/test_main.c \
	tests/test_board.c \
	tests/test_snake.c \
	tests/test_game.c \
	tests/test_layout.c

ASSET_FILES := $(shell find assets -type f 2>/dev/null)
ASSET_STAMP := build/obj/.assets-copied

.PHONY: all run test sanitize-test debug clean format dist

all: $(TARGET)

$(TARGET): $(OBJS) $(ASSET_STAMP)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(RAYLIB_LIBS)

build/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(RAYLIB_CFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(ASSET_STAMP): $(ASSET_FILES)
	@mkdir -p build/bin build/obj
	@rm -rf build/bin/assets
	@cp -R assets build/bin/assets
	@touch $@

$(TEST_TARGET): $(TEST_SRCS) $(CORE_SRCS)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(BASE_CFLAGS) -O2 $(TEST_SRCS) $(CORE_SRCS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

sanitize-test:
	@mkdir -p $(dir $(SANITIZE_TEST_TARGET))
	$(CC) $(CPPFLAGS) $(BASE_CFLAGS) -O1 -fsanitize=address,undefined \
		-fno-omit-frame-pointer $(TEST_SRCS) $(CORE_SRCS) -o $(SANITIZE_TEST_TARGET)
	./$(SANITIZE_TEST_TARGET)

debug:
	$(MAKE) clean
	$(MAKE) EXTRA_CFLAGS="-O0 -fsanitize=address,undefined -fno-omit-frame-pointer" \
		EXTRA_LDFLAGS="-fsanitize=address,undefined" all

run: all
	./$(TARGET)

format:
	@command -v clang-format >/dev/null || { echo "clang-format est requis"; exit 1; }
	clang-format -i $$(find src include tests -type f \( -name '*.c' -o -name '*.h' \))

clean:
	rm -rf build

dist:
	@mkdir -p build
	tar czf build/snake-source.tar.gz src include tests assets docs data/.gitkeep \
		Makefile README.md .clang-format .editorconfig .gitignore

-include $(DEPS)
