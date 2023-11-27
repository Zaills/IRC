# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvinour <rvinour@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:42:21 by rvinour           #+#    #+#              #
#    Updated: 2023/11/27 14:42:51 by rvinour          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++

CXXFLAG = -Wall -Werror -Wextra -std=c++98

NAME = IRC

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

%.o : %.cpp
	${CXX} ${CXXFLAG} -c $< -o ${<:.cpp=.o}

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAG) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
