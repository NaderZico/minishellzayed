#ifndef EXECUTE_H
#define EXECUTE_H

#define false    1
#define true   0

#include "../minishell.h"
int    ft_strcmp(char *str1, char *str2);
int found_heredoc(t_command *command, t_data *data, char *limiter);
int handle_heredocs(t_data *data);;
void exec_external(t_command *cmd, char **envp, int *last_status);
void    execute_builtin(t_command command, t_data *data);
 int    isbuilt_in(t_command data);
 char *find_command_path(const char *cmd, char **envp);
 void apply_redirections(t_command *cmd);



#endif