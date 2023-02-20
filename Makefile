# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frgojard <frgojard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/12 22:36:13 by nbled             #+#    #+#              #
#    Updated: 2023/02/20 13:57:46 by frgojard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS =	main.c \
		philo_parsing.c \
		philo_init.c \
		philo_th.c \
		philo_utils.c \
		philo_free.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
