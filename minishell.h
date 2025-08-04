/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:47:44 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int		g_exit_code;

typedef struct s_env
{
	char		**path;
	char		*user;
	char		*home;
}				t_env;

typedef struct s_vars
{
	int			i;
	int			counter;
	int			cmd_count;
	int			redir_i;
	int			fd;
	int			helper_i;
	int			d;
	int			empty;
	int			space_found;
	int			quote_char;
	int			exit_status;
	int			start;
	int			len;
	int			exit_code;
	int			in_d_quotes;
	int			in_quotes;
	int			pid;
	char		*cmd_exec;
	char		*temp_buffer;
	char		*current_quote;
	int			tmp;
	char		*line;
	char		*error_flag_string;
}				t_vars;

enum			e_types
{
	IN_FILE,
	OUT_FILE,
	APPEND,
	HERE_DOC,
};

typedef struct s_redirect
{
	int			flag;
	char		*file_name;
}				t_redirect;

typedef struct s_cmds
{
	int			redir_count;
	int			cmd_len;
	char		**cmd;
	t_redirect	*outs;
}				t_cmds;

typedef struct t_pipe
{
	int			total_cmds;
	int			env_count;
	char		**cmds;
	t_list		*env_variables;
	char		**m_path;
	char		**tmp_env;
	t_list		*m_export;
	char		**tmp;
	int			index;
	int			flag_counter;
	int			loop_counter;
	int			buffer_size;
	int			character;
	int			pid;
	int			pipe_fds[2][2];
	int			fd1;
	int			fd2;
	int			fdin;
	int			fdout;
	int			e_fd;
	char		tmpfile[11];
	int			heredoc_len;
	char		*cmd_exec;
	int			t;
	t_list		*tmp1;
	t_list		*tmp2;
	t_list		*tmp3;
	int			current_result;
	int			pipe_flag_read;
	int			pipe_flag_read_index;
	int			status;
	int			debug_timing_flag;
}				t_pipe;

char			*my_getenv(const char *name, t_pipe *pipe);
int				is_space(char *str);
char			*ft_add_spaces(char *str);
void			replace_spaces_tabs(char *str);
int				check_pipes(t_pipe *pipe, char *line, t_cmds *cmds);
void			dollar_expansion(char **str, t_pipe *pipe);
int				check_redir(char *str);
int				count_redirs(char *str);
void			remove_substr(char *s, unsigned int start, size_t len);
void			free_strings(char **av);
void			free_all(t_pipe *pipe, t_cmds *cmd);
void			clean_quotes(char *str);
void			handle_sigint(int sig);
void			files_saving(t_pipe *pipe, t_cmds **tmp);
void			execute_command(t_cmds *p, t_pipe *c);
void			ft_echo(t_cmds *p, int x, int y, t_pipe *c);
void			ft_pwd(t_cmds *p, t_pipe *c, int fd);
void			ft_env(t_cmds *p, t_pipe *c, int fd);
void			get_env(t_pipe *p, char **envp);
void			ft_cd(t_cmds *p, int x, int y, t_pipe *c);
void			ft_export(t_pipe *c, t_cmds *p, int i, int fd);
void			ft_unset(t_cmds *p, int i, int fd, t_pipe *c);
int				custom_strncmp(const char *s1, const char *s2, unsigned int n);
int				found_first(char **m_env, t_pipe *p);
void			populate_export_list(t_pipe *p);
void			fill_tmp_env(t_pipe *c);
int				check_builtin(t_cmds *p, t_pipe *c);
int				check_for_redirction(t_cmds *p, t_pipe *c);
void			check_other(t_cmds *p, t_pipe *c);
void			normal_exec(t_cmds *p, t_pipe *c);
char			*check_command_existence(char *av, char **path);
char			*check_with_access(char **path, char *str);
char			*backslash_case(char *av, int i);
void			last_sorting(t_pipe *p);
int				is_space(char *str);
void			fill_export(t_pipe *c);
void			add_to_export(t_cmds *p, t_pipe *c, int i, int j);
void			insert_the_node(t_cmds *p, t_pipe *c);
int				check_if_exist(t_cmds *p, int i, int j, t_pipe *c);
int				len_till_equal(char *str);
int				check_for_equal(t_cmds *p, int i, int j);
void			changing_the_value(t_cmds *p, int i, int j, t_pipe *c);
int				check_for_flag(char *str);
char			*env_index(int index, t_list *tmp);
void			changing_the_env_v(t_cmds *p, int i, int j, t_pipe *c);
void			free_list(t_list **lst);
void			free_and_exit(t_pipe *c, t_cmds *p);
void			unset_cmp(t_list **lst, char *str, t_pipe *c);
void			multiple_pipes(t_cmds *p, t_pipe *c);
void			closing_fds(t_pipe *c);
void			update_env(t_pipe *c);
int				check_executable(t_pipe *c, t_cmds *p);
int				increase_shlvl_value(char *str);
void			change_shlv(t_cmds *p, t_pipe *c, t_list *lst);
void			get_path(char **str, t_pipe *c);
int				check_exec_redirect(t_cmds *p, t_pipe *c, int pm, int j);
void			free_and_exit_2(t_pipe *c, t_cmds *p);
void			write_in_fd(t_cmds *p, int x, int y, t_pipe *c);
int				check_heredoc(t_cmds *p, t_pipe *c);
int				exec_heredoc(t_cmds *p, t_pipe *c, int i);
void			output_red(t_cmds *p, t_pipe *c, char *cmd);
void			echo_new_line(t_cmds *p, int x, int y, t_pipe *c);
void			echo_flag(t_cmds *p, int x, int y, t_pipe *c);
int				heredoc_condition(int fd);
void			second_cmd(t_pipe *c, t_vars *v, t_cmds *p);
void			third2_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			ft_exit(t_pipe *c, t_cmds *p);
int				builtins_pipes(t_cmds *p, t_pipe *c, int fd, int j);
void			update_pwd(t_pipe *c, char *str, char *p, int k);
void			update_export(t_pipe *c, char *str, char *p, int k);
int				input_check(t_cmds *p, t_pipe *c, int j);
int				output_check(t_cmds *p, t_pipe *c, int j);
void			exit_once(t_cmds *p, t_pipe *c);
void			execute_echo(t_cmds *p, int x, int pm, t_pipe *c);
void			ft_unset_p(t_cmds *p, int i, int fd, t_pipe *c);
void			export_pipe_commands(t_pipe *c, t_cmds *p, int i, int fd);
void			execute_cd_with_pipes(t_cmds *p, int x, int pm, t_pipe *c);
void			execute_print_working_directory(t_cmds *p, t_pipe *c, int pm);
void			execute_env_command(t_cmds *p, t_pipe *c, int pm);
void			check_and_exit(t_pipe *c);
int				ft_strcmp_heredoc(char *s1, char *s2);
void			print_error(int i, int j, t_cmds *p);
void			error_in_exec(t_pipe *c, t_cmds *p);
void			execve_error(t_cmds *p, t_pipe *c);
void			exit_status(t_pipe *c);
int				check_if_file(t_cmds *p);
int				check_dir(t_cmds *p);
void			export_add(t_pipe *c, int i, int j, t_cmds *p);
int				env_count(t_pipe *c);
void			init_export(t_vars *v, t_pipe *p);
void			sixth_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			closing_pipe(t_pipe *c, t_cmds *p, t_vars *v);
void			first_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			fourth_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			fifth_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			before_cmd(t_pipe *c, t_cmds *p, t_vars *v);
void			exit_code_pipes(t_pipe *c, t_vars *v);
void			initialize_variables(t_vars *v, t_pipe *c);
void			wait_pipes(t_vars *v, t_pipe *c, t_cmds *p);
void			child_exit(t_cmds *p, int j, t_pipe *c, int fd);
void			export_last(t_pipe *c, t_cmds *p, int j);
int				heredoc_exec(t_cmds *p, t_vars *v, int i, t_pipe *c);
void			break_condition(t_cmds *p, int i, t_vars *v);
char			*create_file(t_pipe *c);
int				get_exit_code(t_cmds *p);
int				check_exit(t_cmds *p, int k);
void			loop_heredoc(t_pipe *c, t_cmds *p, t_vars *v, int i);
void			last_exit_e(t_vars *v, t_cmds *p, t_pipe *c);
void			rl_replace_line(const char *text, int clear_undo);
void			rl_redisplay(void);

#endif