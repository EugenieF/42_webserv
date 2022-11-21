# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: etran <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/03 13:25:12 by etran             #+#    #+#              #
#    Updated: 2022/09/20 14:03:20 by etran            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Targets ===
SERVER = server
SERVER_C = server_side.cpp
SERVER_O = ${SERVER_C:.cpp=.o}

CLIENT = client
CLIENT_C = client_side.cpp
CLIENT_O = ${CLIENT_C:.cpp=.o}

CC = c++
CFLAGS = -Wall -Werror -Wextra -g -std=c++98


# === Rules ===
all: ${SERVER} ${CLIENT} clean

${SERVER}: ${SERVER_O}
	${CC} ${CFLAGS} ${SERVER_O} -o ${SERVER}
${CLIENT}: ${CLIENT_O}
	${CC} ${CFLAGS} ${CLIENT_O} -o ${CLIENT}

%.o: %.cpp
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${CLIENT_O} ${SERVER_O}
fclean: clean
	rm -f ${SERVER} ${CLIENT}
re: fclean all clean

.PHONY: all clean fclean re
