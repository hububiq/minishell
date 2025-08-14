# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/18 10:20:20 by mdziadko          #+#    #+#              #
#    Updated: 2025/08/12 15:07:23 by hgatarek         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 								   VARIABLES								   #
# **************************************************************************** #

NAME		= minishell
CC 			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
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
			env.c env_help.c \
			signals.c \
			lexer.c lexer_extract.c lexer_help.c\
			validator.c \
			expander.c expander_extract.c \
			parser.c parser_help.c parser_process.c \
			heredoc.c \
			redirections.c \
			builtins.c \
			builtin_env.c builtin_export.c builtin_unset.c builtin_exit.c \
			builtin_cd.c builtin_echo.c builtin_pwd.c \
			cleanup_0.c cleanup_1.c \
			exec.c exec_help.c 
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