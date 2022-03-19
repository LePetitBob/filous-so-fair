# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/09 17:31:00 by vduriez           #+#    #+#              #
#    Updated: 2022/03/19 18:43:02 by vduriez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = gcc
FLAGS = -Wall -Wextra -Werror -pthread -g3 -fsanitize=thread
NAME = philo
INCLUDES = philosophers.h
SRCS =	main.c check_args.c ft_str.c ft_atoi.c \
		check_routine.c

OBJ = $(SRCS:.c=.o)

all:		$(NAME) bonus

$(NAME):	$(INCLUDES) $(OBJS)
		$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus