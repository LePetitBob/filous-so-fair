# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/09 17:31:00 by vduriez           #+#    #+#              #
#    Updated: 2022/04/04 22:18:02 by vduriez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g
NAME = philo
INCLUDES = philosophers.h
SRCS =	main.c check_args.c ft_str.c ft_atoi.c \
		check_routine.c cl_creation.c norme_eating.c \
		routine_checks.c routine_eat.c

OBJS = $(SRCS:.c=.o)

all:		$(NAME) bonus

$(NAME):	$(INCLUDES) $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus
