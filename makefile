GRAPH_NAME := snake
RAYLIB_NAME := snake-raylib
TEST_NAME := test_core

CC ?= gcc
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=gnu11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g
LDFLAGS ?=

GRAPH_LIBS := -lgraph
RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS ?= $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(strip $(RAYLIB_LIBS)),)
RAYLIB_LIBS := -lraylib -lm
endif

CORE_SRCS := src/board.c src/snake.c src/game.c src/score.c
COMMON_SRCS := src/main.c
GRAPH_SRCS := src/input_graph.c src/renderer_graph.c src/menu_graph.c
RAYLIB_SRCS := src/input_raylib.c src/renderer_raylib.c src/menu_raylib.c

CORE_OBJS := $(CORE_SRCS:.c=.o)
COMMON_OBJS := $(COMMON_SRCS:.c=.o)
GRAPH_OBJS := $(GRAPH_SRCS:.c=.o)
RAYLIB_OBJS := $(RAYLIB_SRCS:.c=.o)

TEST_SRCS := tests/test_core.c src/board.c src/snake.c src/game.c

.PHONY: all graph raylib clean fclean re debug debug-raylib test compress

all: graph

graph: $(GRAPH_NAME)

raylib: $(RAYLIB_NAME)

$(GRAPH_NAME): $(CORE_OBJS) $(COMMON_OBJS) $(GRAPH_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(GRAPH_LIBS)

$(RAYLIB_NAME): $(CORE_OBJS) $(COMMON_OBJS) $(RAYLIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(RAYLIB_LIBS)

src/%_raylib.o: src/%_raylib.c
	$(CC) $(CPPFLAGS) $(RAYLIB_CFLAGS) $(CFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_NAME): $(TEST_SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SRCS) -o $@

test: $(TEST_NAME)
	./$(TEST_NAME)

debug: CFLAGS += -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS += -fsanitize=address,undefined
debug: fclean $(GRAPH_NAME)

debug-raylib: CFLAGS += -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug-raylib: LDFLAGS += -fsanitize=address,undefined
debug-raylib: fclean $(RAYLIB_NAME)

clean:
	$(RM) $(CORE_OBJS) $(COMMON_OBJS) $(GRAPH_OBJS) $(RAYLIB_OBJS) $(TEST_NAME)

fclean: clean
	$(RM) $(GRAPH_NAME) $(RAYLIB_NAME)

re: fclean all

compress:
	tar czvf Snake.tar.gz src include tests makefile README.md AUDIT.md \
		$(wildcard *.png *.jpg *.gif)
