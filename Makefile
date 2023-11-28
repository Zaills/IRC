# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:42:21 by rvinour           #+#    #+#              #
#    Updated: 2023/11/28 17:50:00 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++

CXXFLAG = -Wall -Werror -Wextra -std=c++98

NAME = ircserv

SRC = main.cpp \
		Socket.cpp

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
