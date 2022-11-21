SHELL				= /bin/sh

UNAME				= $(shell uname)

CXX					= c++

CXXFLAGS			= -Wall -Wextra -Werror -std=c++98 $(INCLUDE)

NAME				= webserv_gtests

INCLUDE				= -Igtest_incs/ -pthread -I$(WEBSERV_INCS)

WEBSERV_INCS		= ../incs

VALGRIND_FILE		= valgrind-out.txt

ifeq ($(UNAME), Darwin)
	LIBGTEST 		= gtest_incs/libgtest_mac.a
else
	LIBGTEST 		= gtest_incs/libgtest.a
endif
CXXFLAGS			+= -std=c++11 -Wno-deprecated-copy

MAIN				= $(SRCS_DIR)/gtest_main.cpp

SRCS_DIR			= srcs
SRCS				= webservTests.cpp
OBJS_DIR			= objs
OBJS 				= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))
DEP					= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))

WEBSERV_OBJS_DIR	= ../objs
WEBSERV_SRCS_DIR	= ../srcs
WEBSERV_SRCS		= Token.cpp Lexer.cpp Parser.cpp Block.cpp Webserv.cpp
WEBSERV_OBJS		= $(addprefix $(WEBSERV_OBJS_DIR)/, $(WEBSERV_SRCS:.cpp=.o))
WEBSERV_DEP			= $(addprefix $(WEBSERV_OBJS_DIR)/, $(WEBSERV_SRCS:.cpp=.d))

RM					= rm -rf

all: $(NAME)

-include $(WEBSERV_DEP)
$(WEBSERV_OBJS_DIR)/%.o: $(WEBSERV_SRCS_DIR)/%.cpp
	@mkdir -p $(WEBSERV_OBJS_DIR)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(NAME): $(WEBSERV_OBJS) $(OBJS)
	@echo "$(YELLOW)🚧 Linking $(NAME)...$(RESET)"
	@chmod 000 testFiles/invalid/noPermission.conf
	@$(CXX) $(CXXFLAGS) $(MAIN) $(WEBSERV_OBJS) $(OBJS) $(LIBGTEST) -o $(NAME)
	@echo "$(CUT)$(GREEN)✔ $(NAME) compiled$(RESET)"

valgrind: $(NAME)
	@echo "$(CUT)$(BLUE)→ execute $(NAME) with valgrind :$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(VALGRIND_FILE) ./$(NAME)
	@cat $(VALGRIND_FILE)

clean:
	@$(RM) $(OBJS_DIR)
	@$(RM) $(VALGRIND_FILE) 
	@echo "$(CUT)$(RED)[deleting gtests objs]$(RESET)"

fclean: clean
	@chmod 777 testFiles/invalid/noPermission.conf
	@$(RM) $(NAME)
	@echo "$(CUT)$(RED)[deleting $(NAME)]$(RESET)"

re: fclean all

.PHONY:		all clean fclean re

RESET		= \033[0m
RED			= \033[0;31m
GREEN		= \033[1;32m
YELLOW		= \033[0;33m
BLUE		= \033[1;34m
WHITE		= \033[0;37m
ORANGE		= \033[0;38;5;208m
UP			= \033[A
CUT			= \033[K