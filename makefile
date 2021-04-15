NAME := snake

CC ?= gcc
CFLAGS ?= -std=gnu11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g
LDFLAGS ?=
LDLIBS := -lgraph

SRCS := Snake.c Spawn.c Menu.c Info.c Deplacement.c Dessin.c Event.c
OBJS := $(SRCS:.c=.o)

.PHONY: all clean fclean re debug compress

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c Snake.h
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS += -fsanitize=address,undefined
debug: fclean $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

compress:
	tar czvf Snake.tar.gz $(SRCS) Snake.h makefile *.png *.jpg *.gif
