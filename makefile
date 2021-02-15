NAME  = snake

SRCS= Snake.c Spawn.c Menu.c Info.c Deplacement.c Dessin.c Event.c

OBJS   = ${SRCS:.c=.o}

RM = rm -rf

CC  = gcc

CGRAPH = -lgraph

.c.o:	${CC} -c $< -o ${<:.c=.o} 

${NAME}:	${OBJS}
	${CC} -o ${NAME} ${OBJS} $(CGRAPH)

all:	${NAME}

clean:
	 ${RM} ${OBJS}

fclean:	clean
	${RM} ${NAME}

re:	fclean all

compress: mrpoper
	tar zcvf Snake.tar.gz

.PHONY:	clean fclean all re compress

