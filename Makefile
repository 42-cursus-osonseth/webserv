# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arlarzil <arlarzil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/06 20:28:41 by arlarzil          #+#    #+#              #
#    Updated: 2024/07/17 14:58:23 by arlarzil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		=	src/main.cpp				\
			src/sampleConfig.cpp		\
			src/server.cpp				\
			src/utils.cpp				\
			src/errcodes.cpp			\
			src/mime.cpp				\
			src/requests/requests.cpp	\
			src/requests/get.cpp		\
			src/requests/post.cpp		\
			src/requests/delete.cpp		\

OBJ		=	$(SRC:.cpp=.o)

NAME	=	webserv.out

CC		= c++

CFLAGS 	= -Wall -Wextra -Werror -std=c++98 -pedantic -Iinclude

all: $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

auteur:
	echo $(USER) > auteur

.PHONY: fclean all re clean
