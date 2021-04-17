NAME := snake
TEST_NAME := test_core

CC ?= gcc
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=gnu11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g
LDFLAGS ?=
LDLIBS := -lgraph

CORE_SRCS := src/board.c src/snake.c src/game.c src/score.c
GRAPH_SRCS := src/main.c src/input_graph.c src/renderer_graph.c src/menu_graph.c
SRCS := $(CORE_SRCS) $(GRAPH_SRCS)
OBJS := $(SRCS:.c=.o)

TEST_SRCS := tests/test_core.c src/board.c src/snake.c src/game.c

.PHONY: all clean fclean re debug test compress

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_NAME): $(TEST_SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SRCS) -o $@

test: $(TEST_NAME)
	./$(TEST_NAME)

debug: CFLAGS += -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS += -fsanitize=address,undefined
debug: fclean $(NAME)

clean:
	$(RM) $(OBJS) $(TEST_NAME)

fclean: clean
	$(RM) $(NAME)

re: fclean all

compress:
	tar czvf Snake.tar.gz src include tests makefile README.md AUDIT.md \
		$(wildcard *.png *.jpg *.gif)
