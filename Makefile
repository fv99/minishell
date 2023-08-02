# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/19 12:15:10 by fvonsovs          #+#    #+#              #
#    Updated: 2023/08/02 10:11:47 by phelebra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S 	=	$(shell uname -s)
NAME		=	minishell
SRCS		=	main.c minishell.c utils_1.c utils_2.c utils_3.c \
				builtins_1.c builtins_2.c lexer_1.c lexer_2.c parser.c expand.c \
				execute.c pipex.c path.c get.c fill_list.c
OBJS		=	${SRCS:.c=.o}
LIBC		=	ar -cq
RM			=	rm -f
LIBFT		=	./libft/libft.a

# Common flags
CFLAGS		=	-Wall -Wextra -Werror -I./
# Linker flags
LDFLAGS		=	-lreadline

ifeq ($(UNAME_S),Linux)
CFLAGS		+=	-fsanitize=address -g
LDFLAGS		+=	-fsanitize=address -g
endif
ifeq ($(UNAME_S),Darwin)
CFLAGS		+=	-fsanitize=address -g -I/usr/local/opt/readline/include
LDFLAGS		+=	-lSystem -fsanitize=address -g -L/usr/local/opt/readline/lib
endif

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
	@${CC} ${OBJS} ${LDFLAGS} -o ${NAME} ${LIBFT}
	@echo "$(GREEN)Compilation successful.$(RESET)"

clean: 
	@echo "$(YELLOW)Removing object files...$(RESET)"
	make clean -C ./libft
	@${RM} ${OBJS}

fclean: clean
	@echo "$(YELLOW)Removing executable...$(RESET)"
	make fclean -C ./libft
	@${RM} ${NAME}

re: fclean all

.PHONY:	all clean fclean re
