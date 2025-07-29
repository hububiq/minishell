# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/18 10:20:20 by mdziadko          #+#    #+#              #
#    Updated: 2025/07/28 23:16:38 by mdziadko         ###   ########.fr        #
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
MINI_FLAGS	= -lreadline #-lhistory UNCOMMENT ON LINUX

# Directories
LIBFT_DIR		= libft
LIBFT_LIB		= $(LIBFT_DIR)/libft.a
OBJ_DIR			= obj

# Includes and Libraries
INCLUDES		= -I. -I$(LIBFT_DIR)
LIBS			= $(LIBFT_LIB)

# **************************************************************************** #
# 									FILES									   #
# **************************************************************************** #

SRCS		= main.c \
			init.c \
			signals.c \
			lexer.c \
			validator.c \
			expander.c \
			parser.c \
			heredoc.c \
			exec_prep.c \
			cleanup.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
HEADERS		= minishell.h

# **************************************************************************** #
# 									RULES									   #
# **************************************************************************** #

all: $(LIBFT_LIB) $(NAME) 

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(MINI_FLAGS) -o $(NAME)

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