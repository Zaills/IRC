# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:42:21 by rvinour           #+#    #+#              #
#    Updated: 2023/12/14 17:08:42 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++

CXXFLAG = -Wall -Werror -Wextra -std=c++98 -g

NAME = ircserv

SRC = main.cpp \
		Socket.cpp\
		Server.cpp\
		Chanel.cpp\
		cmd/CMD_Utils.cpp\
		cmd/Join.cpp\
		cmd/Topic.cpp\
		cmd/Kick.cpp\
		cmd/Privmsg.cpp\
		cmd/Nick.cpp\
		cmd/Pass.cpp\
		cmd/User.cpp\
		cmd/Mode.cpp\
		cmd/Invite.cpp

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
