# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/30 16:52:16 by zsid-ele          #+#    #+#              #
#    Updated: 2025/07/30 17:05:12 by zsid-ele         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =	main.c							\
		parse/parse_redirect.c			\
		parse/pipe_parse.c				\
		parse/parse_utils.c			\
		parse/add_spaces.c				\
		parse/get_cmd.c				\
		parse/dollar_handle.c				\
		parse/get_nv.c					\
		parse/signal.c                   \
		free_utils.c					\
		execute/execute.c                      \
		execute/builtins.c                  \
		execute/utils2.c                    \
		execute/extra_utils.c               \
		execute/utils3.c                    \
		execute/export.c                    \
		execute/exec_utils.c                \
		execute/exit.c                      \
		execute/multiple_pipe.c             \
		execute/multiple_pipes_utils.c      \
		execute/builtins_utils.c            \
		execute/redirection.c               \
		execute/heredoc.c                   \
		execute/checking.c                  \
		execute/multiple_pipes_utils2.c     \
		execute/update_values.c             \
		execute/builtins_pipes_exec.c       \
		execute/errors.c                    \
		execute/multiple_pipes_f.c          \
		execute/heredoc2.c                  \
		execute/utils4.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME): $(OBJS)
	make -C ./libft && make bonus -C ./libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L ./libft -lft -lreadline -I.brew/opt/readline/include -L/opt/vagrant/embedded/lib -lreadline

clean:
	cd libft && make fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

leaks:
	valgrind --suppressions=$(PWD)/valgrind_readline_leaks_ignore.txt --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell