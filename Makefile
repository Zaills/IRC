# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:42:21 by rvinour           #+#    #+#              #
#    Updated: 2023/12/06 12:00:26 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#    Updated: 2023/12/05 14:36:31 by gouz             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++

CXXFLAG = -Wall -Werror -Wextra -std=c++98 -g

NAME = ircserv

SRC = main.cpp \
		Socket.cpp\
		Server.cpp\
		Chanel.cpp\
		Join.cpp

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
