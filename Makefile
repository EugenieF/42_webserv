NAME    		= webserv

SRCS_DIR		= ./srcs

SRCS			= main.cpp \
				Token.cpp \
				Lexer.cpp \
				Parser.cpp \
				Block.cpp

OBJS_DIR		= ./objs

OBJS			= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

DEP				= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))

CXX				= c++

# CXXFLAGS		= -Wall -Wextra -Werror -std=c++98 -MMD -MP -I$(INCLUDE)
CXXFLAGS		= -Wall -Wextra -Werror -MMD -MP -I$(INCLUDE)

RM				= rm -rf

INCLUDE			= ./incs

$(NAME):	$(OBJS)
		@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
		@echo "$(CUT)$(GREEN)✔ $(NAME) compiled$(RESET)"

-include $(DEP)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
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
