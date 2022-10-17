NAME    		= webserv

SRCS_DIR		= ./srcs

SRCS			= main.cpp \
				utils.cpp \
				Token.cpp \
				Lexer.cpp \
				Parser.cpp \
				Block.cpp \
				Webserv.cpp \
				Request.cpp \
				Response.cpp \
				EpollInstance.cpp \
				signal.cpp \
				TcpSocket.cpp \
				Server.cpp \
				Client.cpp \
				StatusCode.cpp \
				MimeType.cpp \
				HttpMethod.cpp \
				Autoindex.cpp \
				CgiHandler.cpp \
				generateErrorPage.cpp \
				Env.cpp

OBJS_DIR		= ./objs

OBJS			= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

vpath			%.cpp $(shell find $(SRCS_DIR) -type d)

DEP				= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))

CXX				= c++

# EXTRA			= -Werror=implicit-fallthrough=0
CXXFLAGS		= -Wall -Wextra -Werror -g $(EXTRA) -std=c++98 -MMD -MP -I$(INCLUDE)

RM				= rm -rf

INCLUDE			= ./incs


$(NAME):	$(OBJS)
		@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
		@echo "$(CUT)$(GREEN)✔ $(NAME) compiled$(RESET)"

-include $(DEP)
$(OBJS_DIR)/%.o: %.cpp
		@mkdir -p $(OBJS_DIR)
		@$(CXX) $(CXXFLAGS) -c $< -o $@
		@echo "$(CUT)$(BLUE)$(CXX) $(CXXFLAGS) $(RESET)$(notdir $@)"
		@printf "$(UP)"

all:	$(NAME)

clean:
		@$(RM) $(OBJS_DIR)
		@echo "$(CUT)[deleting objs]"
		@printf "$(UP)"

fclean:		clean
		@$(RM) $(NAME)
		@echo "$(CUT)[deleting $(NAME)]"

re:			fclean all

test:	all
		valgrind -s --track-fds=yes --leak-check=full ./$(NAME)

.PHONY:		all clean fclean re

RESET		= \033[0m
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
WHITE		= \033[1;37m
ORANGE		= \033[0;38;5;208m
UP			= \033[A
CUT			= \033[K
