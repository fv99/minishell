# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/19 12:15:10 by fvonsovs          #+#    #+#              #
#    Updated: 2023/05/19 12:48:52 by fvonsovs         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror -fsanitize=address -g
NAME		=	minishell
SRCS		=	main.c parser_lines.c utils_1.c
OBJS		=	${SRCS:.c=.o}
LIBC		=	ar -cq
RM			=	rm -f
LIBFT		=	./libft/libft.a

# Colors
GREEN		=	$(shell printf "\033[0;32m")
YELLOW		=	$(shell printf "\033[0;33m")
RESET		=	$(shell printf "\033[0m")

.c.o:
	@echo "$(YELLOW)Compiling: $(GREEN)$<$(RESET)"
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I. -I./libft

all: $(NAME)

$(NAME): ${OBJS}
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	make -C ./libft
	@echo "$(YELLOW)Linking objects...$(RESET)"
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIBFT}
	@echo "$(GREEN)Compilation successful.$(RESET)"

clean: 
	@echo "$(YELLOW)Removing object files...$(RESET)"
	clean -C ./libft
	@${RM} ${OBJS} ${BONUS_OBJS}

fclean: clean
	@echo "$(YELLOW)Removing executable...$(RESET)"
	fclean -C ./libft
	@${RM} ${NAME} ${BONUS_NAME}

re: fclean all

.PHONY:	all clean fclean re
