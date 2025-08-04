# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/18 10:20:20 by mdziadko          #+#    #+#              #
#    Updated: 2025/08/04 18:02:14 by mdziadko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 								   VARIABLES								   #
# **************************************************************************** #

NAME		= minishell
CC 			= cc
CFLAGS		= -Wall -Wextra -Werror -g3 -fsanitize=address
AR			= ar rcs
RM			= rm -f
# MINI_FLAGS	= -lreadline #-lhistory UNCOMMENT ON LINUX

UNAME_S			= $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RB			= $(shell brew --prefix readline)
  MINI_CFLAGS	= -I$(RB)/include
  MINI_LDFLAGS	= -L$(RB)/lib
  MINI_LIBS		= -lreadline
else
  MINI_CFLAGS	=
  MINI_LDFLAGS	=
  MINI_LIBS		= -lreadline -lhistory
endif

# Directories
LIBFT_DIR		= libft
LIBFT_LIB		= $(LIBFT_DIR)/libft.a
OBJ_DIR			= obj

# Includes and Libraries
INCLUDES		= -I. -I$(LIBFT_DIR) $(MINI_CFLAGS)
LIBS			= $(LIBFT_LIB)

# **************************************************************************** #
# 									FILES									   #
# **************************************************************************** #

SRCS		= main.c \
			init.c \
			signals.c \
			lexer.c lexer_extract.c lexer_help.c\
			validator.c \
			expander.c expander_extract.c \
			parser.c parser_help.c parser_process.c \
			heredoc.c \
			redirections.c \
			builtins.c \
			cleanup_0.c cleanup_1.c\
			print.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
HEADERS		= minishell.h

# **************************************************************************** #
# 									RULES									   #
# **************************************************************************** #

all: $(LIBFT_LIB) $(NAME) 

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(MINI_FLAGS) -o $(NAME)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(MINI_LDFLAGS) $(MINI_LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re