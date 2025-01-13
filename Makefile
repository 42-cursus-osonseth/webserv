# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mekherbo <mekherbo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 16:21:25 by mekherbo          #+#    #+#              #
#    Updated: 2025/01/13 01:28:38 by mekherbo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = main.cpp \
	checkFiles.cpp \
	Configuration.cpp

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
PURPLE= \033[38;2;255;105;180m
RESET= \033[0m

OBJ = $(SRC:.cpp=.o)

CC = c++

CPPFLAGS = -g3 -Wall -Wextra -Werror -std=c++98

RM = rm -f

all : $(NAME)

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o)
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean :
	$(RM) $(NAME) $(OBJ)
	@echo "$(BLUE)object files cleaned!$(DEF_COLOR)"

fclean : clean
	$(RM) $(NAME)
	@echo "$(CYAN)executable has been cleaned!$(DEF_COLOR)"

re : fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything$(DEF_COLOR)"

.PHONY : all clean fclean re